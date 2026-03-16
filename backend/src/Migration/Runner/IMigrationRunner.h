#pragma once

#include <string>
#include <vector>

struct MigrationFile;

class IMigrationRunner {
   public:
    virtual ~IMigrationRunner() = default;

    virtual void run(const std::string& migrationsDir) = 0;
    virtual std::vector<MigrationFile> discoverMigrations(const std::string& dir) = 0;
    virtual void resetSchemas() = 0;
};
