#include <gtest/gtest.h>

#include "../../../src/Repositories/inmemory/InMemoryMaintenanceRequestRepository.h"

class InMemoryMaintenanceRequestRepositoryTest : public ::testing::Test {
   protected:
    InMemoryMaintenanceRequestRepository repository;

    MaintenanceRequest request1;
    MaintenanceRequest request2;
    MaintenanceRequest request3;

    InMemoryMaintenanceRequestRepositoryTest()
        : request1(0, 1, 101, "Leaky faucet", MaintenanceRequest::MaintenanceStatus::Open, 3),
          request2(0, 2, 202, "Broken heater", MaintenanceRequest::MaintenanceStatus::InProgress, 5),
          request3(0, 3, 303, "Clogged drain", MaintenanceRequest::MaintenanceStatus::Completed, 2) {}

    void SetUp() override {}
};

TEST_F(InMemoryMaintenanceRequestRepositoryTest, SaveAssignsUniqueIds) {
    auto id1 = repository.save(request1);
    auto id2 = repository.save(request2);
    auto id3 = repository.save(request3);

    EXPECT_NE(id1, id2);
    EXPECT_NE(id1, id3);
    EXPECT_NE(id2, id3);

    EXPECT_EQ(id1, 0);
    EXPECT_EQ(id2, 1);
    EXPECT_EQ(id3, 2);
}

TEST_F(InMemoryMaintenanceRequestRepositoryTest, FindByIdReturnsNulloptForNonExistentId) {
    auto result = repository.findById(9999);  // Assuming 9999 is a non-existent ID
    EXPECT_FALSE(result.has_value());
    EXPECT_FALSE(result);
}

TEST_F(InMemoryMaintenanceRequestRepositoryTest, FindByIdReturnsRequestForExistingId) {
    auto id1 = repository.save(request1);
    auto id2 = repository.save(request2);
    auto id3 = repository.save(request3);
    auto result = repository.findById(id1);

    ASSERT_TRUE(result.has_value());
    auto foundRequest = result.value();

    EXPECT_EQ(foundRequest.getId(), id1);
    EXPECT_EQ(foundRequest.getResidentId(), request1.getResidentId());
    EXPECT_EQ(foundRequest.getApartmentId(), request1.getApartmentId());
    EXPECT_EQ(foundRequest.getDescription(), request1.getDescription());
    EXPECT_EQ(foundRequest.getStatus(), request1.getStatus());
    EXPECT_EQ(foundRequest.getPriority(), request1.getPriority());
}

TEST_F(InMemoryMaintenanceRequestRepositoryTest, FindAllReturnsAllRequests) {
    repository.save(request1);
    repository.save(request2);
    repository.save(request3);

    auto requests = repository.findAll();

    EXPECT_EQ(requests.size(), 3);

    EXPECT_EQ(requests[0].getDescription(), request1.getDescription());
    EXPECT_EQ(requests[1].getDescription(), request2.getDescription());
    EXPECT_EQ(requests[2].getDescription(), request3.getDescription());
}

TEST_F(InMemoryMaintenanceRequestRepositoryTest, FindAllReturnsEmptyVectorWhenNoRequests) {
    auto requests = repository.findAll();
    EXPECT_TRUE(requests.empty());
}

TEST_F(InMemoryMaintenanceRequestRepositoryTest, UpdateDataFromExistingRequest) {
    auto id = repository.save(request1);
    auto result = repository.findById(id);
    ASSERT_TRUE(result.has_value());
    auto requestToUpdate = result.value();

    requestToUpdate.updateMaintenanceRequestInfos(std::nullopt, std::nullopt, std::nullopt,
                                                  std::string("Updated description"),
                                                  MaintenanceRequest::MaintenanceStatus::InProgress, 4);
    repository.update(requestToUpdate);

    auto updatedResult = repository.findById(id);
    ASSERT_TRUE(updatedResult.has_value());
    auto updatedRequest = updatedResult.value();

    EXPECT_EQ(updatedRequest.getDescription(), "Updated description");
    EXPECT_EQ(updatedRequest.getStatus(), MaintenanceRequest::MaintenanceStatus::InProgress);
    EXPECT_EQ(updatedRequest.getPriority(), 4);
}

TEST_F(InMemoryMaintenanceRequestRepositoryTest, UpdateNonExistentRequestDoesNothing) {
    auto id = repository.save(request1);
    auto result = repository.findById(id);
    ASSERT_TRUE(result.has_value());
    auto requestToUpdate = result.value();

    // Update with invalid ID
    constexpr int invalidId = 9999;
    requestToUpdate.updateMaintenanceRequestInfos(invalidId, std::nullopt, std::nullopt,
                                                  std::string("Updated description"),
                                                  MaintenanceRequest::MaintenanceStatus::InProgress, 4);
    repository.update(requestToUpdate);

    auto unchangedResult = repository.findById(id);
    ASSERT_TRUE(unchangedResult.has_value());
    auto unchangedRequest = unchangedResult.value();

    // Data should remain unchanged
    EXPECT_EQ(unchangedRequest.getDescription(), request1.getDescription());
    EXPECT_EQ(unchangedRequest.getStatus(), request1.getStatus());
    EXPECT_EQ(unchangedRequest.getPriority(), request1.getPriority());
}

TEST_F(InMemoryMaintenanceRequestRepositoryTest, RemoveExistingRequest) {
    auto id = repository.save(request1);
    auto result = repository.findById(id);
    ASSERT_TRUE(result.has_value());

    repository.remove(id);

    auto postRemoveResult = repository.findById(id);
    EXPECT_FALSE(postRemoveResult.has_value());
}

TEST_F(InMemoryMaintenanceRequestRepositoryTest, RemoveNonExistentIdDoesNothing) {
    auto id = repository.save(request1);
    auto result = repository.findById(id);
    ASSERT_TRUE(result.has_value());

    repository.remove(9999);  // Assuming 9999 is a non-existent ID

    auto postRemoveResult = repository.findById(id);
    EXPECT_TRUE(postRemoveResult.has_value());
}
