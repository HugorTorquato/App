#include <gtest/gtest.h>

#include <ctime>
#include <memory>
#include <string>

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

TEST_F(BuildingDomainTests, BuildDomainUpdateUpdatedAt) {
    // Sleep for 1 second to ensure a time difference
    std::this_thread::sleep_for(std::chrono::seconds(1));

    defaultBuild->updateUpdatedAt();

    EXPECT_NE(defaultBuild->getCreatedAt(), defaultBuild->getUpdatedAt());
}