#include <gtest/gtest.h>

#include "../../../src/DTOs/Mappers/MaintenanceRequestDTOMapper.h"
#include "../../../src/Repositories/inmemory/InMemoryMaintenanceRequestRepository.h"
#include "../../../src/Services/MaintenanceRequest/MaintenanceRequestService.h"
#include "../../Utils/DomainEnums.h"

class ServiceMaintenanceRequestTest : public ::testing::Test {
   protected:
    InMemoryMaintenanceRequestRepository repository;
    MaintenanceRequestService service;

    MaintenanceRequest request1;
    MaintenanceRequest request2;

    ServiceMaintenanceRequestTest()
        : service(repository),
          request1(0, 1, 101, "Leaky faucet in kitchen", MaintenanceStatus::Open, 1),
          request2(1, 2, 202, "Heating not working", MaintenanceStatus::InProgress, 3) {}

    void SetUp() override {}
};

TEST_F(ServiceMaintenanceRequestTest, CreateMaintenanceRequestStoresRequestInRepository) {
    auto id1 = service.createMaintenanceRequest(MaintenanceRequestDTOMapper::toDTO(request1));
    auto id2 = service.createMaintenanceRequest(MaintenanceRequestDTOMapper::toDTO(request2));

    EXPECT_EQ(id1, 0);
    EXPECT_EQ(id2, 1);

    auto allRequests = repository.findAll();
    EXPECT_EQ(allRequests.size(), 2);

    EXPECT_EQ(allRequests[0].getDescription(), "Leaky faucet in kitchen");
    EXPECT_EQ(allRequests[1].getDescription(), "Heating not working");
}

TEST_F(ServiceMaintenanceRequestTest, GetMaintenanceRequestByIdReturnsCorrectRequest) {
    auto id1 = service.createMaintenanceRequest(MaintenanceRequestDTOMapper::toDTO(request1));
    auto id2 = service.createMaintenanceRequest(MaintenanceRequestDTOMapper::toDTO(request2));

    auto fetchedRequest1Opt = service.getMaintenanceRequestById(id1);
    ASSERT_TRUE(fetchedRequest1Opt.has_value());
    auto fetchedRequest1DTO = fetchedRequest1Opt.value();
    auto fetchedRequest1 = MaintenanceRequestDTOMapper::fromDTO(fetchedRequest1DTO);
    EXPECT_EQ(fetchedRequest1.getDescription(), "Leaky faucet in kitchen");

    auto fetchedRequest2Opt = service.getMaintenanceRequestById(id2);
    ASSERT_TRUE(fetchedRequest2Opt.has_value());
    auto fetchedRequest2DTO = fetchedRequest2Opt.value();
    auto fetchedRequest2 = MaintenanceRequestDTOMapper::fromDTO(fetchedRequest2DTO);
    EXPECT_EQ(fetchedRequest2.getDescription(), "Heating not working");
}

TEST_F(ServiceMaintenanceRequestTest, GetMaintenanceRequestByIdReturnsNulloptForNonExistentId) {
    auto id1 = service.createMaintenanceRequest(MaintenanceRequestDTOMapper::toDTO(request1));
    auto id2 = service.createMaintenanceRequest(MaintenanceRequestDTOMapper::toDTO(request2));

    constexpr int nonExistentId = 9999;
    auto fetchedRequestOpt = service.getMaintenanceRequestById(nonExistentId);
    EXPECT_FALSE(fetchedRequestOpt.has_value());
}

TEST_F(ServiceMaintenanceRequestTest, CreateMultipleRequestsAndFetchThem) {
    const int numRequests = 5;
    for (int i = 0; i < numRequests; ++i) {
        MaintenanceRequestDTO dto;
        dto.id = i;
        dto.resident_id = i + 1;
        dto.apartment_id = 100 + i;
        dto.description = "Issue " + std::to_string(i);
        dto.status = MaintenanceStatus::Open;
        dto.priority = (i % 5) + 1;
        service.createMaintenanceRequest(dto);
    }

    for (int i = 0; i < numRequests; ++i) {
        auto fetchedRequestOpt = service.getMaintenanceRequestById(i);
        ASSERT_TRUE(fetchedRequestOpt.has_value());
        auto fetchedRequestDTO = fetchedRequestOpt.value();
        EXPECT_EQ(fetchedRequestDTO.description, "Issue " + std::to_string(i));
        EXPECT_EQ(fetchedRequestDTO.resident_id, i + 1);
        EXPECT_EQ(fetchedRequestDTO.apartment_id, 100 + i);
    }
}
