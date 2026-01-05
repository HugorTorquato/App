#pragma once

#include <memory>
#include <pqxx/pqxx>
#include <string>

class DbConnectionFactory {
   public:
    std::shared_ptr<pqxx::connection> createConnection();
};
