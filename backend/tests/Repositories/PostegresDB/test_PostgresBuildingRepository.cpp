#include <gtest/gtest.h>

#include "../../../src/Repositories/postgresDB/PostgresBuildingRepository.h"
#include "../../Mocks/MockDbConnectionFactory.h"
#include "../../Mocks/MockDbSession.h"

// =============================================================================
// TESTING STRATEGY
//
// These are *interaction tests*. The repository is a thin dispatcher between
// the service layer and the database session. What we can verify here:
//
//   ✔  The correct SQL string is sent to the session
//   ✔  Parameters are passed in the right order and with the right types
//   ✔  The session lifecycle is correct (open once, commit only on writes)
//   ✔  Failures propagate correctly instead of being swallowed silently
//
// What we CANNOT verify in unit tests:
//
//   ✘  The happy path of save() returning the assigned id
//   ✘  findAll() returning populated Building objects
//   ✘  mapRowToBuilding() field mapping (column names, type casts)
//
// Reason: pqxx::result cannot be constructed with fake rows outside libpq —
// rows only exist inside a real database result. Those paths belong in the
// integration test suite.
//
// Workaround used in save() tests: we make execParams() throw before the code
// reaches result[0]["id"], so EXPECT_CALL matchers can fire and be verified
// cleanly without a secondary crash on the empty result.
// =============================================================================

// -----------------------------------------------------------------------------
// Fixture
// -----------------------------------------------------------------------------

class PostgresSDBBuildingRepositoryTest : public ::testing::Test {
   protected:
    std::optional<Building> building1;
    std::optional<Building> building2;
    std::optional<Building> building3;

    void SetUp() override {
        building1.emplace(0, "Building A", "123 Main St", 10);
        building2.emplace(0, "Building B", "456 Elm St", 15);
        building3.emplace(0, "Building C", "789 Oak St", 20);
    }

    // Creates a repository pre-wired with a mock factory that will return
    // `outSession` when createSession() is called. The factory expectation
    // (Times(1)) is set here, so every test that uses this helper implicitly
    // asserts that exactly one session is opened.
    PostgresBuildingRepository makeRepoWithSession(MockDbSession*& outSession) {
        auto session = std::make_unique<MockDbSession>();
        outSession = session.get();

        auto factory = std::make_unique<MockDbConnectionFactory>();
        EXPECT_CALL(*factory, createSession()).WillOnce(::testing::Return(::testing::ByMove(std::move(session))));

        return PostgresBuildingRepository(std::move(factory));
    }
};

// -----------------------------------------------------------------------------
// findAll()
//
// Execution path:
//   createSession() → exec(SELECT ...) → iterate rows → return vector
//
// No commit() — SELECT is read-only and must never open a write transaction.
// -----------------------------------------------------------------------------

TEST_F(PostgresSDBBuildingRepositoryTest, FindAllExecutesCorrectSqlQuery) {
    // Guards against typos in the SELECT (wrong table, missing column, etc.)
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    EXPECT_CALL(*session, exec("SELECT id, name, address, num_floors FROM buildings;"))
        .WillOnce(::testing::Return(pqxx::result{}));

    repo.findAll();
}

TEST_F(PostgresSDBBuildingRepositoryTest, FindAllReturnsEmptyVectorWhenNoRows) {
    // An empty result from the DB must produce an empty vector, not a crash.
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    EXPECT_CALL(*session, exec(::testing::_)).WillOnce(::testing::Return(pqxx::result{}));

    EXPECT_TRUE(repo.findAll().empty());
}

TEST_F(PostgresSDBBuildingRepositoryTest, FindAllDoesNotCommit) {
    // SELECT must never trigger a commit — committing a read-only session
    // wastes a round-trip and may interfere with surrounding transactions.
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    EXPECT_CALL(*session, exec(::testing::_)).WillOnce(::testing::Return(pqxx::result{}));
    EXPECT_CALL(*session, commit()).Times(0);

    repo.findAll();
}

// -----------------------------------------------------------------------------
// save()
//
// Execution path:
//   createSession() → execParams(INSERT ... RETURNING id) → check result
//     → throw if empty → read result[0]["id"] → commit() → return id
//
// Note: the happy path (commit IS called, correct id returned) requires a real
// pqxx::result row. See integration tests for that coverage.
// -----------------------------------------------------------------------------

TEST_F(PostgresSDBBuildingRepositoryTest, SaveExecutesCorrectInsertSql) {
    // Guards against typos in the INSERT (wrong table, wrong column names,
    // missing RETURNING clause that would break id extraction).
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    const std::string expectedSql =
        "INSERT INTO buildings (name, address, num_floors) VALUES ($1, $2, $3) RETURNING id;";

    EXPECT_CALL(*session, execParams(expectedSql, ::testing::An<const std::vector<std::string>&>()))
        .WillOnce(::testing::Throw(std::runtime_error("db")));  // throw to skip result access

    EXPECT_THROW(repo.save(*building1), std::exception);
}

TEST_F(PostgresSDBBuildingRepositoryTest, SavePassesCorrectParamsToQuery) {
    // Guards against swapped arguments (e.g. address sent where name is expected)
    // and against numFloors not being converted to string for the query.
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    const std::vector<std::string> expectedParams = {"Building A", "123 Main St", "10"};

    EXPECT_CALL(*session, execParams(::testing::_, expectedParams))
        .WillOnce(::testing::Throw(std::runtime_error("db")));  // throw to skip result access

    EXPECT_THROW(repo.save(*building1), std::exception);
}

TEST_F(PostgresSDBBuildingRepositoryTest, SaveThrowsWhenDatabaseReturnsNoRows) {
    // When INSERT ... RETURNING id produces no rows, save() must throw rather
    // than return 0. Returning 0 would be silently stored as the building's id,
    // making subsequent findById(0) and update() calls operate on the wrong row.
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    EXPECT_CALL(*session, execParams(::testing::_, ::testing::An<const std::vector<std::string>&>()))
        .WillOnce(::testing::Return(pqxx::result{}));  // empty — no RETURNING id row
    EXPECT_CALL(*session, commit()).Times(0);          // must not commit if we have no id

    EXPECT_THROW(repo.save(*building1), std::runtime_error);
}

TEST_F(PostgresSDBBuildingRepositoryTest, SaveDoesNotCommitWhenExecParamsFails) {
    // If the DB call itself throws (e.g. connection lost, constraint violation),
    // commit() must never be called — committing after a failed statement leaves
    // the transaction in an undefined state.
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    EXPECT_CALL(*session, execParams(::testing::_, ::testing::An<const std::vector<std::string>&>()))
        .WillOnce(::testing::Throw(std::runtime_error("connection lost")));
    EXPECT_CALL(*session, commit()).Times(0);

    EXPECT_THROW(repo.save(*building1), std::runtime_error);
}

// -----------------------------------------------------------------------------
// findById(int id)
//
// Execution path:
//   createSession() → execParams(SELECT ... WHERE id = $1) → return nullopt
//     if empty, else mapRowToBuilding(result[0]) → return optional<Building>
//
// No commit() — SELECT is read-only.
// Note: the found-building path requires a real pqxx::result row; covered by
// integration tests.
// -----------------------------------------------------------------------------

TEST_F(PostgresSDBBuildingRepositoryTest, FindByIdExecutesCorrectSql) {
    // Guards against wrong table, missing WHERE clause, or wrong column list.
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    EXPECT_CALL(*session, execParams("SELECT id, name, address, num_floors FROM buildings WHERE id = $1;",
                                     ::testing::An<const std::vector<std::string>&>()))
        .WillOnce(::testing::Return(pqxx::result{}));

    repo.findById(1);
}

TEST_F(PostgresSDBBuildingRepositoryTest, FindByIdPassesIdAsStringParam) {
    // The id must be converted to string and passed as the only parameter.
    // Guards against the id being omitted or passed as a raw int.
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    EXPECT_CALL(*session, execParams(::testing::_, std::vector<std::string>{"42"}))
        .WillOnce(::testing::Return(pqxx::result{}));

    repo.findById(42);
}

TEST_F(PostgresSDBBuildingRepositoryTest, FindByIdReturnsNulloptWhenNotFound) {
    // An empty result (no matching row) must return nullopt — not a crash,
    // not a default-constructed Building.
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    EXPECT_CALL(*session, execParams(::testing::_, ::testing::An<const std::vector<std::string>&>()))
        .WillOnce(::testing::Return(pqxx::result{}));

    EXPECT_EQ(repo.findById(1), std::nullopt);
}

TEST_F(PostgresSDBBuildingRepositoryTest, FindByIdDoesNotCommit) {
    // SELECT must never trigger a commit.
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    EXPECT_CALL(*session, execParams(::testing::_, ::testing::An<const std::vector<std::string>&>()))
        .WillOnce(::testing::Return(pqxx::result{}));
    EXPECT_CALL(*session, commit()).Times(0);

    repo.findById(1);
}

// -----------------------------------------------------------------------------
// update(const Building& building)
//
// Execution path:
//   createSession() → execParams(UPDATE ... RETURNING id) → throw if empty
//     → read result[0]["id"] → commit()
//
// update() now reads RETURNING id, same pattern as save(). An empty result
// means the row was not found and the method throws before reaching commit().
// Consequence: commit() on success is not unit-testable (needs a real row).
// See integration tests for that coverage.
// -----------------------------------------------------------------------------

TEST_F(PostgresSDBBuildingRepositoryTest, UpdateExecutesCorrectSql) {
    // Guards against wrong table, wrong SET columns, or missing WHERE clause.
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    EXPECT_CALL(*session,
                execParams("UPDATE buildings SET name = $1, address = $2, num_floors = $3 WHERE id = $4 RETURNING id;",
                           ::testing::An<const std::vector<std::string>&>()))
        .WillOnce(::testing::Throw(std::runtime_error("db")));  // throw to skip result access

    EXPECT_THROW(repo.update(*building1), std::exception);
}

TEST_F(PostgresSDBBuildingRepositoryTest, UpdatePassesCorrectParams) {
    // Guards against wrong parameter order (e.g. id not last) and against
    // numeric fields not being converted to string.
    // building1: name="Building A", address="123 Main St", numFloors=10, id=0
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    const std::vector<std::string> expectedParams = {"Building A", "123 Main St", "10", "0"};

    EXPECT_CALL(*session, execParams(::testing::_, expectedParams))
        .WillOnce(::testing::Throw(std::runtime_error("db")));  // throw to skip result access

    EXPECT_THROW(repo.update(*building1), std::exception);
}

TEST_F(PostgresSDBBuildingRepositoryTest, UpdateThrowsWhenDatabaseReturnsNoRows) {
    // If UPDATE matches no row (building not found), the RETURNING clause
    // produces an empty result. update() must throw rather than silently
    // succeed with a stale id.
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    EXPECT_CALL(*session, execParams(::testing::_, ::testing::An<const std::vector<std::string>&>()))
        .WillOnce(::testing::Return(pqxx::result{}));  // empty — no RETURNING id row
    EXPECT_CALL(*session, commit()).Times(0);          // must not commit if update found nothing

    EXPECT_THROW(repo.update(*building1), std::runtime_error);
}

TEST_F(PostgresSDBBuildingRepositoryTest, UpdateDoesNotCommitWhenExecParamsFails) {
    // If the DB call itself throws (connection lost, constraint violation, etc.),
    // commit() must not be called.
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    EXPECT_CALL(*session, execParams(::testing::_, ::testing::An<const std::vector<std::string>&>()))
        .WillOnce(::testing::Throw(std::runtime_error("connection lost")));
    EXPECT_CALL(*session, commit()).Times(0);

    EXPECT_THROW(repo.update(*building1), std::runtime_error);
}

// -----------------------------------------------------------------------------
// remove(int id)
//
// Execution path:
//   createSession() → execParams(DELETE ... WHERE id = $1)
//     → throw if affected_rows() == 0 → commit()
//
// remove() checks affected_rows() on the result. A default-constructed
// pqxx::result has affected_rows() == 0, so the method throws before
// reaching commit(). commit() on success is not unit-testable (needs a real
// affected row count from libpq). See integration tests for that coverage.
// -----------------------------------------------------------------------------

TEST_F(PostgresSDBBuildingRepositoryTest, RemoveExecutesCorrectSql) {
    // Guards against wrong table or missing WHERE clause (which would delete all rows).
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    EXPECT_CALL(*session,
                execParams("DELETE FROM buildings WHERE id = $1;", ::testing::An<const std::vector<std::string>&>()))
        .WillOnce(::testing::Throw(std::runtime_error("db")));  // throw to skip affected_rows check

    EXPECT_THROW(repo.remove(1), std::exception);
}

TEST_F(PostgresSDBBuildingRepositoryTest, RemovePassesIdAsStringParam) {
    // The id must be converted to string and passed as the only parameter.
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    EXPECT_CALL(*session, execParams(::testing::_, std::vector<std::string>{"7"}))
        .WillOnce(::testing::Throw(std::runtime_error("db")));  // throw to skip affected_rows check

    EXPECT_THROW(repo.remove(7), std::exception);
}

TEST_F(PostgresSDBBuildingRepositoryTest, RemoveThrowsWhenBuildingNotFound) {
    // If DELETE matches no row (building not found), affected_rows() == 0 and
    // remove() must throw — silently succeeding on a missing id hides logic bugs
    // in the caller.
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    EXPECT_CALL(*session, execParams(::testing::_, ::testing::An<const std::vector<std::string>&>()))
        .WillOnce(::testing::Return(pqxx::result{}));  // affected_rows() == 0
    EXPECT_CALL(*session, commit()).Times(0);          // must not commit if nothing was deleted

    EXPECT_THROW(repo.remove(1), std::runtime_error);
}

TEST_F(PostgresSDBBuildingRepositoryTest, RemoveDoesNotCommitWhenExecParamsFails) {
    // If the DB call throws, commit() must not be called.
    MockDbSession* session = nullptr;
    auto repo = makeRepoWithSession(session);

    EXPECT_CALL(*session, execParams(::testing::_, ::testing::An<const std::vector<std::string>&>()))
        .WillOnce(::testing::Throw(std::runtime_error("connection lost")));
    EXPECT_CALL(*session, commit()).Times(0);

    EXPECT_THROW(repo.remove(1), std::runtime_error);
}
