#pragma once

#include <memory>
#include <pqxx/pqxx>
#include <string>

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

   private:
    DbConfig m_config;
};