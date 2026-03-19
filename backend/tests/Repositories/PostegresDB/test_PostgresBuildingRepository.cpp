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
