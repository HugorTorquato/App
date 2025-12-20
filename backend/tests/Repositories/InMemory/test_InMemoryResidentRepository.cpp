#include <gtest/gtest.h>

#include <iostream>

#include "../../../src/Repositories/inmemory/InMemoryResidentRepository.h"

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
        // std::cout << "Resident Name: " << resident.getFullName() << std::endl;
        EXPECT_EQ(resident.getFullName(), expecteNames[resident.getId()]);
    }
}

TEST_F(InMemoryResidentRepositoryTest, UpdateRecordInMemoryContainer) {
    auto res1Id = repository.save(resident1);

    auto originalResident = repository.findAll();
    ASSERT_FALSE(originalResident.empty());

    auto res1 = originalResident.at(res1Id);
    EXPECT_EQ(res1.getFullName(), "Hugo Torquato");

    res1.updateResidentInfos(res1Id, "Hugo Rodrigues Torquato", std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                             std::nullopt, std::nullopt);

    repository.update(res1);

    auto updatedResidents = repository.findAll().at(res1Id);

    EXPECT_EQ(updatedResidents.getFullName(), "Hugo Rodrigues Torquato");
}

TEST_F(InMemoryResidentRepositoryTest, UpdateRecordInMemoryContainerWithInvalidID) {
    auto res1Id = repository.save(resident1);

    auto originalResident = repository.findAll();
    ASSERT_FALSE(originalResident.empty());

    auto res1 = originalResident.at(res1Id);
    EXPECT_EQ(res1.getFullName(), "Hugo Torquato");

    // Update with invalid ID
    constexpr int invalidId = 9999;
    res1.updateResidentInfos(invalidId, "Hugo Rodrigues Torquato", std::nullopt, std::nullopt, std::nullopt,
                             std::nullopt, std::nullopt, std::nullopt);

    repository.update(res1);

    auto updatedResidents = repository.findAll().at(res1Id);

    // Name should remain unchanged
    EXPECT_EQ(updatedResidents.getFullName(), "Hugo Torquato");
}

TEST_F(InMemoryResidentRepositoryTest, FindAllReturnsEmptyVectorWhenNoRecords) {
    auto residents = repository.findAll();
    EXPECT_TRUE(residents.empty());
}

TEST_F(InMemoryResidentRepositoryTest, FindAllReturnsSpecificResidentById) {
    repository.save(resident1);
    repository.save(resident2);
    repository.save(resident3);

    auto residents = repository.findAll();
    ASSERT_EQ(residents.size(), 3);

    for (auto& resident : residents) {
        auto resId = resident.getId();
        // std::cerr << "Looking for Resident with ID: " << resId << std::endl;
        auto foundResidentOpt = repository.findById(resId);

        EXPECT_TRUE(foundResidentOpt.has_value());
        auto foundResident = foundResidentOpt.value();
        EXPECT_EQ(foundResident.getFullName(), resident.getFullName());
        EXPECT_EQ(foundResident.getDocumentNumber(), resident.getDocumentNumber());
        EXPECT_EQ(foundResident.getApartmentId(), resident.getApartmentId());
        EXPECT_EQ(foundResident.getIsOwner(), resident.getIsOwner());
        EXPECT_EQ(foundResident.getPhone(), resident.getPhone());
        EXPECT_EQ(foundResident.getMoveInDate(), resident.getMoveInDate());
        EXPECT_EQ(foundResident.getMoveOutDate(), resident.getMoveOutDate());
    }
}

TEST_F(InMemoryResidentRepositoryTest, FindByIdReturnsNulloptForNonExistentId) {
    repository.save(resident1);
    repository.save(resident2);

    constexpr int nonExistentId = 9999;
    auto foundResidentOpt = repository.findById(nonExistentId);

    EXPECT_FALSE(foundResidentOpt.has_value());
    EXPECT_FALSE(foundResidentOpt);
}

TEST_F(InMemoryResidentRepositoryTest, SaveAssignsUniqueIds) {
    auto id1 = repository.save(resident1);
    auto id2 = repository.save(resident2);
    auto id3 = repository.save(resident3);

    EXPECT_NE(id1, id2);
    EXPECT_NE(id1, id3);
    EXPECT_NE(id2, id3);

    EXPECT_EQ(id1, 0);
    EXPECT_EQ(id2, 1);
    EXPECT_EQ(id3, 2);
}

TEST_F(InMemoryResidentRepositoryTest, SaveStoresResidentsCorrectly) {
    auto id1 = repository.save(resident1);
    auto id2 = repository.save(resident2);

    auto foundResident1Opt = repository.findById(id1);
    auto foundResident2Opt = repository.findById(id2);

    ASSERT_TRUE(foundResident1Opt.has_value());
    ASSERT_TRUE(foundResident2Opt.has_value());

    auto foundResident1 = foundResident1Opt.value();
    auto foundResident2 = foundResident2Opt.value();

    EXPECT_EQ(foundResident1.getFullName(), resident1.getFullName());
    EXPECT_EQ(foundResident2.getFullName(), resident2.getFullName());
}

TEST_F(InMemoryResidentRepositoryTest, RemoveDeletesResidentById) {
    auto id1 = repository.save(resident1);
    auto id2 = repository.save(resident2);

    repository.remove(id1);

    auto foundResident1Opt = repository.findById(id1);
    auto foundResident2Opt = repository.findById(id2);

    EXPECT_FALSE(foundResident1Opt.has_value());
    EXPECT_TRUE(foundResident2Opt.has_value());
}

TEST_F(InMemoryResidentRepositoryTest, RemoveNonExistentIdDoesNothing) {
    auto id1 = repository.save(resident1);
    auto id2 = repository.save(resident2);

    constexpr int nonExistentId = 9999;
    repository.remove(nonExistentId);

    auto foundResident1Opt = repository.findById(id1);
    auto foundResident2Opt = repository.findById(id2);

    EXPECT_TRUE(foundResident1Opt.has_value());
    EXPECT_TRUE(foundResident2Opt.has_value());
}
