#include "DbConnectionFactory.h"

std::shared_ptr<pqxx::connection> DbConnectionFactory::createConnection() {
    // Retrieve database connection parameters from environment variables
    const char* host = std::getenv("DB_HOST");
    const char* port = std::getenv("DB_PORT");
    const char* dbname = std::getenv("DB_NAME");

    // Construct the connection string
    std::string connStr = "host=" + std::string(host ? host : "localhost") +
                          " port=" + std::string(port ? port : "5432") +
                          " dbname=" + std::string(dbname ? dbname : "condo");

    // Create and return a shared pointer to the pqxx::connection
    return std::make_shared<pqxx::connection>(connStr);
}