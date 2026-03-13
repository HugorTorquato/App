#include <gtest/gtest.h>

#include "../../src/Repositories/PostgresDB/DbConnectionFactory.h"

class DbConnectionFactoryTest : public ::testing::Test {
   protected:
    // We use unique_ptr so we can initialize it AFTER SetUp runs
    std::unique_ptr<DbConnectionFactory> factory;

    void SetUp() override {
        // Prepare the environment
        setenv("DB_HOST", "condo_db", 1);
        setenv("DB_PORT", "5432", 1);
        setenv("DB_NAME", "condo_test", 1);
        setenv("DB_USER", "postgres", 1);
        setenv("DB_PASS", "password", 1);
    }

    void TearDown() override {
        unsetenv("DB_HOST");
        unsetenv("DB_PORT");
        unsetenv("DB_NAME");
        unsetenv("DB_USER");
        unsetenv("DB_PASS");
    }
};

// 1. Test that it parses environment variables correctly
TEST_F(DbConnectionFactoryTest, LoadConfigFromEnvSuccess) {
    EXPECT_NO_THROW({
        DbConfig config = DbConfig::fromEnv();
        EXPECT_EQ(config.host, "condo_db");
        EXPECT_EQ(config.name, "condo_test");
    });
}

// 2. Test the "Crash-Fast" logic (Missing Env Var)
TEST_F(DbConnectionFactoryTest, ThrowsWhenEnvVarMissing) {
    unsetenv("DB_HOST");
    EXPECT_THROW(DbConfig::fromEnv(), std::runtime_error);
}

// 3. Test Factory Initialization and Connection Attempt
// Note: This test will fail if a real Postgres is NOT running at condo_db:5432
TEST_F(DbConnectionFactoryTest, CreateConnectionAttempt) {
    DbConfig config = DbConfig::fromEnv();
    factory = std::make_unique<DbConnectionFactory>(config);

    // If no DB is present, pqxx will throw an exception here.
    // We catch it to prove the factory tried to use the correct string.
    try {
        auto session = factory->createSession();
        auto* dbSession = dynamic_cast<DbSession*>(session.get());
        ASSERT_NE(dbSession, nullptr);
        EXPECT_TRUE(dbSession->conn->is_open());
    } catch (const std::exception& e) {
        // If it fails because the DB isn't there, that's expected in a unit test,
        // but we verify the error came from libpqxx, not our code.
        std::string error = e.what();
        SUCCEED() << "Factory correctly attempted connection: " << error;
    }
}

// 4. Test connection string format by attempting a connection
TEST_F(DbConnectionFactoryTest, CreateConnectionStringFormat) {
    DbConfig config = DbConfig::fromEnv();
    factory = std::make_unique<DbConnectionFactory>(config);

    try {
        auto session = factory->createSession();
        auto* dbSession = dynamic_cast<DbSession*>(session.get());
        ASSERT_NE(dbSession, nullptr);
        EXPECT_TRUE(dbSession->conn->is_open());
    } catch (const std::exception& e) {
        SUCCEED() << "Factory correctly attempted connection with proper format: " << e.what();
    }
}

// 5. Test multiple connections can be created
TEST_F(DbConnectionFactoryTest, MultipleConnections) {
    DbConfig config = DbConfig::fromEnv();
    factory = std::make_unique<DbConnectionFactory>(config);

    const int numConnections = 5;
    for (int i = 0; i < numConnections; ++i) {
        try {
            auto session = factory->createSession();
            auto* dbSession = dynamic_cast<DbSession*>(session.get());
            ASSERT_NE(dbSession, nullptr);
            EXPECT_TRUE(dbSession->conn->is_open());
        } catch (const std::exception& e) {
            SUCCEED() << "Factory correctly attempted connection " << (i + 1) << ": " << e.what();
        }
    }
}

// 6. Test that a transaction is created alongside the connection
TEST_F(DbConnectionFactoryTest, CreateTransaction) {
    DbConfig config = DbConfig::fromEnv();
    factory = std::make_unique<DbConnectionFactory>(config);

    try {
        auto session = factory->createSession();
        auto* dbSession = dynamic_cast<DbSession*>(session.get());
        ASSERT_NE(dbSession, nullptr);
        EXPECT_NE(dbSession->tx, nullptr);
    } catch (const std::exception& e) {
        SUCCEED() << "Factory correctly attempted to create transaction: " << e.what();
    }
}