
#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "../../src/DTOs/maintenanceRequest_dto.h"

class MaintenanceRequestDTOTest : public ::testing::Test {
   protected:
    MaintenanceRequestDTO R1;

    MaintenanceRequestDTOTest() {
        R1.id = 301;
        R1.resident_id = 401;
        R1.apartment_id = 501;
        R1.description = "Leaky faucet in kitchen";
        R1.status = MaintenanceStatus::Open;
        R1.priority = 3;
    }
};

TEST_F(MaintenanceRequestDTOTest, ValidateDataFromMaintenanceRequestDTO) {
    EXPECT_EQ(R1.id, 301);
    EXPECT_EQ(R1.resident_id, 401);
    EXPECT_EQ(R1.apartment_id, 501);
    EXPECT_EQ(R1.description, "Leaky faucet in kitchen");
    EXPECT_EQ(R1.status, MaintenanceStatus::Open);
    EXPECT_EQ(R1.priority, 3);
}

TEST_F(MaintenanceRequestDTOTest, ValidateSerializationDataFromMaintenanceRequestDTO) {
    nlohmann::json j = R1;

    EXPECT_EQ(j["id"], R1.id);
    EXPECT_EQ(j["resident_id"], R1.resident_id);
    EXPECT_EQ(j["apartment_id"], R1.apartment_id);
    EXPECT_EQ(j["description"], R1.description);
    EXPECT_EQ(j["status"], R1.status);
    EXPECT_EQ(j["priority"], R1.priority);
}

TEST_F(MaintenanceRequestDTOTest, ValidateDerializationDataFromMaintenanceRequestDTO) {
    nlohmann::json j = nlohmann::json{{"id", 302},
                                      {"resident_id", 402},
                                      {"apartment_id", 502},
                                      {"description", "Broken heater"},
                                      {"status", MaintenanceStatus::InProgress},
                                      {"priority", 4}};

    MaintenanceRequestDTO request = j.get<MaintenanceRequestDTO>();

    EXPECT_EQ(request.id, 302);
    EXPECT_EQ(request.resident_id, 402);
    EXPECT_EQ(request.apartment_id, 502);
    EXPECT_EQ(request.description, "Broken heater");
    EXPECT_EQ(request.status, MaintenanceStatus::InProgress);
    EXPECT_EQ(request.priority, 4);
}

TEST_F(MaintenanceRequestDTOTest, ValidateRoundTripDataFromMaintenanceRequestDTO) {
    nlohmann::json j = R1;
    MaintenanceRequestDTO deserializedRequest = j.get<MaintenanceRequestDTO>();

    EXPECT_EQ(R1.id, deserializedRequest.id);
    EXPECT_EQ(R1.resident_id, deserializedRequest.resident_id);
    EXPECT_EQ(R1.apartment_id, deserializedRequest.apartment_id);
    EXPECT_EQ(R1.description, deserializedRequest.description);
    EXPECT_EQ(R1.status, deserializedRequest.status);
    EXPECT_EQ(R1.priority, deserializedRequest.priority);
}
