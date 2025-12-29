#include <gtest/gtest.h>

#include <memory>

#include "../../src/Domains/apartment.h"

class ApartmentDomainTests : public ::testing::Test {
   public:
    const int defaultId = 1;
    const int defaultBuildingId = 1;
    const std::string defaultNumber = "101";
    const bool defaultIsOccupied = false;
    const bool defaultPetAllowed = true;
    const double defaultCondoFee = 250.0;
    const double defaultAreaM2 = 50.0;

   protected:
    std::unique_ptr<Apartment> defaultApartment;

    ApartmentDomainTests() {
        defaultApartment = std::make_unique<Apartment>(defaultId, defaultBuildingId, defaultNumber, defaultIsOccupied,
                                                       defaultPetAllowed, defaultCondoFee, defaultAreaM2);
    }
};

TEST_F(ApartmentDomainTests, ApartmentDomainConstructor) { EXPECT_NE(defaultApartment, nullptr); }

TEST_F(ApartmentDomainTests, ValidateApartmentGetters) {
    EXPECT_EQ(defaultApartment->getId(), defaultId);
    EXPECT_EQ(defaultApartment->getBuildingId(), defaultBuildingId);
    EXPECT_EQ(defaultApartment->getNumber(), defaultNumber);
    EXPECT_EQ(defaultApartment->getIsOccupied(), defaultIsOccupied);
    EXPECT_EQ(defaultApartment->getPetAllowed(), defaultPetAllowed);
    EXPECT_EQ(defaultApartment->getCondoFee(), defaultCondoFee);
    EXPECT_EQ(defaultApartment->getCreatedAt(), defaultApartment->getUpdatedAt());
    EXPECT_EQ(defaultApartment->getAreaM2(), defaultAreaM2);
}

TEST_F(ApartmentDomainTests, UpdateApartmentDomain) {
    const int newId = 2;
    const int newBuildingId = 2;
    const std::string newNumber = "202";
    const bool newIsOccupied = true;
    const bool newPetAllowed = false;
    const double newCondoFee = 300.0;
    const double newAreaM2 = 60.0;

    defaultApartment->updateApartmentInfos(newId, newBuildingId, newNumber, newIsOccupied, newPetAllowed, newCondoFee,
                                           newAreaM2);

    EXPECT_EQ(defaultApartment->getId(), newId);
    EXPECT_EQ(defaultApartment->getBuildingId(), newBuildingId);
    EXPECT_EQ(defaultApartment->getNumber(), newNumber);
    EXPECT_EQ(defaultApartment->getIsOccupied(), newIsOccupied);
    EXPECT_EQ(defaultApartment->getPetAllowed(), newPetAllowed);
    EXPECT_EQ(defaultApartment->getCondoFee(), newCondoFee);
    EXPECT_EQ(defaultApartment->getAreaM2(), newAreaM2);
}

TEST_F(ApartmentDomainTests, UpdateApartmentDomainWithOptionalValuesUpdateNumberAndIsOccupied) {
    const std::string newNumber = "303";
    const bool newIsOccupied = true;

    defaultApartment->updateApartmentInfos(std::nullopt, std::nullopt, newNumber, newIsOccupied, std::nullopt,
                                           std::nullopt, std::nullopt);

    EXPECT_EQ(defaultApartment->getId(), defaultId);
    EXPECT_EQ(defaultApartment->getBuildingId(), defaultBuildingId);
    EXPECT_EQ(defaultApartment->getNumber(), newNumber);
    EXPECT_EQ(defaultApartment->getIsOccupied(), newIsOccupied);
    EXPECT_EQ(defaultApartment->getPetAllowed(), defaultPetAllowed);
    EXPECT_EQ(defaultApartment->getCondoFee(), defaultCondoFee);
    EXPECT_EQ(defaultApartment->getAreaM2(), defaultAreaM2);
}

TEST_F(ApartmentDomainTests, UpdateApartmentDomainWithOptionalValuesUpdateCondoFee) {
    const double newCondoFee = 350.0;

    defaultApartment->updateApartmentInfos(std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                           newCondoFee, std::nullopt);

    EXPECT_EQ(defaultApartment->getId(), defaultId);
    EXPECT_EQ(defaultApartment->getBuildingId(), defaultBuildingId);
    EXPECT_EQ(defaultApartment->getNumber(), defaultNumber);
    EXPECT_EQ(defaultApartment->getIsOccupied(), defaultIsOccupied);
    EXPECT_EQ(defaultApartment->getPetAllowed(), defaultPetAllowed);
    EXPECT_EQ(defaultApartment->getCondoFee(), newCondoFee);
    EXPECT_EQ(defaultApartment->getAreaM2(), defaultAreaM2);
}
