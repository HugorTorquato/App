#pragma once

#include <memory>
#include <pqxx/pqxx>
#include <string>

#include "../../Utils/Env.h"

struct DbConfig {
   public:
    std::string host;
    std::string port;
    std::string name;
    std::string user;
    std::string password;

    static DbConfig fromEnv();
};

// This struct "bundles" the connection and its transaction
struct DbSession {
    std::shared_ptr<pqxx::connection> conn;
    std::unique_ptr<pqxx::work> tx;  // Fallback, access the TX directly

    // Convenience methods to execute queries and commit transactions
    pqxx::result exec(const std::string& query);
    void commit();
};

class DbConnectionFactory {
   public:
    explicit DbConnectionFactory(const DbConfig& config);

    DbSession createSession();

   private:
    std::shared_ptr<pqxx::connection> createConnection();
    DbConfig m_config;
};