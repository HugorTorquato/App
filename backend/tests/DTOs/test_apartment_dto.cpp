#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "../../src/DTOs/apartment_dto.h"

class ApartmentDTOTest : public ::testing::Test {
   protected:
    ApartmentDTO R1;

    ApartmentDTOTest() {
        R1.id = 201;
        R1.number = "101";
        R1.area_m2 = 75.5;
    }
};

TEST_F(ApartmentDTOTest, ValidateDataFromResidentDTO) {
    EXPECT_EQ(R1.id, 201);
    EXPECT_EQ(R1.number, "101");
    EXPECT_EQ(R1.area_m2, 75.5);
}

TEST_F(ApartmentDTOTest, ValidateSerializationDataFromApartmentDTO) {
    nlohmann::json j = R1;

    EXPECT_EQ(j["id"], R1.id);
    EXPECT_EQ(j["number"], R1.number);
    EXPECT_EQ(j["area_m2"], R1.area_m2);
}

TEST_F(ApartmentDTOTest, ValidateDerializationDataFromApartmentDTO) {
    nlohmann::json j = nlohmann::json{{"id", 103}, {"number", "101"}, {"area_m2", 80.0}};

    ApartmentDTO apartment = j.get<ApartmentDTO>();

    EXPECT_EQ(apartment.id, 103);
    EXPECT_EQ(apartment.number, "101");
    EXPECT_EQ(apartment.area_m2, 80.0);
}

TEST_F(ApartmentDTOTest, ValidateRoundTripDataFromApartmentDTO) {
    nlohmann::json j = R1;
    ApartmentDTO deserializedApartment = j.get<ApartmentDTO>();

    EXPECT_EQ(R1.id, deserializedApartment.id);
    EXPECT_EQ(R1.number, deserializedApartment.number);
    EXPECT_EQ(R1.area_m2, deserializedApartment.area_m2);
}
