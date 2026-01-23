#include "DbConnectionFactory.h"

// Helper to build config from Environment
DbConfig DbConfig::fromEnv() {
    auto get = [](const char* key) -> std::string {
        const char* val = std::getenv(key);
        // CRITICAL: Check pointer before converting to std::string
        if (!val || std::string(val).empty()) {
            throw std::runtime_error("Database Configuration Error: Missing environment variable: " + std::string(key));
        }
        return std::string(val);
    };

    return DbConfig{get("DB_HOST"), get("DB_PORT"), get("DB_NAME"), get("DB_USER"), get("DB_PASS")};
}

DbConnectionFactory::DbConnectionFactory(const DbConfig& config) : m_config(config) {}

std::shared_ptr<pqxx::connection> DbConnectionFactory::createConnection() {
    std::string connStr = "host=" + m_config.host + " port=" + m_config.port + " dbname=" + m_config.name +
                          " user=" + m_config.user + " password=" + m_config.password;
    // libpqxx throws an exception if the connection fails, so no need for extra checks here
    return std::make_shared<pqxx::connection>(connStr);
}
