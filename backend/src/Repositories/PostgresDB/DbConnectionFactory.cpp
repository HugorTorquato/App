#include "DbConnectionFactory.h"

// Helper to build config from Environment
DbConfig DbConfig::fromEnv() {
    auto get = [](const char* key) -> std::string {
        const std::string val = Env::getEnv(key);
        // CRITICAL: Check pointer before converting to std::string
        if (val.empty()) {
            throw std::runtime_error("Database Configuration Error: Missing environment variable: " + std::string(key));
        }
        return val;
    };

    return DbConfig{get("DB_HOST"), get("DB_PORT"), get("DB_NAME"), get("DB_USER"), get("DB_PASS")};
}

pqxx::result DbSession::exec(const std::string& query) { return tx->exec(query); }

pqxx::result DbSession::execParams(const std::string& query, const std::string& param) {
    return tx->exec_params(query, param);
}

pqxx::result DbSession::execParams(const std::string& query, const std::vector<std::string>& params) {
    return tx->exec_params(query, pqxx::prepare::make_dynamic_params(params));
}

void DbSession::commit() { tx->commit(); }

DbConnectionFactory::DbConnectionFactory(const DbConfig& config) : m_config(config) {}

std::unique_ptr<IDbSession> DbConnectionFactory::createSession() {
    auto connection = createConnection();
    auto transaction = std::make_unique<pqxx::work>(*connection);
    auto session = std::make_unique<DbSession>();
    session->conn = connection;
    session->tx = std::move(transaction);
    return session;
}

std::shared_ptr<pqxx::connection> DbConnectionFactory::createConnection() {
    std::string connStr = "host=" + m_config.host + " port=" + m_config.port + " dbname=" + m_config.name +
                          " user=" + m_config.user + " password=" + m_config.password;
    // libpqxx throws an exception if the connection fails, so no need for extra checks here
    Logger::info("Creating DB connection to " + m_config.host + ":" + m_config.port);
    return std::make_shared<pqxx::connection>(connStr);
}
