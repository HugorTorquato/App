#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "../../../src/DTOs/responses/building_response.h"

class BuildingResponseTest : public ::testing::Test {
   protected:
    BuildingDTO buildingDTO;
    BuildingResponse buildingResponse;

    BuildingResponseTest() {
        buildingDTO.id = 1;
        buildingDTO.name = "Sunset Apartments";
        buildingDTO.address = "123 Main St";
        buildingDTO.total_floors = 5;

        ApartmentDTO apt1;
        apt1.id = 101;
        apt1.number = "A1";
        apt1.area_m2 = 55.5;

        ApartmentDTO apt2;
        apt2.id = 102;
        apt2.number = "A2";
        apt2.area_m2 = 60.0;

        buildingDTO.apartments = {apt1, apt2};

        buildingResponse.data = buildingDTO;
    }
};

TEST_F(BuildingResponseTest, ValidateDTOPData) {
    EXPECT_EQ(buildingDTO.id, 1);
    EXPECT_EQ(buildingDTO.name, "Sunset Apartments");
    EXPECT_EQ(buildingDTO.address, "123 Main St");
    EXPECT_EQ(buildingDTO.total_floors, 5);

    EXPECT_EQ(buildingResponse.data.id, buildingDTO.id);
    EXPECT_EQ(buildingResponse.data.name, buildingDTO.name);
    EXPECT_EQ(buildingResponse.data.address, buildingDTO.address);
    EXPECT_EQ(buildingResponse.data.total_floors, buildingDTO.total_floors);
}

TEST_F(BuildingResponseTest, serializationDTOdata) {
    nlohmann::json j = buildingResponse;

    EXPECT_EQ(j["building"]["id"], buildingDTO.id);
    EXPECT_EQ(j["building"]["name"], buildingDTO.name);
    EXPECT_EQ(j["building"]["address"], buildingDTO.address);
    EXPECT_EQ(j["building"]["total_floors"], buildingDTO.total_floors);
    EXPECT_EQ(j["building"]["apartments"].size(), buildingDTO.apartments.size());
}
