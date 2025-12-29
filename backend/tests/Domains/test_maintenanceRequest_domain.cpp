#include <gtest/gtest.h>

#include <memory>

#include "../../src/Domains/maintenanceRequest.h"

class MaintenanceRequestDomainTests : public ::testing::Test {
   public:
    const int defaultId = 1;
    const int defaultResidentId = 1;
    const int defaultApartmentId = 101;
    const std::string defaultDescription = "Leaky faucet in kitchen";
    const MaintenanceStatus defaultStatus = MaintenanceStatus::Open;
    const int defaultPriority = 3;

   protected:
    std::unique_ptr<MaintenanceRequest> defaultMaintenanceRequest;

    MaintenanceRequestDomainTests() {
        defaultMaintenanceRequest = std::make_unique<MaintenanceRequest>(
            defaultId, defaultResidentId, defaultApartmentId, defaultDescription, defaultStatus, defaultPriority);
    }
};

TEST_F(MaintenanceRequestDomainTests, MaintenanceRequestDomainConstructorAndValidateGetters) {
    EXPECT_EQ(defaultMaintenanceRequest->getId(), defaultId);
    EXPECT_EQ(defaultMaintenanceRequest->getResidentId(), defaultResidentId);
    EXPECT_EQ(defaultMaintenanceRequest->getApartmentId(), defaultApartmentId);
    EXPECT_EQ(defaultMaintenanceRequest->getDescription(), defaultDescription);
    EXPECT_EQ(defaultMaintenanceRequest->getStatus(), defaultStatus);
    EXPECT_EQ(defaultMaintenanceRequest->getPriority(), defaultPriority);
    EXPECT_EQ(defaultMaintenanceRequest->getCreatedAt(), defaultMaintenanceRequest->getUpdatedAt());
}

TEST_F(MaintenanceRequestDomainTests, UpdateMaintenanceRequestDomain) {
    const int newId = 2;
    const int newResidentId = 2;
    const int newApartmentId = 202;
    const std::string newDescription = "Broken window in living room";
    const MaintenanceStatus newStatus = MaintenanceStatus::InProgress;
    const int newPriority = 5;

    defaultMaintenanceRequest->updateMaintenanceRequestInfos(newId, newResidentId, newApartmentId, newDescription,
                                                             newStatus, newPriority);

    EXPECT_EQ(defaultMaintenanceRequest->getId(), newId);
    EXPECT_EQ(defaultMaintenanceRequest->getResidentId(), newResidentId);
    EXPECT_EQ(defaultMaintenanceRequest->getApartmentId(), newApartmentId);
    EXPECT_EQ(defaultMaintenanceRequest->getDescription(), newDescription);
    EXPECT_EQ(defaultMaintenanceRequest->getStatus(), newStatus);
    EXPECT_EQ(defaultMaintenanceRequest->getPriority(), newPriority);
}

TEST_F(MaintenanceRequestDomainTests, UpdateMaintenanceRequestDomainWithOptionalValuesUpdateDescriptionAndStatus) {
    const std::string newDescription = "Clogged drain in bathroom";
    const MaintenanceStatus newStatus = MaintenanceStatus::Completed;

    defaultMaintenanceRequest->updateMaintenanceRequestInfos(std::nullopt, std::nullopt, std::nullopt, newDescription,
                                                             newStatus, std::nullopt);

    EXPECT_EQ(defaultMaintenanceRequest->getId(), defaultId);
    EXPECT_EQ(defaultMaintenanceRequest->getResidentId(), defaultResidentId);
    EXPECT_EQ(defaultMaintenanceRequest->getApartmentId(), defaultApartmentId);
    EXPECT_EQ(defaultMaintenanceRequest->getDescription(), newDescription);
    EXPECT_EQ(defaultMaintenanceRequest->getStatus(), newStatus);
    EXPECT_EQ(defaultMaintenanceRequest->getPriority(), defaultPriority);
}

TEST_F(MaintenanceRequestDomainTests, UpdateMaintenanceRequestDomainWithOptionalValuesUpdatePriorityOnly) {
    const int newPriority = 1;

    defaultMaintenanceRequest->updateMaintenanceRequestInfos(std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                                             std::nullopt, newPriority);

    EXPECT_EQ(defaultMaintenanceRequest->getId(), defaultId);
    EXPECT_EQ(defaultMaintenanceRequest->getResidentId(), defaultResidentId);
    EXPECT_EQ(defaultMaintenanceRequest->getApartmentId(), defaultApartmentId);
    EXPECT_EQ(defaultMaintenanceRequest->getDescription(), defaultDescription);
    EXPECT_EQ(defaultMaintenanceRequest->getStatus(), defaultStatus);
    EXPECT_EQ(defaultMaintenanceRequest->getPriority(), newPriority);
}
