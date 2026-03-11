#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <unordered_set>
#include <vector>

#include "../Repositories/PostgresDB/DbConnectionFactory.h"
#include "../Utils/Logger.h"

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

int main() {

    DbConfig config = DbConfig::fromEnv();
    auto conn = DbConnectionFactory(config);
    auto connection = conn.createConnection();
    std::unordered_set<std::string> applied;

    pqxx::work tx(*connection);

    Logger::info("Connected to database");

    std::cerr << "Current folder : " << std::filesystem::current_path() << std::endl;  /// app/backend/build

    tx.exec(readFile("../src/Migration/Migrations/20260124_000_schema_migrations.sql"));

    pqxx::result appliedMigrations = tx.exec("SELECT version FROM schema_migrations;");

    for (auto row : appliedMigrations) {
        const std::string version = row["version"].c_str();
        applied.insert(version);
        std::cerr << "Existing migration version: " << version << std::endl;
    }

    // Load migration files
    std::vector<std::pair<std::string, std::string>> migrations = {
        {"001", readFile("../src/Migration/Migrations/20260124_001_Create_Residents.sql")}
    };

    // Apply pending migrations
    for (auto& [version, sql] : migrations) {
        if (applied.contains(version)) {
            Logger::info("Skipping migration " + version);
            continue;
        }

        Logger::info("Applying migration " + version);
        tx.exec(sql);
        tx.exec_params("INSERT INTO schema_migrations (version) VALUES ($1)", version);
    }



    pqxx::result result = tx.exec("SELECT 1");  // Test query to verify connection

    std::cerr << "Test query result: " << result[0][0].as<int>() << std::endl;  // Should print 1

    tx.commit();

    Logger::info("Migrations completed successfully");
    return 0;
}
