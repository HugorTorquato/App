#pragma once

#include <pqxx/pqxx>
#include <string>

class IDbSession {
   public:
    virtual ~IDbSession() = default;

    virtual pqxx::result exec(const std::string& query) = 0;
    virtual pqxx::result execParams(const std::string& query, const std::string& param) = 0;
    virtual void commit() = 0;
};