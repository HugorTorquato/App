#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "../../src/DTOs/building_dto.h"

class BuildingDTOTest : public ::testing::Test {
protected:

    ApartmentDTO A1;
    ApartmentDTO A2;
    BuildingDTO building;

    BuildingDTOTest() {

        A1.id = 101;
        A1.number = "A1";
        A1.area_m2 = 55.5;

        A2.id = 102;
        A2.number = "A2";
        A2.area_m2 = 60.0;

        building.id = 1;
        building.name = "Sunset Apartments";
        building.address = "123 Main St";
        building.total_floors = 5;
        building.apartments = {A1, A2};
    }
};

// ===== APARTMENT DTO TESTS =====

TEST_F(BuildingDTOTest, ApartmentDTOInitialization) {
    // Purpose: Verify ApartmentDTO correctly initializes with expected values
    EXPECT_EQ(A1.id, 101);
    EXPECT_EQ(A1.number, "A1");
    EXPECT_DOUBLE_EQ(A1.area_m2, 55.5);
}

TEST_F(BuildingDTOTest, ApartmentDTOSerialization) {
    // Purpose: Verify ApartmentDTO correctly serializes to JSON
    nlohmann::json j = A1;
    
    EXPECT_EQ(j["id"], 101);
    EXPECT_EQ(j["number"], "A1");
    EXPECT_DOUBLE_EQ(j["area_m2"], 55.5);
}

TEST_F(BuildingDTOTest, ApartmentDTODeserialization) {
    // Purpose: Verify ApartmentDTO correctly deserializes from JSON
    nlohmann::json j = nlohmann::json{
        {"id", 103},
        {"number", "A3"},
        {"area_m2", 75.0}
    };
    
    ApartmentDTO apartment = j.get<ApartmentDTO>();
    
    EXPECT_EQ(apartment.id, 103);
    EXPECT_EQ(apartment.number, "A3");
    EXPECT_DOUBLE_EQ(apartment.area_m2, 75.0);
}

TEST_F(BuildingDTOTest, ApartmentDTORoundTrip) {
    // Purpose: Verify ApartmentDTO survives serialization -> deserialization cycle
    nlohmann::json j = A2;
    ApartmentDTO deserializedApartment = j.get<ApartmentDTO>();
    
    EXPECT_EQ(A2.id, deserializedApartment.id);
    EXPECT_EQ(A2.number, deserializedApartment.number);
    EXPECT_DOUBLE_EQ(A2.area_m2, deserializedApartment.area_m2);
}

TEST_F(BuildingDTOTest, ApartmentDTOLargeArea) {
    // Purpose: Verify ApartmentDTO handles large area values correctly
    ApartmentDTO largeApt;
    largeApt.id = 999;
    largeApt.number = "PENTHOUSE";
    largeApt.area_m2 = 500.75;
    
    nlohmann::json j = largeApt;
    ApartmentDTO deserialized = j.get<ApartmentDTO>();
    
    EXPECT_DOUBLE_EQ(deserialized.area_m2, 500.75);
}

TEST_F(BuildingDTOTest, ApartmentDTOSmallArea) {
    // Purpose: Verify ApartmentDTO handles minimal area values
    ApartmentDTO smallApt;
    smallApt.id = 1;
    smallApt.number = "STUDIO";
    smallApt.area_m2 = 25.0;
    
    nlohmann::json j = smallApt;
    ApartmentDTO deserialized = j.get<ApartmentDTO>();
    
    EXPECT_DOUBLE_EQ(deserialized.area_m2, 25.0);
}

// ===== BUILDING DTO TESTS =====

TEST_F(BuildingDTOTest, BuildingDTOInitialization) {
    // Purpose: Verify BuildingDTO correctly initializes with expected values
    EXPECT_EQ(building.id, 1);
    EXPECT_EQ(building.name, "Sunset Apartments");
    EXPECT_EQ(building.address, "123 Main St");
    EXPECT_EQ(building.total_floors, 5);
    ASSERT_EQ(building.apartments.size(), 2);
}

TEST_F(BuildingDTOTest, BuildingDTOSerialization) {
    // Purpose: Verify BuildingDTO correctly serializes to JSON with all fields
    nlohmann::json j = building;
    
    EXPECT_EQ(j["id"], 1);
    EXPECT_EQ(j["name"], "Sunset Apartments");
    EXPECT_EQ(j["address"], "123 Main St");
    EXPECT_EQ(j["total_floors"], 5);
    EXPECT_EQ(j["apartments"].size(), 2);
}

TEST_F(BuildingDTOTest, BuildingDTODeserialization) {
    // Purpose: Verify BuildingDTO correctly deserializes from JSON
    nlohmann::json j = nlohmann::json{
        {"id", 2},
        {"name", "Downtown Tower"},
        {"address", "456 Oak Ave"},
        {"total_floors", 10},
        {"apartments", nlohmann::json::array()}
    };
    
    BuildingDTO building_new = j.get<BuildingDTO>();
    
    EXPECT_EQ(building_new.id, 2);
    EXPECT_EQ(building_new.name, "Downtown Tower");
    EXPECT_EQ(building_new.address, "456 Oak Ave");
    EXPECT_EQ(building_new.total_floors, 10);
    EXPECT_EQ(building_new.apartments.size(), 0);
}

TEST_F(BuildingDTOTest, BuildingDTORoundTrip) {
    // Purpose: Verify BuildingDTO survives full serialization -> deserialization cycle
    nlohmann::json j = building;
    BuildingDTO deserializedBuilding = j.get<BuildingDTO>();

    EXPECT_EQ(building.id, deserializedBuilding.id);
    EXPECT_EQ(building.name, deserializedBuilding.name);
    EXPECT_EQ(building.address, deserializedBuilding.address);
    EXPECT_EQ(building.total_floors, deserializedBuilding.total_floors);
    ASSERT_EQ(building.apartments.size(), deserializedBuilding.apartments.size());
    
    for (size_t i = 0; i < building.apartments.size(); ++i) {
        EXPECT_EQ(building.apartments[i].id, deserializedBuilding.apartments[i].id);
        EXPECT_EQ(building.apartments[i].number, deserializedBuilding.apartments[i].number);
        EXPECT_DOUBLE_EQ(building.apartments[i].area_m2, deserializedBuilding.apartments[i].area_m2);
    }
}

TEST_F(BuildingDTOTest, BuildingDTOEmptyApartments) {
    // Purpose: Verify BuildingDTO handles empty apartment list correctly
    BuildingDTO emptyBuilding;
    emptyBuilding.id = 3;
    emptyBuilding.name = "Vacant Building";
    emptyBuilding.address = "789 Empty St";
    emptyBuilding.total_floors = 2;
    emptyBuilding.apartments = {};
    
    nlohmann::json j = emptyBuilding;
    BuildingDTO deserialized = j.get<BuildingDTO>();
    
    EXPECT_EQ(deserialized.apartments.size(), 0);
    EXPECT_EQ(deserialized.name, "Vacant Building");
}

TEST_F(BuildingDTOTest, BuildingDTOMultipleApartments) {
    // Purpose: Verify BuildingDTO correctly handles multiple apartments
    BuildingDTO multiBuilding;
    multiBuilding.id = 4;
    multiBuilding.name = "Large Complex";
    multiBuilding.address = "999 Big Ave";
    multiBuilding.total_floors = 20;
    
    // Add 5 apartments
    for (int i = 0; i < 5; ++i) {
        ApartmentDTO apt;
        apt.id = 200 + i;
        apt.number = "APT_" + std::to_string(i);
        apt.area_m2 = 45.0 + (i * 5);
        multiBuilding.apartments.push_back(apt);
    }
    
    nlohmann::json j = multiBuilding;
    BuildingDTO deserialized = j.get<BuildingDTO>();
    
    EXPECT_EQ(deserialized.apartments.size(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(deserialized.apartments[i].id, 200 + i);
        EXPECT_DOUBLE_EQ(deserialized.apartments[i].area_m2, 45.0 + (i * 5));
    }
}

TEST_F(BuildingDTOTest, BuildingDTOHighFloors) {
    // Purpose: Verify BuildingDTO handles high floor counts
    BuildingDTO tallBuilding;
    tallBuilding.id = 5;
    tallBuilding.name = "Skyscraper";
    tallBuilding.address = "100 Sky Lane";
    tallBuilding.total_floors = 100;
    
    nlohmann::json j = tallBuilding;
    BuildingDTO deserialized = j.get<BuildingDTO>();
    
    EXPECT_EQ(deserialized.total_floors, 100);
}

TEST_F(BuildingDTOTest, BuildingDTOSpecialCharactersInName) {
    // Purpose: Verify BuildingDTO handles special characters in name/address
    BuildingDTO specialBuilding;
    specialBuilding.id = 6;
    specialBuilding.name = "Apt's & Towers (2024)";
    specialBuilding.address = "123 O'Brien St, Suite #500";
    specialBuilding.total_floors = 7;
    
    nlohmann::json j = specialBuilding;
    BuildingDTO deserialized = j.get<BuildingDTO>();
    
    EXPECT_EQ(deserialized.name, "Apt's & Towers (2024)");
    EXPECT_EQ(deserialized.address, "123 O'Brien St, Suite #500");
}

TEST_F(BuildingDTOTest, ApartmentDTOSpecialCharactersInNumber) {
    // Purpose: Verify ApartmentDTO handles special characters in apartment number
    ApartmentDTO specialApt;
    specialApt.id = 555;
    specialApt.number = "12A-1";
    specialApt.area_m2 = 65.5;
    
    nlohmann::json j = specialApt;
    ApartmentDTO deserialized = j.get<ApartmentDTO>();
    
    EXPECT_EQ(deserialized.number, "12A-1");
}

TEST_F(BuildingDTOTest, BuildingDTOJSONStructure) {
    // Purpose: Verify the exact JSON structure matches API expectations
    nlohmann::json j = building;
    
    // Verify all expected keys exist
    EXPECT_TRUE(j.contains("id"));
    EXPECT_TRUE(j.contains("name"));
    EXPECT_TRUE(j.contains("address"));
    EXPECT_TRUE(j.contains("total_floors"));
    EXPECT_TRUE(j.contains("apartments"));
    
    // Verify types
    EXPECT_TRUE(j["id"].is_number());
    EXPECT_TRUE(j["name"].is_string());
    EXPECT_TRUE(j["address"].is_string());
    EXPECT_TRUE(j["total_floors"].is_number());
    EXPECT_TRUE(j["apartments"].is_array());
}

TEST_F(BuildingDTOTest, ApartmentDTOJSONStructure) {
    // Purpose: Verify the exact JSON structure of apartments
    nlohmann::json j = A1;
    
    // Verify all expected keys exist
    EXPECT_TRUE(j.contains("id"));
    EXPECT_TRUE(j.contains("number"));
    EXPECT_TRUE(j.contains("area_m2"));
    
    // Verify types
    EXPECT_TRUE(j["id"].is_number());
    EXPECT_TRUE(j["number"].is_string());
    EXPECT_TRUE(j["area_m2"].is_number_float());
}