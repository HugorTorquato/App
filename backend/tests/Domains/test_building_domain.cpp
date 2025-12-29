#include <gtest/gtest.h>

#include <memory>

#include "../../Domains/apartment.h"
#include "../../src/Domains/building.h"

class BuildingDomainTests : public ::testing::Test {
   public:
    const int defaultId = 1;
    const std::string defaultName = "build1";
    const std::string defaultAddress = "adress1";
    const int defaultNumberOfFloors = 5;
    const std::vector<Apartment> defaultApartments = {};

   protected:
    // Initialize the pointer to null
    // Building* defaultBuild = nullptr;
    std::unique_ptr<Building> defaultBuild;

    BuildingDomainTests() {
        // defaultBuild = new Building(defaultId, defaultName, defaultAddress, defaultNumberOfFloors);
        defaultBuild = std::make_unique<Building>(defaultId, defaultName, defaultAddress, defaultNumberOfFloors,
                                                  defaultApartments);
    }

    // ~BuildingDomainTests() { delete defaultBuild; }
};

TEST_F(BuildingDomainTests, BuildDomainConstructorAndValidateGetters) {
    EXPECT_EQ(defaultBuild->getId(), defaultId);
    EXPECT_EQ(defaultBuild->getName(), defaultName);
    EXPECT_EQ(defaultBuild->getAddress(), defaultAddress);
    EXPECT_EQ(defaultBuild->getNumberOfFloors(), defaultNumberOfFloors);
    EXPECT_EQ(defaultBuild->getCreatedAt(), defaultBuild->getUpdatedAt());
    EXPECT_EQ(defaultBuild->getApartments().size(), defaultApartments.size());
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
    EXPECT_EQ(defaultBuild->getApartments().size(), defaultApartments.size());

    EXPECT_NE(defaultBuild->getCreatedAt(), defaultBuild->getUpdatedAt());
}

TEST_F(BuildingDomainTests, UpdateBuildDomainWithOptionalValuesUpdateBuildName) {
    const std::string newName = "build2";

    defaultBuild->updateBuildInfos(std::nullopt, newName, std::nullopt, std::nullopt);

    EXPECT_EQ(defaultBuild->getId(), defaultId);
    EXPECT_EQ(defaultBuild->getName(), newName);
    EXPECT_EQ(defaultBuild->getAddress(), defaultAddress);
    EXPECT_EQ(defaultBuild->getNumberOfFloors(), defaultNumberOfFloors);
    EXPECT_EQ(defaultBuild->getApartments().size(), defaultApartments.size());
}

TEST_F(BuildingDomainTests, UpdateBuildDomainWithOptionalValuesUpdateBuildAddress) {
    const std::string newAddress = "address2";
    defaultBuild->updateBuildInfos(std::nullopt, std::nullopt, newAddress, std::nullopt);

    EXPECT_EQ(defaultBuild->getId(), defaultId);
    EXPECT_EQ(defaultBuild->getName(), defaultName);
    EXPECT_EQ(defaultBuild->getAddress(), newAddress);
    EXPECT_EQ(defaultBuild->getNumberOfFloors(), defaultNumberOfFloors);
    EXPECT_EQ(defaultBuild->getApartments().size(), defaultApartments.size());
}

TEST_F(BuildingDomainTests, UpdateBuildDomainWithOptionalValuesUpdateNumberOfFloors) {
    const int newNumberOfFloors = 15;
    defaultBuild->updateBuildInfos(std::nullopt, std::nullopt, std::nullopt, newNumberOfFloors);

    EXPECT_EQ(defaultBuild->getId(), defaultId);
    EXPECT_EQ(defaultBuild->getName(), defaultName);
    EXPECT_EQ(defaultBuild->getAddress(), defaultAddress);
    EXPECT_EQ(defaultBuild->getNumberOfFloors(), newNumberOfFloors);
    EXPECT_EQ(defaultBuild->getApartments().size(), defaultApartments.size());
}

TEST_F(BuildingDomainTests, UpdateBuildDomainWithOptionalValuesUpdateApartments) {
    std::vector<Apartment> newApartments = {Apartment(1, defaultId, "101", false, true, 200.0, 50.0),
                                            Apartment(2, defaultId, "102", true, false, 250.0, 60.0)};

    defaultBuild->updateBuildInfos(std::nullopt, std::nullopt, std::nullopt, std::nullopt, newApartments);

    EXPECT_EQ(defaultBuild->getId(), defaultId);
    EXPECT_EQ(defaultBuild->getName(), defaultName);
    EXPECT_EQ(defaultBuild->getAddress(), defaultAddress);
    EXPECT_EQ(defaultBuild->getNumberOfFloors(), defaultNumberOfFloors);
    EXPECT_EQ(defaultBuild->getApartments().size(), newApartments.size());
}

TEST_F(BuildingDomainTests, UpdateBuildDomainWithOptionalValuesUpdateAllFields) {
    std::vector<Apartment> newApartments = {Apartment(3, defaultId, "201", false, true, 300.0, 70.0),
                                            Apartment(4, defaultId, "202", true, false, 350.0, 80.0)};

    const int newId = 3;
    const std::string newName = "build3";
    const std::string newAddress = "adress3";
    const int newNumberOfFloors = 20;

    defaultBuild->updateBuildInfos(newId, newName, newAddress, newNumberOfFloors, newApartments);

    EXPECT_EQ(defaultBuild->getId(), newId);
    EXPECT_EQ(defaultBuild->getName(), newName);
    EXPECT_EQ(defaultBuild->getAddress(), newAddress);
    EXPECT_EQ(defaultBuild->getNumberOfFloors(), newNumberOfFloors);
    EXPECT_EQ(defaultBuild->getApartments().size(), newApartments.size());
}
