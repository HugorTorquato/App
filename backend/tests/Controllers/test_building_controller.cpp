#include <gmock/gmock.h>

#include "../../../src/Controllers/BuildingController.h"
#include "../../Services/Building/IBuildingService.h"
#include "Utils/Logger.h"

// For controller unit tests, the key idea is:

// mock the service

// test only:

// request parsing

// HTTP status codes

// JSON output

// interaction with the service

// You do not spin up Crow or hit real HTTP.
// You test only the controller class in isolation

class MockBuildingService : public IBuildingService {
   public:
    MOCK_METHOD(int, createBuilding, (const BuildingDTO& buildingDTO), (override));
    MOCK_METHOD(std::optional<BuildingDTO>, getBuildingById, (int id), (override));
};

class BuildingControllerTests : public ::testing::Test {
   protected:
    // I'm not testing anything from this logic. Use mock to make this unit test self contained
    MockBuildingService mockService;
    BuildingController controller{mockService};
};

TEST_F(BuildingControllerTests, CreateBuildingReturns201AndBuildingJsonOnSuccess) {
    // Arrange
    constexpr int expectedBuildingId = 0;
    BuildingDTO inputDTO{expectedBuildingId, "Building A", "123 Main St", 10, {}};
    BuildingDTO returnedDTO = inputDTO;

    nlohmann::json requestJson = inputDTO;
    crow::request req;
    req.body = requestJson.dump();

    EXPECT_CALL(mockService, createBuilding(testing::Eq(inputDTO))).WillOnce(testing::Return(0));

    // Act
    crow::response res = controller.create(req);

    // Assert
    EXPECT_EQ(res.code, 201);
    Logger::info("Response DTO: " + res.body);
    EXPECT_EQ(res.body, nlohmann::json({{"id", expectedBuildingId}}).dump());
}

TEST_F(BuildingControllerTests, GetBuildingByIdReturns200AndBuildingJsonOnSuccess) {
    // Arrange
    constexpr int buildingId = 0;
    BuildingDTO returnedDTO{buildingId, "Building A", "123 Main St", 10, {}};

    EXPECT_CALL(mockService, getBuildingById(buildingId)).WillOnce(testing::Return(returnedDTO));

    // Act
    crow::response res = controller.getById(buildingId);

    // Assert
    EXPECT_EQ(res.code, 200);
    nlohmann::json responseJson = nlohmann::json::parse(res.body);
    BuildingDTO responseDTO = responseJson.get<BuildingDTO>();
    Logger::info("Response DTO: " + nlohmann::json(responseJson).dump());
    EXPECT_TRUE(responseDTO == returnedDTO);
}

TEST_F(BuildingControllerTests, GetBuildingByIdReturns404WhenNotFound) {
    // Arrange
    constexpr int buildingId = 0;

    EXPECT_CALL(mockService, getBuildingById(buildingId)).WillOnce(testing::Return(std::nullopt));

    // Act
    crow::response res = controller.getById(buildingId);

    // Assert
    EXPECT_EQ(res.code, 404);
}