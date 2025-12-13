#include <gtest/gtest.h>

#include <memory>

#include "../../src/Domains/building.h"

class BuildingDomainTests : public ::testing::Test {
   public:
    const int defaultId = 1;
    const std::string defaultName = "build1";
    const std::string defaultAddress = "adress1";
    const int defaultNumberOfFloors = 5;

   protected:
    // Initialize the pointer to null
    // Building* defaultBuild = nullptr;
    std::unique_ptr<Building> defaultBuild;

    BuildingDomainTests() {
        // defaultBuild = new Building(defaultId, defaultName, defaultAddress, defaultNumberOfFloors);
        defaultBuild = std::make_unique<Building>(defaultId, defaultName, defaultAddress, defaultNumberOfFloors);
    }

    // ~BuildingDomainTests() { delete defaultBuild; }
};

TEST_F(BuildingDomainTests, BuildDomainConstructorAndValidateGetters) {
    EXPECT_EQ(defaultBuild->getId(), defaultId);
    EXPECT_EQ(defaultBuild->getName(), defaultName);
    EXPECT_EQ(defaultBuild->getAddress(), defaultAddress);
    EXPECT_EQ(defaultBuild->getNumberOfFloors(), defaultNumberOfFloors);
    EXPECT_EQ(defaultBuild->getCreatedAt(), defaultBuild->getUpdatedAt());
}

TEST_F(BuildingDomainTests, UpdateBuildDomain) {
    // Sleep for 1 second to ensure a time difference
    std::this_thread::sleep_for(std::chrono::seconds(1));

    const int newId = 2;
    const std::string newName = "build2";
    const std::string newAddress = "adress2";
    const int newNumberOfFloors = 10;

    defaultBuild->updateBuildInfos(newId, newName, newAddress, newNumberOfFloors);

    EXPECT_EQ(defaultBuild->getId(), newId);
    EXPECT_EQ(defaultBuild->getName(), newName);
    EXPECT_EQ(defaultBuild->getAddress(), newAddress);
    EXPECT_EQ(defaultBuild->getNumberOfFloors(), newNumberOfFloors);

    EXPECT_NE(defaultBuild->getCreatedAt(), defaultBuild->getUpdatedAt());
}

TEST_F(BuildingDomainTests, UpdateBuildDomainWithOptionalValuesUpdateBuildName) {
    const std::string newName = "build2";

    defaultBuild->updateBuildInfos(std::nullopt, newName, std::nullopt, std::nullopt);

    EXPECT_EQ(defaultBuild->getId(), defaultId);
    EXPECT_EQ(defaultBuild->getName(), newName);
    EXPECT_EQ(defaultBuild->getAddress(), defaultAddress);
    EXPECT_EQ(defaultBuild->getNumberOfFloors(), defaultNumberOfFloors);
}

TEST_F(BuildingDomainTests, UpdateBuildDomainWithOptionalValuesUpdateBuildAddress) {
    const std::string newAddress = "address2";
    defaultBuild->updateBuildInfos(std::nullopt, std::nullopt, newAddress, std::nullopt);

    EXPECT_EQ(defaultBuild->getId(), defaultId);
    EXPECT_EQ(defaultBuild->getName(), defaultName);
    EXPECT_EQ(defaultBuild->getAddress(), newAddress);
    EXPECT_EQ(defaultBuild->getNumberOfFloors(), defaultNumberOfFloors);
}
