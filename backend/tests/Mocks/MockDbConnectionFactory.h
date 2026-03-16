#pragma once

#include <gmock/gmock.h>

#include "../../src/Repositories/PostgresDB/IDbConnectionFactory.h"

class MockDbConnectionFactory : public IDbConnectionFactory {
   public:
    MOCK_METHOD(std::unique_ptr<IDbSession>, createSession, (), (override));
};
