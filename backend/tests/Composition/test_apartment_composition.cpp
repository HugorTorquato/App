#include <gtest/gtest.h>

#include "../../src/Composition/Apartment_composition.h"
#include "../../src/Utils/Env.h"

TEST(ApartmentCompositionTest, InMemoryRepositorySelection) {
    // Set the environment variable to use InMemory repository
    setenv("REPOSITORY_IMPL", "InMemory", 1);

    ApartmentModule module("InMemory");

    // Check that the selected repository is of type InMemoryApartmentRepository
    EXPECT_NE(dynamic_cast<InMemoryApartmentRepository*>(module.repo.get()), nullptr);
    unsetenv("REPOSITORY_IMPL");
}

TEST(ApartmentCompositionTest, PostgresRepositorySelection) {
    setenv("REPOSITORY_IMPL", "Postgres", 1);

    ApartmentModule module("Postgres");

    // Check that the selected repository is of type PostgresApartmentRepository
    EXPECT_NE(dynamic_cast<PostgresApartmentRepository*>(module.repo.get()), nullptr);
    unsetenv("REPOSITORY_IMPL");
}