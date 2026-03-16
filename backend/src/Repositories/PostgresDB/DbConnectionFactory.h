#pragma once

#include <memory>
#include <pqxx/pqxx>
#include <string>

#include "../../Utils/Env.h"
#include "IDbConnectionFactory.h"
#include "IDbSession.h"

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
struct DbSession : public IDbSession {
    std::shared_ptr<pqxx::connection> conn;
    std::unique_ptr<pqxx::work> tx;

    pqxx::result exec(const std::string& query) override;
    pqxx::result execParams(const std::string& query, const std::string& param) override;
    pqxx::result execParams(const std::string& query, const std::vector<std::string>& params) override;
    void commit() override;
};

class DbConnectionFactory : public IDbConnectionFactory {
   public:
    explicit DbConnectionFactory(const DbConfig& config);

    std::unique_ptr<IDbSession> createSession() override;

   private:
    std::shared_ptr<pqxx::connection> createConnection();
    DbConfig m_config;
};