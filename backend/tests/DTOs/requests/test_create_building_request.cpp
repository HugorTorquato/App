#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "../../../src/DTOs/requests/create_building_request.h"

class CreateBuildingRequestTest : public ::testing::Test {
   protected:
    nlohmann::json validJson;
    nlohmann::json invalidJsonMissingField;
    nlohmann::json invalidJsonWrongType;

    CreateBuildingRequestTest() {
        validJson = nlohmann::json{{"name", "Greenwood Residency"}, {"address", "789 Pine St"}, {"total_floors", 8}};

        invalidJsonMissingField = nlohmann::json{{"name", "Greenwood Residency"},
                                                 // Missing address
                                                 {"total_floors", 8}};

        invalidJsonWrongType = nlohmann::json{
            {"name", "Greenwood Residency"}, {"address", "789 Pine St"}, {"total_floors", "eight"}  // Wrong type
        };
    }
};

TEST_F(CreateBuildingRequestTest, ValidDeserialization) {
    // Purpose: Verify CreateBuildingRequest correctly deserializes from valid JSON
    CreateBuildingRequest request = validJson.get<CreateBuildingRequest>();

    EXPECT_EQ(request.name, "Greenwood Residency");
    EXPECT_EQ(request.address, "789 Pine St");
    EXPECT_EQ(request.total_floors, 8);
}

TEST_F(CreateBuildingRequestTest, MissingFieldDeserialization) {
    // Purpose: Verify deserialization fails when a required field is missing
    EXPECT_THROW({ CreateBuildingRequest request = invalidJsonMissingField.get<CreateBuildingRequest>(); },
                 nlohmann::json::out_of_range);
}

TEST_F(CreateBuildingRequestTest, WrongTypeDeserialization) {
    // Purpose: Verify deserialization fails when a field has the wrong type
    EXPECT_THROW({ CreateBuildingRequest request = invalidJsonWrongType.get<CreateBuildingRequest>(); },
                 nlohmann::json::type_error);
}

TEST_F(CreateBuildingRequestTest, RoundTripSerialization) {
    // Purpose: Verify CreateBuildingRequest survives serialization -> deserialization cycle
    CreateBuildingRequest originalRequest;
    originalRequest.name = "Greenwood Residency";
    originalRequest.address = "789 Pine St";
    originalRequest.total_floors = 8;

    nlohmann::json j = {{"name", originalRequest.name},
                        {"address", originalRequest.address},
                        {"total_floors", originalRequest.total_floors}};

    CreateBuildingRequest deserializedRequest = j.get<CreateBuildingRequest>();

    EXPECT_EQ(originalRequest.name, deserializedRequest.name);
    EXPECT_EQ(originalRequest.address, deserializedRequest.address);
    EXPECT_EQ(originalRequest.total_floors, deserializedRequest.total_floors);
}

TEST_F(CreateBuildingRequestTest, ExtraFieldsInJSON) {
    // Purpose: Verify deserialization ignores extra fields in JSON
    nlohmann::json j = nlohmann::json{{"name", "Greenwood Residency"},
                                      {"address", "789 Pine St"},
                                      {"total_floors", 8},
                                      {"extra_field", "should be ignored"}};

    CreateBuildingRequest request = j.get<CreateBuildingRequest>();

    EXPECT_EQ(request.name, "Greenwood Residency");
    EXPECT_EQ(request.address, "789 Pine St");
    EXPECT_EQ(request.total_floors, 8);
}

TEST_F(CreateBuildingRequestTest, EmptyStringsDeserialization) {
    // Purpose: Verify CreateBuildingRequest handles empty strings correctly
    nlohmann::json j = nlohmann::json{{"name", ""}, {"address", ""}, {"total_floors", 0}};

    CreateBuildingRequest request = j.get<CreateBuildingRequest>();

    EXPECT_EQ(request.name, "");
    EXPECT_EQ(request.address, "");
    EXPECT_EQ(request.total_floors, 0);
}