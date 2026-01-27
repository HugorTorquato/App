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

class DbConnectionFactory {
   public:
    explicit DbConnectionFactory(const DbConfig& config);

    std::shared_ptr<pqxx::connection> createConnection();

    std::shared_ptr<pqxx::work> tx();

   private:
    DbConfig m_config;
};