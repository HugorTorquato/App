#pragma once

#include <gmock/gmock.h>

#include "../../src/Repositories/PostgresDB/IDbSession.h"

class MockDbSession : public IDbSession {
   public:
    MOCK_METHOD(pqxx::result, exec, (const std::string& query), (override));
    MOCK_METHOD(pqxx::result, execParams, (const std::string& query, const std::string& param), (override));
    MOCK_METHOD(void, commit, (), (override));
};