#include "../Repositories/PostgresDB/DbConnectionFactory.h"
#include "Runner/MigrationRunner.h"
#include "Utils/Logger.h"

// TODO:
//  - How do i know the database is clean on each build? Probably this only happends when restarting the conotainer
//  - Create the migration for all the domains

int main(int argc, char* argv[]) {
    DbConfig config = DbConfig::fromEnv();

    auto conn = DbConnectionFactory(config);
    MigrationRunner runner(conn);

    if (bool shouldReset = (argc > 1 && std::string(argv[1]) == "--reset"); shouldReset) {
        if (bool isProd = Env::getEnv("APP_ENV") == "production"; isProd)
            Logger::info("Running in production environment, skipping schema reset!");
        else
            runner.resetSchemas();
    }

    runner.run("../src/Migration/MigrationSchemas");

    return 0;
}
