#include <gtest/gtest.h>

#include "../../../src/Repositories/inmemory/InMemoryResidentRepository.h"

// ✅ The correct mental model for repository tests

// ❌ Wrong model

// “Repository is global / singleton / static”

// ✅ Correct model

// “Each test owns its own repository instance”

// Why?

// Tests must be isolated

// No shared state

// Deterministic behavior

// Your in-memory repository is NOT a global object.
// It is just a normal class with state.

class InMemoryResidentRepositoryTest : public ::testing::Test {
   protected:
    InMemoryResidentRepository repository;

    Resident resident1;
    Resident resident2;
    Resident resident3;

    InMemoryResidentRepositoryTest()
        : resident1("Hugo Torquato", "12345678910", "303", true, "555-0101", 1622505600, 0),
          resident2("Jane Smith", "12345678911", "202", false, "555-0102", 1622505600, 0),
          resident3("Alice Johnson", "12345678912", "101", true, "555-0103", 1622505600, 0) {}

    void SetUp() override {}
};

TEST_F(InMemoryResidentRepositoryTest, InMemoryStorageIsEmpty) {
    // The idea is to not initialize the storage for the tests
    EXPECT_TRUE(repository.findAll().empty());
}

TEST_F(InMemoryResidentRepositoryTest, AddOneResidentToTheStorage) {
    Resident resident1("Hugo Torquato", "12345678910", "303", true, "555-0101", 1622505600, 0);
    repository.save(resident1);
    EXPECT_EQ(repository.findAll().size(), 1);
}

TEST_F(InMemoryResidentRepositoryTest, AddMultipleResidentsToTheStorage) {
    repository.save(resident1);
    repository.save(resident2);
    repository.save(resident3);

    auto residents = repository.findAll();

    EXPECT_EQ(residents.size(), 3);

    const auto& expecteNames = std::vector<std::string>{"Hugo Torquato", "Jane Smith", "Alice Johnson"};

    for (const auto& resident : residents) {
        std::cout << "Resident Name: " << resident.getFullName() << std::endl;
        EXPECT_EQ(resident.getFullName(), expecteNames[resident.getId() - 1]);
    }
}
