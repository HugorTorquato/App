#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>

class IDbSession {
   public:
    virtual ~IDbSession() = default;

    virtual pqxx::result exec(const std::string& query) = 0;

    // Single-param: used by findById and remove (WHERE id = $1)
    virtual pqxx::result execParams(const std::string& query, const std::string& param) = 0;

    // Multi-param: used by save and update (INSERT/UPDATE with multiple columns)
    virtual pqxx::result execParams(const std::string& query, const std::vector<std::string>& params) = 0;

    virtual void commit() = 0;
};