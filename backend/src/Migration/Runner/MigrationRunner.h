#pragma once

#include <filesystem>
#include <string>
#include <unordered_set>

#include "../../Repositories/PostgresDB/IDbConnectionFactory.h"
#include "../../Repositories/PostgresDB/IDbSession.h"
#include "../../Utils/Logger.h"
#include "IMigrationRunner.h"

struct MigrationFile {
    std::string version;
    std::filesystem::path path;

    MigrationFile(const std::string& version, const std::filesystem::path& path);
};

class MigrationRunner : public IMigrationRunner {
   public:
    explicit MigrationRunner(IDbConnectionFactory& factory);

    void run(const std::string& migrationsDir) override;
    std::vector<MigrationFile> discoverMigrations(const std::string& dir) override;

   protected:
    std::string readFile(const std::string& filePath);
    virtual std::unordered_set<std::string> getAppliedMigrations(IDbSession& session);

   private:
    void ensureSchemaTable(IDbSession& session);

   private:
    IDbConnectionFactory& m_factory;
};