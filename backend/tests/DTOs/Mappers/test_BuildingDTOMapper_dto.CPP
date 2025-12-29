#include <gtest/gtest.h>

#include <nlohmann/json.hpp>


#include "../../src/DTOs/Mappers/BuildingDTOMapper.h"

class BuildingDTOMapperTest : public ::testing::Test {
   protected:
    Building building;
    BuildingDTO dto;

    BuildingDTOMapperTest() : building(1, "Sunset Apartments", "123 Main St", 5) {
        dto.id = 1;
        dto.name = "Sunset Apartments";
        dto.address = "123 Main St";
        dto.total_floors = 5;
    }
};

TEST_F(BuildingDTOMapperTest, ToDTO) {
    auto dto = BuildingDTOMapper::toDTO(building);
    EXPECT_EQ(dto.id, building.getId());
    EXPECT_EQ(dto.name, building.getName());
    EXPECT_EQ(dto.address, building.getAddress());
    EXPECT_EQ(dto.total_floors, building.getNumberOfFloors());
}

TEST_F(BuildingDTOMapperTest, FromDTO) {
    auto building = BuildingDTOMapper::fromDTO(dto);
    EXPECT_EQ(building.getId(), dto.id);
    EXPECT_EQ(building.getName(), dto.name);
    EXPECT_EQ(building.getAddress(), dto.address);
    EXPECT_EQ(building.getNumberOfFloors(), dto.total_floors);
}

TEST_F(BuildingDTOMapperTest, RoundTrip) {
    auto dto = BuildingDTOMapper::toDTO(building);
    auto mappedBuilding = BuildingDTOMapper::fromDTO(dto);

    EXPECT_EQ(mappedBuilding.getId(), building.getId());
    EXPECT_EQ(mappedBuilding.getName(), building.getName());
    EXPECT_EQ(mappedBuilding.getAddress(), building.getAddress());
    EXPECT_EQ(mappedBuilding.getNumberOfFloors(), building.getNumberOfFloors());
}

TEST_F(BuildingDTOMapperTest, FromDTOWithInvalidData) {
    BuildingDTO invalidDto;
    invalidDto.id = -1;               // Invalid ID
    invalidDto.name = "";             // Empty name
    invalidDto.address = "";          // Empty address
    invalidDto.total_floors = -10;   // Invalid number of floors

    auto building = BuildingDTOMapper::fromDTO(invalidDto);
    EXPECT_EQ(building.getId(), invalidDto.id);
    EXPECT_EQ(building.getName(), invalidDto.name);
    EXPECT_EQ(building.getAddress(), invalidDto.address);
    EXPECT_EQ(building.getNumberOfFloors(), invalidDto.total_floors);
}
