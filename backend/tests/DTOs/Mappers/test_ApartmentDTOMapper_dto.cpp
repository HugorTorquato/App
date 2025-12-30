#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "../../Utils/Logger.h"
#include "../../src/DTOs/Mappers/ApartmentDTOMapper.h"
#include "Utils/Logger.h"

class ApartmentDTOMapperTest : public ::testing::Test {
   protected:
    Apartment A1;
    ApartmentDTO R1;

    ApartmentDTOMapperTest() : A1(1, 1, "101", false, true, 250.0, 75.5) {
        R1.id = 201;
        R1.number = "101";
        R1.area_m2 = 75.5;
    }
};

TEST_F(ApartmentDTOMapperTest, ToDTO) {
    auto dto = ApartmentDTOMapper::toDTO(A1);
    EXPECT_EQ(dto.id, A1.getId());
    EXPECT_EQ(dto.number, A1.getNumber());
    EXPECT_EQ(dto.area_m2, A1.getAreaM2());
}

TEST_F(ApartmentDTOMapperTest, FromDTO) {
    auto apartment = ApartmentDTOMapper::fromDTO(R1);

    EXPECT_EQ(apartment.getId(), R1.id);
    EXPECT_EQ(apartment.getNumber(), R1.number);
    EXPECT_EQ(apartment.getAreaM2(), R1.area_m2);
}

TEST_F(ApartmentDTOMapperTest, RoundTrip) {
    auto dto = ApartmentDTOMapper::toDTO(A1);
    auto mappedApartment = ApartmentDTOMapper::fromDTO(dto);

    EXPECT_EQ(mappedApartment.getId(), A1.getId());
    EXPECT_EQ(mappedApartment.getNumber(), A1.getNumber());
    EXPECT_EQ(mappedApartment.getAreaM2(), A1.getAreaM2());
}

TEST_F(ApartmentDTOMapperTest, FromDTOWithInvalidData) {
    ApartmentDTO invalidDto;
    invalidDto.id = -1;          // Invalid ID
    invalidDto.number = "";      // Empty number
    invalidDto.area_m2 = -50.0;  // Invalid area

    auto apartment = ApartmentDTOMapper::fromDTO(invalidDto);
    EXPECT_EQ(apartment.getId(), invalidDto.id);
    EXPECT_EQ(apartment.getNumber(), invalidDto.number);
    EXPECT_EQ(apartment.getAreaM2(), invalidDto.area_m2);
}