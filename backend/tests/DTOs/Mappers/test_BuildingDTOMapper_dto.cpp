#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "../../src/DTOs/Mappers/BuildingDTOMapper.h"

class BuildingDTOMapperTest : public ::testing::Test {
   protected:
    Building building;
    BuildingDTO dto;

    BuildingDTOMapperTest() : building(1, "Sunset Apartments", "123 Main St", 5, {}) {
        dto.id = 1;
        dto.name = "Sunset Apartments";
        dto.address = "123 Main St";
        dto.total_floors = 5;
        dto.apartments = {};
    }
};

TEST_F(BuildingDTOMapperTest, ToDTO) {
    auto dto = BuildingDTOMapper::toDTO(building);
    EXPECT_EQ(dto.id, building.getId());
    EXPECT_EQ(dto.name, building.getName());
    EXPECT_EQ(dto.address, building.getAddress());
    EXPECT_EQ(dto.total_floors, building.getNumberOfFloors());
    EXPECT_EQ(dto.apartments.size(), building.getApartments().size());
}

TEST_F(BuildingDTOMapperTest, FromDTO) {
    auto building = BuildingDTOMapper::fromDTO(dto);
    EXPECT_EQ(building.getId(), dto.id);
    EXPECT_EQ(building.getName(), dto.name);
    EXPECT_EQ(building.getAddress(), dto.address);
    EXPECT_EQ(building.getNumberOfFloors(), dto.total_floors);
    EXPECT_EQ(building.getApartments().size(), 0);  // Since we pass empty apartments in fromDTO
}

TEST_F(BuildingDTOMapperTest, RoundTrip) {
    auto dto = BuildingDTOMapper::toDTO(building);
    auto mappedBuilding = BuildingDTOMapper::fromDTO(dto);

    EXPECT_EQ(mappedBuilding.getId(), building.getId());
    EXPECT_EQ(mappedBuilding.getName(), building.getName());
    EXPECT_EQ(mappedBuilding.getAddress(), building.getAddress());
    EXPECT_EQ(mappedBuilding.getNumberOfFloors(), building.getNumberOfFloors());
    EXPECT_EQ(mappedBuilding.getApartments().size(), 0);  // Since we pass empty apartments in fromDTO
}

TEST_F(BuildingDTOMapperTest, FromDTOWithInvalidData) {
    BuildingDTO invalidDto;
    invalidDto.id = -1;             // Invalid ID
    invalidDto.name = "";           // Empty name
    invalidDto.address = "";        // Empty address
    invalidDto.total_floors = -10;  // Invalid number of floors

    auto building = BuildingDTOMapper::fromDTO(invalidDto);
    EXPECT_EQ(building.getId(), invalidDto.id);
    EXPECT_EQ(building.getName(), invalidDto.name);
    EXPECT_EQ(building.getAddress(), invalidDto.address);
    EXPECT_EQ(building.getNumberOfFloors(), invalidDto.total_floors);
    EXPECT_EQ(building.getApartments().size(), 0);
}

TEST_F(BuildingDTOMapperTest, ToDTOWithApartments) {
    // Purpose: Verify BuildingDTOMapper correctly maps apartments in toDTO
    Apartment apt1(101, 1, "A1", false, true, 200.0, 55.5);
    Apartment apt2(102, 1, "A2", true, false, 250.0, 60.0);
    Building buildingWithApts(2, "Maple Residences", "456 Oak Ave", 3, {apt1, apt2});

    auto dto = BuildingDTOMapper::toDTO(buildingWithApts);
    EXPECT_EQ(dto.id, buildingWithApts.getId());
    EXPECT_EQ(dto.name, buildingWithApts.getName());
    EXPECT_EQ(dto.address, buildingWithApts.getAddress());
    EXPECT_EQ(dto.total_floors, buildingWithApts.getNumberOfFloors());
    ASSERT_EQ(dto.apartments.size(), 2);
    EXPECT_EQ(dto.apartments[0].id, apt1.getId());
    EXPECT_EQ(dto.apartments[0].number, apt1.getNumber());
    EXPECT_DOUBLE_EQ(dto.apartments[0].area_m2, apt1.getAreaM2());
    EXPECT_EQ(dto.apartments[1].id, apt2.getId());
    EXPECT_EQ(dto.apartments[1].number, apt2.getNumber());
    EXPECT_DOUBLE_EQ(dto.apartments[1].area_m2, apt2.getAreaM2());
}

TEST_F(BuildingDTOMapperTest, FromDTOWithApartmentsIgnored) {
    // Purpose: Verify BuildingDTOMapper ignores apartments in fromDTO
    ApartmentDTO aptDTO1;
    aptDTO1.id = 201;
    aptDTO1.number = "B1";
    aptDTO1.area_m2 = 70.0;

    ApartmentDTO aptDTO2;
    aptDTO2.id = 202;
    aptDTO2.number = "B2";
    aptDTO2.area_m2 = 80.0;

    BuildingDTO dtoWithApts;
    dtoWithApts.id = 3;
    dtoWithApts.name = "Pine Towers";
    dtoWithApts.address = "789 Pine St";
    dtoWithApts.total_floors = 4;
    dtoWithApts.apartments = {aptDTO1, aptDTO2};

    auto building = BuildingDTOMapper::fromDTO(dtoWithApts);
    EXPECT_EQ(building.getId(), dtoWithApts.id);
    EXPECT_EQ(building.getName(), dtoWithApts.name);
    EXPECT_EQ(building.getAddress(), dtoWithApts.address);
    EXPECT_EQ(building.getNumberOfFloors(), dtoWithApts.total_floors);
    EXPECT_EQ(building.getApartments().size(), 0);  // Apartments should be ignored
    // Include apto would come from other flow and not direct from building DTO creation
}