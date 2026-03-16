#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_set>

#include "../../Mocks/MockDbConnectionFactory.h"
#include "../../Mocks/MockDbSession.h"
#include "../../src/Migration/Runner/MigrationRunner.h"

// ─── Test subclass ────────────────────────────────────────────────────────────
// Exposes protected methods and allows overriding getAppliedMigrations for run() tests.

class TestMigrationRunner : public MigrationRunner {
   public:
    explicit TestMigrationRunner(IDbConnectionFactory& factory) : MigrationRunner(factory) {}

    // Make readFile publicly callable in tests
    using MigrationRunner::readFile;

    // Controllable applied-migrations set (used by Run_* tests)
    std::unordered_set<std::string> appliedOverride;
    bool useAppliedOverride = false;

    // Controllable table names (used by Reset_* tests)
    std::vector<std::string> tableNamesOverride;
    bool useTableNamesOverride = false;

   protected:
    std::unordered_set<std::string> getAppliedMigrations(IDbSession&) override {
        if (useAppliedOverride) return appliedOverride;
        return {};
    }

    virtual std::vector<std::string> getTableNames(IDbSession&) {
        if (useTableNamesOverride) return tableNamesOverride;
        return {};
    }
};

// ─── Helpers ──────────────────────────────────────────────────────────────────

static void writeFile(const std::filesystem::path& path, const std::string& content) {
    std::ofstream f(path);
    f << content;
}

// ─── Fixture: MigrationFileTest ───────────────────────────────────────────────

class MigrationFileTest : public ::testing::Test {
   protected:
    std::string file;
    std::filesystem::path testMigrationsDir;

    void SetUp() override {
        file = "File1";
        testMigrationsDir = "../../src/Migration/TestMigrations";
    }
};

// ─── Fixture: MigrationRunnerFsTest ──────────────────────────────────────────
// For discoverMigrations and readFile tests — real filesystem, no DB.

class MigrationRunnerFsTest : public ::testing::Test {
   protected:
    std::filesystem::path tmpDir;
    MockDbConnectionFactory factory;
    TestMigrationRunner* runner = nullptr;

    void SetUp() override {
        tmpDir = std::filesystem::temp_directory_path() / "mr_test";
        std::filesystem::create_directories(tmpDir);

        writeFile(tmpDir / "20260124_000_schema_migrations.sql", "CREATE TABLE schema_migrations();");
        writeFile(tmpDir / "20260124_001_Create_Residents.sql", "CREATE TABLE residents();");

        runner = new TestMigrationRunner(factory);
    }

    void TearDown() override {
        delete runner;
        std::filesystem::remove_all(tmpDir);
    }
};

// ─── Fixture: MigrationRunnerRunTest ─────────────────────────────────────────
// For run() tests — mocked DB session.

// ─── Fixture: MigrationRunnerResetTest ───────────────────────────────────────
// For resetSchemas() tests — mocked DB session, no filesystem needed.

class MigrationRunnerResetTest : public ::testing::Test {
   protected:
    MockDbConnectionFactory factory;
    MockDbSession* rawSession = nullptr;
    TestMigrationRunner* runner = nullptr;

    void SetUp() override {
        rawSession = new MockDbSession();

        ON_CALL(*rawSession, exec(testing::_)).WillByDefault(testing::Return(pqxx::result{}));

        EXPECT_CALL(factory, createSession())
            .WillOnce(testing::Return(testing::ByMove(std::unique_ptr<IDbSession>(rawSession))));

        runner = new TestMigrationRunner(factory);
        runner->useTableNamesOverride = true;
    }

    void TearDown() override { delete runner; }
};

class MigrationRunnerRunTest : public ::testing::Test {
   protected:
    std::filesystem::path tmpDir;
    MockDbConnectionFactory factory;
    MockDbSession* rawSession = nullptr;
    TestMigrationRunner* runner = nullptr;

    void SetUp() override {
        tmpDir = std::filesystem::temp_directory_path() / "mr_run_test";
        std::filesystem::create_directories(tmpDir);
        writeFile(tmpDir / "20260124_000_schema_migrations.sql", "SELECT 1;");

        rawSession = new MockDbSession();

        // ensureSchemaTable and any other exec calls default to returning empty result
        ON_CALL(*rawSession, exec(testing::_)).WillByDefault(testing::Return(pqxx::result{}));
        ON_CALL(*rawSession, execParams(testing::_, testing::_)).WillByDefault(testing::Return(pqxx::result{}));

        EXPECT_CALL(factory, createSession())
            .WillOnce(testing::Return(testing::ByMove(std::unique_ptr<IDbSession>(rawSession))));

        runner = new TestMigrationRunner(factory);
        runner->useAppliedOverride = true;  // control applied set per-test
    }

    void TearDown() override {
        delete runner;
        std::filesystem::remove_all(tmpDir);
    }
};

// ═══════════════════════════════════════════════════════════════════════════════
// Tests
// ═══════════════════════════════════════════════════════════════════════════════

// 1 ── MigrationFile stores version and path correctly
TEST_F(MigrationFileTest, MigrationFileStruct) {
    MigrationFile migrationFile{file, testMigrationsDir};
    EXPECT_EQ(migrationFile.version, file);
    EXPECT_EQ(migrationFile.path.string(), testMigrationsDir.string());
}

// 2 ── discoverMigrations finds the two seeded .sql files
TEST_F(MigrationRunnerFsTest, DiscoverMigrations) {
    auto migrations = runner->discoverMigrations(tmpDir.string());
    EXPECT_GE(migrations.size(), 2u);
    EXPECT_EQ(migrations[0].version, "000");
    EXPECT_EQ(migrations[1].version, "001");
    EXPECT_EQ(migrations[0].path.filename().string(), "20260124_000_schema_migrations.sql");
    EXPECT_EQ(migrations[1].path.filename().string(), "20260124_001_Create_Residents.sql");
}

// 3 ── discoverMigrations returns empty vector for an empty directory
TEST_F(MigrationRunnerFsTest, DiscoverMigrations_EmptyDir) {
    std::filesystem::path emptyDir = tmpDir / "empty";
    std::filesystem::create_directory(emptyDir);

    auto migrations = runner->discoverMigrations(emptyDir.string());
    EXPECT_TRUE(migrations.empty());
}

// 4 ── discoverMigrations ignores non-.sql files
TEST_F(MigrationRunnerFsTest, DiscoverMigrations_IgnoresNonSqlFiles) {
    writeFile(tmpDir / "20260124_002_notes.txt", "some note");
    writeFile(tmpDir / "20260124_003_readme.md", "# readme");

    auto migrations = runner->discoverMigrations(tmpDir.string());
    for (const auto& m : migrations) {
        EXPECT_EQ(m.path.extension().string(), ".sql");
    }
}

// 5 ── discoverMigrations returns results sorted by version
TEST_F(MigrationRunnerFsTest, DiscoverMigrations_IsSorted) {
    auto migrations = runner->discoverMigrations(tmpDir.string());
    for (size_t i = 1; i < migrations.size(); ++i) {
        EXPECT_LT(migrations[i - 1].version, migrations[i].version);
    }
}

// 6 ── readFile returns the exact contents of a file
TEST_F(MigrationRunnerFsTest, ReadFile_ReturnsContents) {
    auto path = tmpDir / "20260124_000_schema_migrations.sql";
    auto contents = runner->readFile(path.string());
    EXPECT_EQ(contents, "CREATE TABLE schema_migrations();");
}

// 7 ── readFile throws std::runtime_error for a missing path
TEST_F(MigrationRunnerFsTest, ReadFile_ThrowsOnMissing) {
    EXPECT_THROW(runner->readFile("/nonexistent/path.sql"), std::runtime_error);
}

// 8 ── run() executes the SQL and inserts the version for a pending migration
TEST_F(MigrationRunnerRunTest, Run_AppliesPendingMigrations) {
    runner->appliedOverride = {};  // nothing applied yet

    EXPECT_CALL(*rawSession, exec(testing::HasSubstr("CREATE TABLE IF NOT EXISTS schema_migrations"))).Times(1);
    EXPECT_CALL(*rawSession, exec("SELECT 1;")).Times(1);
    EXPECT_CALL(*rawSession, execParams("INSERT INTO schema_migrations (version) VALUES ($1)", "000")).Times(1);
    EXPECT_CALL(*rawSession, commit()).Times(1);

    runner->run(tmpDir.string());
}

// 9 ── run() skips migrations whose version is already in the applied set
TEST_F(MigrationRunnerRunTest, Run_SkipsAlreadyApplied) {
    runner->appliedOverride = {"000"};

    EXPECT_CALL(*rawSession, exec(testing::HasSubstr("CREATE TABLE IF NOT EXISTS schema_migrations"))).Times(1);
    EXPECT_CALL(*rawSession, exec("SELECT 1;")).Times(0);
    EXPECT_CALL(*rawSession, execParams(testing::_, testing::_)).Times(0);

    runner->run(tmpDir.string());
}

// 10 ── run() always calls commit() exactly once
TEST_F(MigrationRunnerRunTest, Run_CommitsSession) {
    runner->appliedOverride = {};

    EXPECT_CALL(*rawSession, exec(testing::_)).Times(testing::AnyNumber());
    EXPECT_CALL(*rawSession, execParams(testing::_, testing::_)).Times(testing::AnyNumber());
    EXPECT_CALL(*rawSession, commit()).Times(1);

    runner->run(tmpDir.string());
}

// 11 ── run() with empty migrations dir: no exec/execParams, commit still called
TEST_F(MigrationRunnerRunTest, Run_EmptyMigrationsDir) {
    std::filesystem::path emptyDir = tmpDir / "empty";
    std::filesystem::create_directory(emptyDir);
    runner->appliedOverride = {};

    EXPECT_CALL(*rawSession, exec(testing::_)).Times(0);
    EXPECT_CALL(*rawSession, exec(testing::HasSubstr("CREATE TABLE IF NOT EXISTS schema_migrations"))).Times(1);
    EXPECT_CALL(*rawSession, execParams(testing::_, testing::_)).Times(0);

    runner->run(emptyDir.string());
}

// 12 ── resetSchemas() issues no DROP when there are no tables
TEST_F(MigrationRunnerResetTest, Reset_NoTables_NothingDropped) {
    runner->tableNamesOverride = {};

    EXPECT_CALL(*rawSession, exec(testing::HasSubstr("DROP"))).Times(0);
    EXPECT_CALL(*rawSession, commit()).Times(1);

    runner->resetSchemas();
}

// 13 ── resetSchemas() issues one DROP per table returned by getTableNames
TEST_F(MigrationRunnerResetTest, Reset_DropsEachTable) {
    runner->tableNamesOverride = {"residents", "schema_migrations"};

    EXPECT_CALL(*rawSession, exec(testing::HasSubstr("residents"))).Times(1);
    EXPECT_CALL(*rawSession, exec(testing::HasSubstr("schema_migrations"))).Times(1);
    EXPECT_CALL(*rawSession, commit()).Times(1);

    runner->resetSchemas();
}

// 14 ── resetSchemas() uses CASCADE so FK constraints don't block drops
TEST_F(MigrationRunnerResetTest, Reset_DropUsesCascade) {
    runner->tableNamesOverride = {"residents"};

    EXPECT_CALL(*rawSession, exec(testing::AllOf(testing::HasSubstr("DROP TABLE"), testing::HasSubstr("CASCADE"))))
        .Times(1);
    EXPECT_CALL(*rawSession, commit()).Times(1);

    runner->resetSchemas();
}

// 15 ── resetSchemas() calls commit() exactly once regardless of table count
TEST_F(MigrationRunnerResetTest, Reset_CommitsOnce) {
    runner->tableNamesOverride = {"table_a", "table_b", "table_c"};

    EXPECT_CALL(*rawSession, exec(testing::_)).Times(testing::AnyNumber());
    EXPECT_CALL(*rawSession, commit()).Times(1);

    runner->resetSchemas();
}
