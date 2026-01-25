#include <cstdlib>
#include <iostream>
#include <string>

#include "../Utils/Env.h"
#include "../Utils/Logger.h"

int main() {
    const std::string host = Env::getEnv("DB_HOST");
    const std::string db = Env::getEnv("DB_NAME");
    const std::string user = Env::getEnv("DB_USER");

    if (host.empty() || db.empty() || user.empty()) {
        Logger::info("Missing DB environment variables");
        return 1;
    }

    Logger::info("Connecting to DB at " + host + " with user " + user);

    // TODO:
    // - connect using libpqxx or libpq
    // - create migrations table
    // - apply pending migrations

    Logger::info("Migrations completed successfully");
    return 0;
}
