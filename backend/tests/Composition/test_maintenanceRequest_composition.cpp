#include <gtest/gtest.h>

#include "../../src/Composition/MaintenanceRequest_composition.h"
#include "../../src/Utils/Env.h"

TEST(MaintenanceRequestCompositionTest, InMemoryRepositorySelection) {
    // Set the environment variable to use InMemory repository
    setenv("REPOSITORY_IMPL", "InMemory", 1);

    MaintenanceRequestModule module("InMemory");

    // Check that the selected repository is of type InMemoryMaintenanceRequestRepository
    EXPECT_NE(dynamic_cast<InMemoryMaintenanceRequestRepository*>(module.repo.get()), nullptr);
    unsetenv("REPOSITORY_IMPL");
}

TEST(MaintenanceRequestCompositionTest, PostgresRepositorySelection) {
    setenv("REPOSITORY_IMPL", "Postgres", 1);

    MaintenanceRequestModule module("Postgres");

    // Check that the selected repository is of type PostgresMaintenanceRequestRepository
    EXPECT_NE(dynamic_cast<PostgresMaintenanceRequestRepository*>(module.repo.get()), nullptr);
    unsetenv("REPOSITORY_IMPL");
}
