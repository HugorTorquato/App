#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "../../src/DTOs/Mappers/ResidentDTOMapper.h"

class ResidentDTOMapperTest : public ::testing::Test {
   protected:
    Resident resident;
    ResidentDTO dto;

    ResidentDTOMapperTest() : resident(1, "Alice Johnson", "98765432100", "404", true, "555-0202", 1622505600, 0) {
        dto.id = 1;
        dto.name = "Alice Johnson";
        dto.phone = "555-0202";
        dto.apartment_number = "404";
    }
};

TEST_F(ResidentDTOMapperTest, ToDTO) {
    auto dto = ResidentDTOMapper::toDTO(resident);
    EXPECT_EQ(dto.id, resident.getId());
    EXPECT_EQ(dto.name, resident.getFullName());
    EXPECT_EQ(dto.phone, resident.getPhone());
    EXPECT_EQ(dto.apartment_number, resident.getApartmentId());
}

TEST_F(ResidentDTOMapperTest, FromDTO) {
    auto resident = ResidentDTOMapper::fromDTO(dto);
    EXPECT_EQ(resident.getId(), dto.id);
    EXPECT_EQ(resident.getFullName(), dto.name);
    EXPECT_EQ(resident.getPhone(), dto.phone);
    EXPECT_EQ(resident.getApartmentId(), dto.apartment_number);
}

TEST_F(ResidentDTOMapperTest, RoundTrip) {
    auto dto = ResidentDTOMapper::toDTO(resident);
    auto mappedResident = ResidentDTOMapper::fromDTO(dto);

    EXPECT_EQ(mappedResident.getId(), resident.getId());
    EXPECT_EQ(mappedResident.getFullName(), resident.getFullName());
    EXPECT_EQ(mappedResident.getPhone(), resident.getPhone());
    EXPECT_EQ(mappedResident.getApartmentId(), resident.getApartmentId());
}

TEST_F(ResidentDTOMapperTest, FromDTOWithInvalidData) {
    ResidentDTO invalidDto;
    invalidDto.id = -1;                  // Invalid ID
    invalidDto.name = "";                // Empty name
    invalidDto.phone = "invalid-phone";  // Invalid phone format
    invalidDto.apartment_number = "";    // Empty apartment number

    auto resident = ResidentDTOMapper::fromDTO(invalidDto);
    EXPECT_EQ(resident.getId(), invalidDto.id);
    EXPECT_EQ(resident.getFullName(), invalidDto.name);
    EXPECT_EQ(resident.getPhone(), invalidDto.phone);
    EXPECT_EQ(resident.getApartmentId(), invalidDto.apartment_number);
}