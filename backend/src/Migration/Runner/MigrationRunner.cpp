#include "MigrationRunner.h"

#include <algorithm>
#include <fstream>
#include <unordered_set>

MigrationFile::MigrationFile(const std::string& version, const std::filesystem::path& path)
    : version(version), path(path) {}

MigrationRunner::MigrationRunner(IDbConnectionFactory& factory) : m_factory(factory) {}

std::vector<MigrationFile> MigrationRunner::discoverMigrations(const std::string& dir) {
    Logger::info("[MigrationRunner::discoverMigrations] Starting discoverMigrations...");

    std::vector<MigrationFile> migrations;
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".sql") {
            std::string filename = entry.path().filename().string();
            size_t first = filename.find('_');
            size_t second = filename.find('_', first + 1);
            std::string version = filename.substr(first + 1, second - first - 1);
            migrations.emplace_back(version, entry.path());
            Logger::info("[MigrationRunner::discoverMigrations] Found migration: " + filename +
                         " with version: " + version);
        }
    }

    std::sort(migrations.begin(), migrations.end(), [](const MigrationFile& a, const MigrationFile& b) {
        return a.path.filename().string() < b.path.filename().string();
    });

    return migrations;
}

std::string MigrationRunner::readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

std::unordered_set<std::string> MigrationRunner::getAppliedMigrations(IDbSession& session) {
    std::unordered_set<std::string> applied;
    pqxx::result rows = session.exec("SELECT version FROM schema_migrations;");
    for (auto row : rows) {
        applied.insert(row["version"].c_str());
    }
    return applied;
}

void MigrationRunner::ensureSchemaTable(IDbSession& session) {
    session.exec(
        "CREATE TABLE IF NOT EXISTS schema_migrations ("
        "    version     TEXT PRIMARY KEY,"
        "    applied_at  TIMESTAMPTZ NOT NULL DEFAULT now()"
        ");");
}

void MigrationRunner::run(const std::string& migrationsDir) {
    Logger::info("[MigrationRunner::run] Starting migration runner...");

    auto session = m_factory.createSession();

    ensureSchemaTable(*session);

    auto migrations = discoverMigrations(migrationsDir);
    auto applied = getAppliedMigrations(*session);

    for (auto& [version, path] : migrations) {
        if (applied.contains(version)) {
            Logger::info("[MigrationRunner::run] Skipping migration " + path.filename().string());
            continue;
        }
        std::string sql = readFile(path.string());
        Logger::info("[MigrationRunner::run] Applying migration " + path.filename().string());
        session->exec(sql);
        session->execParams("INSERT INTO schema_migrations (version) VALUES ($1)", version);
    }

    session->commit();
    Logger::info("[MigrationRunner::run] Migrations completed successfully");
}

std::vector<std::string> MigrationRunner::getTableNames(IDbSession& session) {
    std::vector<std::string> tables;
    pqxx::result rows = session.exec("SELECT tablename FROM pg_tables WHERE schemaname = 'public';");
    for (auto row : rows) {
        tables.push_back(row["tablename"].c_str());
    }
    return tables;
}

void MigrationRunner::resetSchemas() {
    Logger::info("[MigrationRunner::resetSchemas] Dropping all tables...");
    auto session = m_factory.createSession();
    for (const auto& table : getTableNames(*session)) {
        Logger::info("[MigrationRunner::resetSchemas] Dropping: " + table);
        session->exec("DROP TABLE IF EXISTS " + table + " CASCADE;");
    }
    session->commit();
    Logger::info("[MigrationRunner::resetSchemas] Reset complete.");
}
