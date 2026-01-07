#include <gtest/gtest.h>

#include "../../src/Composition/Building_composition.h"
#include "../../src/Utils/Env.h"

TEST(BuildingCompositionTest, InMemoryRepositorySelection) {
    // Set the environment variable to use InMemory repository
    setenv("REPOSITORY_IMPL", "InMemory", 1);

    BuildingModule module("InMemory");

    // Check that the selected repository is of type InMemoryBuildingRepository
    EXPECT_NE(dynamic_cast<InMemoryBuildingRepository*>(module.repo.get()), nullptr);
    unsetenv("REPOSITORY_IMPL");
}

TEST(BuildingCompositionTest, PostgresRepositorySelection) {
    setenv("REPOSITORY_IMPL", "Postgres", 1);

    BuildingModule module("Postgres");

    // Check that the selected repository is of type PostgresBuildingRepository
    EXPECT_NE(dynamic_cast<PostgresBuildingRepository*>(module.repo.get()), nullptr);
    unsetenv("REPOSITORY_IMPL");
}