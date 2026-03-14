#include "../Repositories/PostgresDB/DbConnectionFactory.h"
#include "Runner/MigrationRunner.h"

// TODO:
//  - How do i know the database is clean on each build? Probably this only happends when restarting the conotainer
//  - Create the migration for all the domains

int main() {
    DbConfig config = DbConfig::fromEnv();

    auto conn = DbConnectionFactory(config);
    MigrationRunner runner(conn);

    runner.run("../src/Migration/MigrationSchemas");

    return 0;
}
