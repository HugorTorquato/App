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
    pqxx::work tx(*connection);

    pqxx::result result = tx.exec("SELECT 1");  // Test query to verify connection

    std::cerr << "Test query result: " << result[0][0].as<int>() << std::endl;  // Should print 1


    // pqxx::work tx(connection);
    // auto tx = conn.tx();

    Logger::info("Connected to database");

    // std::cerr << "Current folder : " << std::filesystem::current_path() << std::endl;  /// app/backend/build

    // // TODO: Move to the right folder, but also need to create the .sql files

    // tx->exec(readFile("../src/Migration/Migrations/20260124_000_schema_migrations.sql"));

    // auto result = tx->exec("SELECT version FROM schema_migrations;");
    // std::unordered_set<std::string> applied;

    // for (auto row : result) {
    //     applied.insert(row["version"].c_str());
    // }

    // std::cerr << "Applied migrations: ";
    // for (const auto& version : applied) {
    //     std::cerr << version << " ";
    // }
    // std::cerr << std::endl;

    // // Load migration files
    // std::vector<std::pair<std::string, std::string>> migrations = {
    //     {"001", readFile("infra/PostgresDB/Migrations/20260124_001_Create_Residents.sql")}};

    // // Apply pending migrations
    // for (auto& [version, sql] : migrations) {
    //     if (applied.contains(version)) {
    //         Logger::info("Skipping migration " + version);
    //         continue;
    //     }

    //     Logger::info("Applying migration " + version);
    //     tx->exec(sql);
    //     tx->exec_params("INSERT INTO schema_migrations (version) VALUES ($1)", version);
    // }

    // tx->commit();
    Logger::info("Migrations completed successfully");

    // - create migrations table
    // - apply pending migrations

    Logger::info("Migrations completed successfully");
    return 0;
}
