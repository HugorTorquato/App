#include <gtest/gtest.h>

#include "../../../src/Repositories/inmemory/InMemoryResidentRepository.h"
#include "../../../src/Services/Resident/ResidentService.h"

class ServiceResidentTest : public ::testing::Test {
   protected:
    InMemoryResidentRepository repository;
    ResidentService service;

    Resident resident1;
    Resident resident2;

    ServiceResidentTest()
        : service(repository),
          resident1("Hugo Torquato", "12345678910", "303", true, "555-0101", 1622505600, 0),
          resident2("Jane Smith", "12345678911", "202", false, "555-0102", 1622505600, 0) {}

    void SetUp() override {}
};

TEST_F(ServiceResidentTest, CreateResidentStoresResidentInRepository) {
    auto id1 = service.createResident(resident1);
    auto id2 = service.createResident(resident2);

    EXPECT_EQ(id1, 0);
    EXPECT_EQ(id2, 1);

    auto allResidents = repository.findAll();
    EXPECT_EQ(allResidents.size(), 2);

    EXPECT_EQ(allResidents[0].getFullName(), "Hugo Torquato");
    EXPECT_EQ(allResidents[1].getFullName(), "Jane Smith");
}

TEST_F(ServiceResidentTest, GetResidentByIdReturnsCorrectResident) {
    auto id1 = service.createResident(resident1);
    auto id2 = service.createResident(resident2);

    auto fetchedResident1Opt = service.getResidentById(id1);
    ASSERT_TRUE(fetchedResident1Opt.has_value());
    auto fetchedResident1 = fetchedResident1Opt.value();
    EXPECT_EQ(fetchedResident1.getFullName(), "Hugo Torquato");

    auto fetchedResident2Opt = service.getResidentById(id2);
    ASSERT_TRUE(fetchedResident2Opt.has_value());
    auto fetchedResident2 = fetchedResident2Opt.value();
    EXPECT_EQ(fetchedResident2.getFullName(), "Jane Smith");
}

TEST_F(ServiceResidentTest, GetResidentByIdReturnsNulloptForNonExistentId) {
    service.createResident(resident1);
    service.createResident(resident2);

    constexpr int nonExistentId = 9999;
    auto fetchedResidentOpt = service.getResidentById(nonExistentId);

    EXPECT_FALSE(fetchedResidentOpt.has_value());
    EXPECT_FALSE(fetchedResidentOpt);
}

TEST_F(ServiceResidentTest, MoveOutResidentUpdatesMoveOutDate) {
    auto id1 = service.createResident(resident1);

    constexpr time_t newMoveOutDate = 1672531199;  // Example timestamp
    service.moveOutResident(id1, newMoveOutDate);

    auto fetchedResidentOpt = service.getResidentById(id1);
    ASSERT_TRUE(fetchedResidentOpt.has_value());
    auto fetchedResident = fetchedResidentOpt.value();

    EXPECT_EQ(fetchedResident.getMoveOutDate(), newMoveOutDate);
}

TEST_F(ServiceResidentTest, MoveOutResidentNonExistentIdDoesNothing) {
    auto id1 = service.createResident(resident1);

    constexpr int nonExistentId = 9999;
    constexpr time_t newMoveOutDate = 1672531199;  // Example timestamp
    service.moveOutResident(nonExistentId, newMoveOutDate);

    // Ensure that the existing resident's move-out date remains unchanged
    auto fetchedResidentOpt = service.getResidentById(id1);
    ASSERT_TRUE(fetchedResidentOpt.has_value());
    auto fetchedResident = fetchedResidentOpt.value();

    EXPECT_EQ(fetchedResident.getMoveOutDate(), 0);  // Original move-out date
}