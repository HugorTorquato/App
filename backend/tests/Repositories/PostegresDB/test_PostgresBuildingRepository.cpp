#include <gtest/gtest.h>

#include "../../../src/Repositories/postgresDB/PostgresBuildingRepository.h"
#include "../../Mocks/MockDbConnectionFactory.h"

class PostgresSDBBuildingRepositoryTest : public ::testing::Test {
   protected:
    PostgresBuildingRepository* repository = nullptr;

    std::optional<Building> building1;
    std::optional<Building> building2;
    std::optional<Building> building3;

    void SetUp() override {
        repository = new PostgresBuildingRepository(std::move(std::unique_ptr<MockDbConnectionFactory>()));
        building1.emplace(0, "Building A", "123 Main St", 10);
        building2.emplace(0, "Building B", "456 Elm St", 15);
        building3.emplace(0, "Building C", "789 Oak St", 20);
    }

    void TearDown() override {
        delete repository;
        repository = nullptr;
    }
};

TEST_F(PostgresSDBBuildingRepositoryTest, SaveAssignsUniqueIds) {
    auto id1 = repository->save(*building1);
    auto id2 = repository->save(*building2);
    auto id3 = repository->save(*building3);

    EXPECT_NE(id1, id2);
    EXPECT_NE(id1, id3);
    EXPECT_NE(id2, id3);

    EXPECT_EQ(id1, 0);
    EXPECT_EQ(id2, 1);
    EXPECT_EQ(id3, 2);
}