#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "../../src/DTOs/resident_dto.h"

class ResidentDTOTest : public ::testing::Test {
   protected:
    ResidentDTO R1;

    ResidentDTOTest() {
        R1.id = 201;
        R1.name = "John Doe";
        R1.phone = "9876543210";
        R1.apartment_number = "101";
    }
};

TEST_F(ResidentDTOTest, ValidateDataFromResidentDTO) {
    EXPECT_EQ(R1.id, 201);
    EXPECT_EQ(R1.name, "John Doe");
    EXPECT_EQ(R1.phone, "9876543210");
    EXPECT_EQ(R1.apartment_number, "101");
}

TEST_F(ResidentDTOTest, ValidateSerializationDataFromResidentDTO) {
    nlohmann::json j = R1;

    EXPECT_EQ(j["id"], R1.id);
    EXPECT_EQ(j["name"], R1.name);
    EXPECT_EQ(j["phone"], R1.phone);
    EXPECT_EQ(j["apartment_number"], R1.apartment_number);
}

TEST_F(ResidentDTOTest, ValidateDerializationDataFromResidentDTO) {
    nlohmann::json j =
        nlohmann::json{{"id", 103}, {"name", "John Doe"}, {"phone", "9876543210"}, {"apartment_number", "101"}};

    ResidentDTO resident = j.get<ResidentDTO>();

    EXPECT_EQ(resident.id, 103);
    EXPECT_EQ(resident.name, "John Doe");
    EXPECT_EQ(resident.phone, "9876543210");
    EXPECT_EQ(resident.apartment_number, "101");
}

TEST_F(ResidentDTOTest, ValidateRoundTripDataFromResidentDTO) {
    nlohmann::json j = R1;
    ResidentDTO deserializedResident = j.get<ResidentDTO>();

    EXPECT_EQ(R1.id, deserializedResident.id);
    EXPECT_EQ(R1.name, deserializedResident.name);
    EXPECT_EQ(R1.phone, deserializedResident.phone);
    EXPECT_EQ(R1.apartment_number, deserializedResident.apartment_number);
}

TEST_F(ResidentDTOTest, ValidateEqualityOperatorForResidentDTO) {
    ResidentDTO R2;
    R2.id = 201;
    R2.name = "John Doe";
    R2.phone = "9876543210";
    R2.apartment_number = "101";

    EXPECT_TRUE(R1 == R2);

    R2.phone = "1234567890";  // Change one field
    EXPECT_FALSE(R1 == R2);
}