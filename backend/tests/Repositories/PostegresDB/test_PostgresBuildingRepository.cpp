#include <gtest/gtest.h>

#include "../../../src/Repositories/postgresDB/PostgresBuildingRepository.h"
#include "../../Mocks/MockDbConnectionFactory.h"
#include "../../Mocks/MockDbSession.h"

// The unit test layer you have is correct and worth keeping — but it covers the orchestration logic only.
//  These are interaction tests — they verify the repository's behaviour as a dispatcher: does it call the right method
//  on the session, with the right SQL, with the right parameters?

class PostgresSDBBuildingRepositoryTest : public ::testing::Test {
   protected:
    PostgresBuildingRepository* repository = nullptr;

    std::optional<Building> building1;
    std::optional<Building> building2;
    std::optional<Building> building3;

    void SetUp() override {
        repository = new PostgresBuildingRepository(std::move(std::unique_ptr<MockDbConnectionFactory>()));
        building1.emplace(0, "Building A", "123 Main St", 10);
        building2.emplace(0, "Building B", "456 Elm St", 15);
        building3.emplace(0, "Building C", "789 Oak St", 20);
    }

    void TearDown() override {
        delete repository;
        repository = nullptr;
    }

    // Helper: create a repository wired with a mock factory that returns the given session.
    // Ownership of mockSession is transferred; the raw pointer remains valid for setting expectations.
    PostgresBuildingRepository makerepoWithSession(MockDbSession*& outSession) {
        auto session = std::make_unique<MockDbSession>();
        outSession = session.get();

        auto factory = std::make_unique<MockDbConnectionFactory>();
        EXPECT_CALL(*factory, createSession()).WillOnce(::testing::Return(::testing::ByMove(std::move(session))));

        return PostgresBuildingRepository(std::move(factory));
    }
};

// ─── findAll() ───────────────────────────────────────────────────────────────

TEST_F(PostgresSDBBuildingRepositoryTest, FindAllReturnsEmptyVectorWhenNoRows) {
    MockDbSession* mockSession = nullptr;
    auto repo = makerepoWithSession(mockSession);

    EXPECT_CALL(*mockSession, exec("SELECT id, name, address, num_floors FROM buildings;"))
        .WillOnce(::testing::Return(pqxx::result{}));

    auto result = repo.findAll();

    EXPECT_TRUE(result.empty());
}

TEST_F(PostgresSDBBuildingRepositoryTest, FindAllExecutesCorrectSqlQuery) {
    MockDbSession* mockSession = nullptr;
    auto repo = makerepoWithSession(mockSession);

    // Verify the exact query string the repository sends to the session.
    EXPECT_CALL(*mockSession, exec("SELECT id, name, address, num_floors FROM buildings;"))
        .WillOnce(::testing::Return(pqxx::result{}));

    repo.findAll();
}

TEST_F(PostgresSDBBuildingRepositoryTest, FindAllCreatesExactlyOneSession) {
    auto mockSession = std::make_unique<MockDbSession>();
    EXPECT_CALL(*mockSession, exec(::testing::_)).WillOnce(::testing::Return(pqxx::result{}));

    auto mockFactory = std::make_unique<MockDbConnectionFactory>();
    // createSession() must be called exactly once per findAll() call.
    EXPECT_CALL(*mockFactory, createSession())
        .Times(1)
        .WillOnce(::testing::Return(::testing::ByMove(std::move(mockSession))));

    PostgresBuildingRepository repo(std::move(mockFactory));
    repo.findAll();
}

TEST_F(PostgresSDBBuildingRepositoryTest, FindAllDoesNotCallCommit) {
    MockDbSession* mockSession = nullptr;
    auto repo = makerepoWithSession(mockSession);

    EXPECT_CALL(*mockSession, exec(::testing::_)).WillOnce(::testing::Return(pqxx::result{}));
    // commit() must never be called — findAll is a read-only operation.
    EXPECT_CALL(*mockSession, commit()).Times(0);

    repo.findAll();
}

// ─── save() ──────────────────────────────────────────────────────────────────
//
// save() calls execParams(..., vector<string>) then reads result[0]["id"].
// pqxx::result rows cannot be constructed outside libpq, so the happy-path
// return value is covered by integration tests.  Here we make execParams throw
// *before* the result-access line so we can assert every observable mock
// interaction cleanly.

TEST_F(PostgresSDBBuildingRepositoryTest, SaveExecutesCorrectInsertSql) {
    MockDbSession* mockSession = nullptr;
    auto repo = makerepoWithSession(mockSession);  // also asserts createSession() called once

    const std::string expectedSql =
        "INSERT INTO buildings (name, address, num_floors) VALUES ($1, $2, $3) RETURNING id;";

    EXPECT_CALL(*mockSession, execParams(expectedSql, ::testing::An<const std::vector<std::string>&>()))
        .WillOnce(::testing::Throw(std::runtime_error("db")));

    EXPECT_THROW(repo.save(*building1), std::exception);
}

TEST_F(PostgresSDBBuildingRepositoryTest, SavePassesCorrectParamsToQuery) {
    MockDbSession* mockSession = nullptr;
    auto repo = makerepoWithSession(mockSession);

    const std::vector<std::string> expectedParams = {"Building A", "123 Main St", "10"};

    EXPECT_CALL(*mockSession, execParams(::testing::_, expectedParams))
        .WillOnce(::testing::Throw(std::runtime_error("db")));

    EXPECT_THROW(repo.save(*building1), std::exception);
}

TEST_F(PostgresSDBBuildingRepositoryTest, SaveCreatesExactlyOneSession) {
    auto mockSession = std::make_unique<MockDbSession>();
    EXPECT_CALL(*mockSession, execParams(::testing::_, ::testing::An<const std::vector<std::string>&>()))
        .WillOnce(::testing::Throw(std::runtime_error("db")));

    auto mockFactory = std::make_unique<MockDbConnectionFactory>();
    EXPECT_CALL(*mockFactory, createSession())
        .Times(1)
        .WillOnce(::testing::Return(::testing::ByMove(std::move(mockSession))));

    PostgresBuildingRepository repo(std::move(mockFactory));
    EXPECT_THROW(repo.save(*building1), std::exception);
}

TEST_F(PostgresSDBBuildingRepositoryTest, SaveThrowsWhenDatabaseReturnsNoRows) {
    // save() must throw rather than silently return 0 — the caller cannot
    // distinguish id=0 from a genuine failure, which leads to silent data corruption.
    MockDbSession* mockSession = nullptr;
    auto repo = makerepoWithSession(mockSession);

    EXPECT_CALL(*mockSession, execParams(::testing::_, ::testing::An<const std::vector<std::string>&>()))
        .WillOnce(::testing::Return(pqxx::result{}));  // empty — no RETURNING id row
    EXPECT_CALL(*mockSession, commit()).Times(0);       // must not commit on failure

    EXPECT_THROW(repo.save(*building1), std::runtime_error);
}

TEST_F(PostgresSDBBuildingRepositoryTest, SaveDoesNotCommitWhenExecParamsFails) {
    // If the DB call itself throws, commit() must never be called — committing
    // after a failed execParams would be undefined behaviour at the transaction level.
    MockDbSession* mockSession = nullptr;
    auto repo = makerepoWithSession(mockSession);

    EXPECT_CALL(*mockSession, execParams(::testing::_, ::testing::An<const std::vector<std::string>&>()))
        .WillOnce(::testing::Throw(std::runtime_error("connection lost")));
    EXPECT_CALL(*mockSession, commit()).Times(0);

    EXPECT_THROW(repo.save(*building1), std::runtime_error);
}

// NOTE: SaveCommitsOnSuccess — verifying that commit() IS called on a successful
// INSERT is the most important commit() test, but it requires a real pqxx::result
// row (pqxx::result cannot be constructed with data outside libpq).
// This must be covered by an integration test.
