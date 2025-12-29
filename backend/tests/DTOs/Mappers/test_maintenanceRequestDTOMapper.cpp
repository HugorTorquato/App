#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "../../src/DTOs/Mappers/MaintenanceRequestDTOMapper.h"

class MaintenanceRequestDTOMapperTest : public ::testing::Test {
   protected:
    MaintenanceRequest domainRequest;
    MaintenanceRequestDTO dtoRequest;

    MaintenanceRequestDTOMapperTest() : domainRequest(1, 2, 3, "Leaky faucet in kitchen", MaintenanceStatus::Open, 3) {
        dtoRequest.id = 1;
        dtoRequest.resident_id = 2;
        dtoRequest.apartment_id = 3;
        dtoRequest.description = "Leaky faucet in kitchen";
        dtoRequest.status = MaintenanceStatus::Open;
        dtoRequest.priority = 3;
    }
};

TEST_F(MaintenanceRequestDTOMapperTest, ToDTO) {
    auto dto = MaintenanceRequestDTOMapper::toDTO(domainRequest);
    EXPECT_EQ(dto.id, domainRequest.getId());
    EXPECT_EQ(dto.resident_id, domainRequest.getResidentId());
    EXPECT_EQ(dto.apartment_id, domainRequest.getApartmentId());
    EXPECT_EQ(dto.description, domainRequest.getDescription());
    EXPECT_EQ(dto.status, domainRequest.getStatus());
    EXPECT_EQ(dto.priority, domainRequest.getPriority());
}

TEST_F(MaintenanceRequestDTOMapperTest, fromDTO) {
    auto domain = MaintenanceRequestDTOMapper::fromDTO(dtoRequest);
    EXPECT_EQ(domain.getId(), dtoRequest.id);
    EXPECT_EQ(domain.getResidentId(), dtoRequest.resident_id);
    EXPECT_EQ(domain.getApartmentId(), dtoRequest.apartment_id);
    EXPECT_EQ(domain.getDescription(), dtoRequest.description);
    EXPECT_EQ(domain.getStatus(), dtoRequest.status);
    EXPECT_EQ(domain.getPriority(), dtoRequest.priority);
}

TEST_F(MaintenanceRequestDTOMapperTest, RoundTrip) {
    auto dto = MaintenanceRequestDTOMapper::toDTO(domainRequest);
    auto mappedDomain = MaintenanceRequestDTOMapper::fromDTO(dto);

    EXPECT_EQ(mappedDomain.getId(), domainRequest.getId());
    EXPECT_EQ(mappedDomain.getResidentId(), domainRequest.getResidentId());
    EXPECT_EQ(mappedDomain.getApartmentId(), domainRequest.getApartmentId());
    EXPECT_EQ(mappedDomain.getDescription(), domainRequest.getDescription());
    EXPECT_EQ(mappedDomain.getStatus(), domainRequest.getStatus());
    EXPECT_EQ(mappedDomain.getPriority(), domainRequest.getPriority());
}

TEST_F(MaintenanceRequestDTOMapperTest, FromDTOWithInvalidData) {
    MaintenanceRequestDTO invalidDto;
    invalidDto.id = -1;                                      // Invalid ID
    invalidDto.resident_id = -5;                             // Invalid Resident ID
    invalidDto.apartment_id = -10;                           // Invalid Apartment ID
    invalidDto.description = "";                             // Empty description
    invalidDto.status = static_cast<MaintenanceStatus>(-1);  // Invalid status
    invalidDto.priority = 10;                                // Invalid priority

    auto domain = MaintenanceRequestDTOMapper::fromDTO(invalidDto);
    EXPECT_EQ(domain.getId(), invalidDto.id);
    EXPECT_EQ(domain.getResidentId(), invalidDto.resident_id);
    EXPECT_EQ(domain.getApartmentId(), invalidDto.apartment_id);
    EXPECT_EQ(domain.getDescription(), invalidDto.description);
    EXPECT_EQ(domain.getStatus(), invalidDto.status);
    EXPECT_EQ(domain.getPriority(), invalidDto.priority);
}
