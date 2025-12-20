#include <gtest/gtest.h>

#include "../../../src/Repositories/inmemory/InMemoryApartmentRepository.h"

class InMemoryApartmentRepositoryTest : public ::testing::Test {
   protected:
    InMemoryApartmentRepository repository;

    Apartment apartment1;
    Apartment apartment2;
    Apartment apartment3;

    InMemoryApartmentRepositoryTest()
        : apartment1(0, 1, "101", false, true, 1500.0),
          apartment2(0, 1, "202", true, false, 1800.0),
          apartment3(0, 2, "303", false, true, 2000.0) {}

    void SetUp() override {}
};

TEST_F(InMemoryApartmentRepositoryTest, SaveAssignsUniqueIds) {
    auto id1 = repository.save(apartment1);
    auto id2 = repository.save(apartment2);
    auto id3 = repository.save(apartment3);

    EXPECT_NE(id1, id2);
    EXPECT_NE(id1, id3);
    EXPECT_NE(id2, id3);

    EXPECT_EQ(id1, 0);
    EXPECT_EQ(id2, 1);
    EXPECT_EQ(id3, 2);
}

TEST_F(InMemoryApartmentRepositoryTest, SaveStoresApartmentsCorrectly) {
    auto id1 = repository.save(apartment1);
    auto id2 = repository.save(apartment2);

    EXPECT_EQ(id1, 0);
    EXPECT_EQ(id2, 1);
}

TEST_F(InMemoryApartmentRepositoryTest, FindByIdReturnsNulloptForNonExistentId) {
    auto result = repository.findById(9999);  // Assuming 9999 is a non-existent ID
    EXPECT_FALSE(result.has_value());
    EXPECT_FALSE(result);
}

TEST_F(InMemoryApartmentRepositoryTest, FindByIdReturnsApartmentForExistingId) {
    auto id1 = repository.save(apartment1);
    auto id2 = repository.save(apartment2);
    auto id3 = repository.save(apartment3);
    auto result = repository.findById(id1);

    ASSERT_TRUE(result.has_value());
    auto foundApartment = result.value();

    auto apartments = repository.findAll();

    for (auto& apartment : apartments) {
        auto aptId = apartment.getId();
        std::cerr << "Looking for Apartment with ID: " << aptId << std::endl;
        auto foundApartmentOpt = repository.findById(aptId);

        EXPECT_TRUE(foundApartmentOpt.has_value());
        auto foundApartment = foundApartmentOpt.value();
        EXPECT_EQ(foundApartment.getId(), apartment.getId());
        EXPECT_EQ(foundApartment.getBuildingId(), apartment.getBuildingId());
        EXPECT_EQ(foundApartment.getNumber(), apartment.getNumber());
        EXPECT_EQ(foundApartment.getIsOccupied(), apartment.getIsOccupied());
        EXPECT_EQ(foundApartment.getPetAllowed(), apartment.getPetAllowed());
        EXPECT_EQ(foundApartment.getCondoFee(), apartment.getCondoFee());
        EXPECT_EQ(foundApartment.getCreatedAt(), apartment.getCreatedAt());
        EXPECT_EQ(foundApartment.getUpdatedAt(), apartment.getUpdatedAt());
    }
}

TEST_F(InMemoryApartmentRepositoryTest, FindAllReturnsAllApartments) {
    repository.save(apartment1);
    repository.save(apartment2);
    repository.save(apartment3);

    auto apartments = repository.findAll();
    ASSERT_EQ(apartments.size(), 3);

    // Check that all saved apartments are present
    bool foundApt1 = false;
    bool foundApt2 = false;
    bool foundApt3 = false;

    for (const auto& apt : apartments) {
        if (apt.getNumber() == apartment1.getNumber()) {
            foundApt1 = true;
        } else if (apt.getNumber() == apartment2.getNumber()) {
            foundApt2 = true;
        } else if (apt.getNumber() == apartment3.getNumber()) {
            foundApt3 = true;
        }
    }

    EXPECT_TRUE(foundApt1);
    EXPECT_TRUE(foundApt2);
    EXPECT_TRUE(foundApt3);
}

TEST_F(InMemoryApartmentRepositoryTest, FindAllReturnsEmptyVectorWhenNoRecords) {
    auto apartments = repository.findAll();
    EXPECT_TRUE(apartments.empty());
}

TEST_F(InMemoryApartmentRepositoryTest, UpdateDataFromExistingApartment) {
    auto id = repository.save(apartment1);
    auto result = repository.findById(id);
    ASSERT_TRUE(result.has_value());
    auto apartmentToUpdate = result.value();

    apartmentToUpdate.updateApartmentInfos(std::nullopt, std::nullopt, std::string("102"), true, std::nullopt, 1600.0);
    repository.update(apartmentToUpdate);

    auto updatedResult = repository.findById(id);
    ASSERT_TRUE(updatedResult.has_value());
    auto updatedApartment = updatedResult.value();

    EXPECT_EQ(updatedApartment.getNumber(), "102");
    EXPECT_EQ(updatedApartment.getIsOccupied(), true);
    EXPECT_EQ(updatedApartment.getCondoFee(), 1600.0);
}

TEST_F(InMemoryApartmentRepositoryTest, UpdateNonExistentApartmentDoesNothing) {
    auto id = repository.save(apartment1);
    auto result = repository.findById(id);
    ASSERT_TRUE(result.has_value());
    auto apartmentToUpdate = result.value();

    // Update with invalid ID
    constexpr int invalidId = 9999;
    apartmentToUpdate.updateApartmentInfos(invalidId, std::nullopt, std::string("102"), true, std::nullopt, 1600.0);
    repository.update(apartmentToUpdate);

    auto unchangedResult = repository.findById(id);
    ASSERT_TRUE(unchangedResult.has_value());
    auto unchangedApartment = unchangedResult.value();

    // Data should remain unchanged
    EXPECT_EQ(unchangedApartment.getNumber(), apartment1.getNumber());
    EXPECT_EQ(unchangedApartment.getIsOccupied(), apartment1.getIsOccupied());
    EXPECT_EQ(unchangedApartment.getCondoFee(), apartment1.getCondoFee());
}

TEST_F(InMemoryApartmentRepositoryTest, RemoveDeletesApartmentById) {
    auto id1 = repository.save(apartment1);
    auto id2 = repository.save(apartment2);

    repository.remove(id1);

    auto foundApartment1Opt = repository.findById(id1);
    auto foundApartment2Opt = repository.findById(id2);

    EXPECT_FALSE(foundApartment1Opt.has_value());
    EXPECT_TRUE(foundApartment2Opt.has_value());
}

TEST_F(InMemoryApartmentRepositoryTest, RemoveNonExistentIdDoesNothing) {
    auto id1 = repository.save(apartment1);
    auto id2 = repository.save(apartment2);

    constexpr int nonExistentId = 9999;
    repository.remove(nonExistentId);

    auto foundApartment1Opt = repository.findById(id1);
    auto foundApartment2Opt = repository.findById(id2);

    EXPECT_TRUE(foundApartment1Opt.has_value());
    EXPECT_TRUE(foundApartment2Opt.has_value());
}
