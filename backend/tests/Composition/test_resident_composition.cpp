#include <gtest/gtest.h>

#include "../../src/Composition/Resident_composition.h"
#include "../../src/Utils/Env.h"

TEST(ResidentCompositionTest, InMemoryRepositorySelection) {
    // Set the environment variable to use InMemory repository
    setenv("REPOSITORY_IMPL", "InMemory", 1);

    ResidentModule module("InMemory");

    // Check that the selected repository is of type InMemoryResidentRepository
    EXPECT_NE(dynamic_cast<InMemoryResidentRepository*>(module.repo.get()), nullptr);
    unsetenv("REPOSITORY_IMPL");
}

TEST(ResidentCompositionTest, PostgresRepositorySelection) {
    setenv("REPOSITORY_IMPL", "Postgres", 1);

    ResidentModule module("Postgres");

    // Check that the selected repository is of type PostgresResidentRepository
    EXPECT_NE(dynamic_cast<PostgresResidentRepository*>(module.repo.get()), nullptr);
    unsetenv("REPOSITORY_IMPL");
}