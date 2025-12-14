#include <gtest/gtest.h>

#include <memory>

#include "../../src/Domains/resident.h"

class ResidentDomainTests : public ::testing::Test {
   public:
    const int defaultId = 1;
    const std::string defaultFullName = "John Doe";
    const std::string defaultDocumentNumber = "123456789";
    const std::string defaultApartmentId = "101";
    const bool defaultIsOwner = true;
    const std::string defaultPhone = "555-1234";
    const time_t defaultMoveInDate = 1622505600;  // June 1, 2021
    const time_t defaultMoveOutDate = 0;          // No move-out date

   protected:
    std::unique_ptr<Resident> defaultResident;

    ResidentDomainTests() {
        defaultResident =
            std::make_unique<Resident>(defaultId, defaultFullName, defaultDocumentNumber, defaultApartmentId,
                                       defaultIsOwner, defaultPhone, defaultMoveInDate, defaultMoveOutDate);
    }
};

TEST_F(ResidentDomainTests, ResidentDomainConstructorAndValidateGetters) {
    EXPECT_EQ(defaultResident->getId(), defaultId);
    EXPECT_EQ(defaultResident->getFullName(), defaultFullName);
    EXPECT_EQ(defaultResident->getDocumentNumber(), defaultDocumentNumber);
    EXPECT_EQ(defaultResident->getApartmentId(), defaultApartmentId);
    EXPECT_EQ(defaultResident->getIsOwner(), defaultIsOwner);
    EXPECT_EQ(defaultResident->getPhone(), defaultPhone);
    EXPECT_EQ(defaultResident->getMoveInDate(), defaultMoveInDate);
    EXPECT_EQ(defaultResident->getMoveOutDate(), defaultMoveOutDate);
    EXPECT_EQ(defaultResident->getCreatedAt(), defaultResident->getUpdatedAt());
}

TEST_F(ResidentDomainTests, UseResidentConstructorWithoutID) {
    std::unique_ptr<Resident> residentWithoutID;

    constexpr int noID = 0;

    residentWithoutID = std::make_unique<Resident>(defaultFullName, defaultDocumentNumber, defaultApartmentId,
                                                   defaultIsOwner, defaultPhone, defaultMoveInDate, defaultMoveOutDate);

    EXPECT_EQ(residentWithoutID->getId(), noID);
    EXPECT_EQ(residentWithoutID->getFullName(), defaultFullName);
    EXPECT_EQ(residentWithoutID->getDocumentNumber(), defaultDocumentNumber);
    EXPECT_EQ(residentWithoutID->getApartmentId(), defaultApartmentId);
    EXPECT_EQ(residentWithoutID->getIsOwner(), defaultIsOwner);
    EXPECT_EQ(residentWithoutID->getPhone(), defaultPhone);
    EXPECT_EQ(residentWithoutID->getMoveInDate(), defaultMoveInDate);
    EXPECT_EQ(residentWithoutID->getMoveOutDate(), defaultMoveOutDate);
    EXPECT_EQ(residentWithoutID->getCreatedAt(), residentWithoutID->getUpdatedAt());
}

TEST_F(ResidentDomainTests, UpdateResidentDomain) {
    const int newId = 2;
    const std::string newFullName = "Jane Smith";
    const std::string newDocumentNumber = "987654321";
    const std::string& newApartmentId = "202";
    const bool newIsOwner = false;
    const std::string newPhone = "555-5678";
    const time_t newMoveInDate = 1625097600;   // July 1, 2021
    const time_t newMoveOutDate = 1656633600;  // July 1, 2022

    defaultResident->updateResidentInfos(newId, newFullName, newDocumentNumber, newApartmentId, newIsOwner, newPhone,
                                         newMoveInDate, newMoveOutDate);

    EXPECT_EQ(defaultResident->getId(), newId);
    EXPECT_EQ(defaultResident->getFullName(), newFullName);
    EXPECT_EQ(defaultResident->getDocumentNumber(), newDocumentNumber);
    EXPECT_EQ(defaultResident->getApartmentId(), newApartmentId);
    EXPECT_EQ(defaultResident->getIsOwner(), newIsOwner);
    EXPECT_EQ(defaultResident->getPhone(), newPhone);
    EXPECT_EQ(defaultResident->getMoveInDate(), newMoveInDate);
    EXPECT_EQ(defaultResident->getMoveOutDate(), newMoveOutDate);
}

TEST_F(ResidentDomainTests, UpdateResidentDomainWithOptionalValuesUpdateFullNameAndPhone) {
    const std::string newFullName = "Alice Johnson";
    const std::string newPhone = "555-9012";

    defaultResident->updateResidentInfos(std::nullopt, newFullName, std::nullopt, std::nullopt, std::nullopt, newPhone,
                                         std::nullopt, std::nullopt);

    EXPECT_EQ(defaultResident->getId(), defaultId);
    EXPECT_EQ(defaultResident->getFullName(), newFullName);
    EXPECT_EQ(defaultResident->getDocumentNumber(), defaultDocumentNumber);
    EXPECT_EQ(defaultResident->getApartmentId(), defaultApartmentId);
    EXPECT_EQ(defaultResident->getIsOwner(), defaultIsOwner);
    EXPECT_EQ(defaultResident->getPhone(), newPhone);
    EXPECT_EQ(defaultResident->getMoveInDate(), defaultMoveInDate);
    EXPECT_EQ(defaultResident->getMoveOutDate(), defaultMoveOutDate);
}
