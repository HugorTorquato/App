#include <gtest/gtest.h>

#include "../../../src/Repositories/inmemory/InMemoryBuildingRepository.h"

class InMemoryBuildingRepositoryTest : public ::testing::Test {
   protected:
    InMemoryBuildingRepository repository;

    Building building1;
    Building building2;
    Building building3;

    InMemoryBuildingRepositoryTest()
        : building1(0, "Building A", "123 Main St", 10),
          building2(0, "Building B", "456 Elm St", 15),
          building3(0, "Building C", "789 Oak St", 20) {}

    void SetUp() override {}
};

TEST_F(InMemoryBuildingRepositoryTest, SaveAssignsUniqueIds) {
    auto id1 = repository.save(building1);
    auto id2 = repository.save(building2);
    auto id3 = repository.save(building3);

    EXPECT_NE(id1, id2);
    EXPECT_NE(id1, id3);
    EXPECT_NE(id2, id3);

    EXPECT_EQ(id1, 0);
    EXPECT_EQ(id2, 1);
    EXPECT_EQ(id3, 2);
}

TEST_F(InMemoryBuildingRepositoryTest, FindByIdReturnsNulloptForNonExistentId) {
    auto result = repository.findById(9999);  // Assuming 9999 is a non-existent ID
    EXPECT_FALSE(result.has_value());
    EXPECT_FALSE(result);
}

TEST_F(InMemoryBuildingRepositoryTest, FindByIdReturnsBuildingForExistingId) {
    auto id1 = repository.save(building1);
    auto id2 = repository.save(building2);
    auto id3 = repository.save(building3);

    auto result = repository.findById(id1);

    ASSERT_TRUE(result.has_value());
    auto foundBuilding = result.value();

    EXPECT_EQ(foundBuilding.getId(), id1);
    EXPECT_EQ(foundBuilding.getName(), building1.getName());
    EXPECT_EQ(foundBuilding.getAddress(), building1.getAddress());
    EXPECT_EQ(foundBuilding.getNumberOfFloors(), building1.getNumberOfFloors());
}

TEST_F(InMemoryBuildingRepositoryTest, FindAllReturnsAllBuildings) {
    repository.save(building1);
    repository.save(building2);
    repository.save(building3);

    auto buildings = repository.findAll();

    EXPECT_EQ(buildings.size(), 3);

    EXPECT_EQ(buildings[0].getName(), building1.getName());
    EXPECT_EQ(buildings[1].getName(), building2.getName());
    EXPECT_EQ(buildings[2].getName(), building3.getName());
}

TEST_F(InMemoryBuildingRepositoryTest, FindAllReturnsEmptyVectorWhenNoBuildings) {
    auto buildings = repository.findAll();
    EXPECT_TRUE(buildings.empty());
}

TEST_F(InMemoryBuildingRepositoryTest, FindAllAfterSavingAndRetrievingBuildings) {
    auto id1 = repository.save(building1);
    auto id2 = repository.save(building2);

    auto retrievedBuilding1 = repository.findById(id1);
    auto retrievedBuilding2 = repository.findById(id2);

    ASSERT_TRUE(retrievedBuilding1.has_value());
    ASSERT_TRUE(retrievedBuilding2.has_value());

    auto buildings = repository.findAll();

    EXPECT_EQ(buildings.size(), 2);
    EXPECT_EQ(buildings[0].getId(), retrievedBuilding1->getId());
    EXPECT_EQ(buildings[1].getId(), retrievedBuilding2->getId());
}

TEST_F(InMemoryBuildingRepositoryTest, UpdateDataFromExistingBuilding) {
    auto id = repository.save(building1);
    auto result = repository.findById(id);
    ASSERT_TRUE(result.has_value());
    auto buildingToUpdate = result.value();

    buildingToUpdate.updateBuildInfos(std::nullopt, std::string("Updated Building A"), std::nullopt, 12);
    repository.update(buildingToUpdate);

    auto updatedResult = repository.findById(id);
    ASSERT_TRUE(updatedResult.has_value());
    auto updatedBuilding = updatedResult.value();

    EXPECT_EQ(updatedBuilding.getName(), "Updated Building A");
    EXPECT_EQ(updatedBuilding.getNumberOfFloors(), 12);
}

TEST_F(InMemoryBuildingRepositoryTest, UpdateNonExistentBuildingDoesNothing) {
    auto id = repository.save(building1);
    auto result = repository.findById(id);
    ASSERT_TRUE(result.has_value());
    auto buildingToUpdate = result.value();

    buildingToUpdate.updateBuildInfos(std::nullopt, std::string("Updated Building A"), std::nullopt, 12);
    repository.update(buildingToUpdate);

    // Attempt to update a non-existent building
    Building nonExistentBuilding(9999, "Non-Existent", "No Address", 0);
    repository.update(nonExistentBuilding);

    auto updatedResult = repository.findById(id);
    ASSERT_TRUE(updatedResult.has_value());
    auto updatedBuilding = updatedResult.value();

    // Ensure the original building remains unchanged
    EXPECT_EQ(updatedBuilding.getName(), "Updated Building A");
    EXPECT_EQ(updatedBuilding.getNumberOfFloors(), 12);
}

TEST_F(InMemoryBuildingRepositoryTest, RemoveDeletesBuildingById) {
    auto id1 = repository.save(building1);
    auto id2 = repository.save(building2);

    repository.remove(id1);

    auto foundBuilding1Opt = repository.findById(id1);
    auto foundBuilding2Opt = repository.findById(id2);

    EXPECT_FALSE(foundBuilding1Opt.has_value());
    EXPECT_TRUE(foundBuilding2Opt.has_value());
}

TEST_F(InMemoryBuildingRepositoryTest, RemoveNonExistentIdDoesNothing) {
    auto id1 = repository.save(building1);
    auto id2 = repository.save(building2);

    repository.remove(9999);  // Assuming 9999 is a non-existent ID

    auto foundBuilding1Opt = repository.findById(id1);
    auto foundBuilding2Opt = repository.findById(id2);

    EXPECT_TRUE(foundBuilding1Opt.has_value());
    EXPECT_TRUE(foundBuilding2Opt.has_value());
}
