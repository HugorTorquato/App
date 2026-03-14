#pragma once

#include <gmock/gmock.h>

#include "../../src/Migration/Runner/IMigrationRunner.h"

class MockMigrationRunner : public IMigrationRunner {
   public:
    MOCK_METHOD(void, run, (const std::string& migrationsDir), (override));
    MOCK_METHOD(std::vector<MigrationFile>, discoverMigrations, (const std::string& dir), (override));
};
