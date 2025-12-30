#include <gmock/gmock.h>

#include "../../../src/Controllers/MaintenanceRequestController.h"
#include "../../Services/MaintenanceRequest/IMaintenanceRequestService.h"
#include "../Utils/DomainEnums.h"
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

class MockMaintenanceRequestService : public IMaintenanceRequestService {
   public:
    MOCK_METHOD(int, createMaintenanceRequest, (const MaintenanceRequestDTO& MaintenanceRequestDTO), (override));
    MOCK_METHOD(std::optional<MaintenanceRequestDTO>, getMaintenanceRequestById, (int id), (override));
};

class MaintenanceRequestControllerTests : public ::testing::Test {
   protected:
    // I'm not testing anything from this logic. Use mock to make this unit test self contained
    MockMaintenanceRequestService mockService;
    MaintenanceRequestController controller{mockService};
};

TEST_F(MaintenanceRequestControllerTests, CreateMaintenanceRequestReturns201AndMaintenanceRequestJsonOnSuccess) {
    // Arrange
    constexpr int expectedMaintenanceRequestId = 0;
    MaintenanceRequestDTO inputDTO{expectedMaintenanceRequestId, 0, 0, "Fix leaky faucet", MaintenanceStatus::Open, 3};
    MaintenanceRequestDTO returnedDTO = inputDTO;

    nlohmann::json requestJson = inputDTO;
    crow::request req;
    req.body = requestJson.dump();

    EXPECT_CALL(mockService, createMaintenanceRequest(testing::Eq(inputDTO))).WillOnce(testing::Return(0));

    // Act
    crow::response res = controller.create(req);

    // Assert
    EXPECT_EQ(res.code, 201);
    Logger::info("Response DTO: " + res.body);
    EXPECT_EQ(res.body, nlohmann::json({{"id", expectedMaintenanceRequestId}}).dump());
}

TEST_F(MaintenanceRequestControllerTests, GetMaintenanceRequestByIdReturns200AndMaintenanceRequestJsonOnSuccess) {
    // Arrange
    constexpr int maintenanceRequestId = 0;
    MaintenanceRequestDTO returnedDTO{maintenanceRequestId, 0, 0, "Fix leaky faucet", MaintenanceStatus::Open, 3};

    EXPECT_CALL(mockService, getMaintenanceRequestById(maintenanceRequestId))
        .WillOnce(testing::Return(std::optional<MaintenanceRequestDTO>(returnedDTO)));

    // Act
    crow::response res = controller.getById(maintenanceRequestId);

    // Assert
    EXPECT_EQ(res.code, 200);
    nlohmann::json responseJson = nlohmann::json::parse(res.body);
    MaintenanceRequestDTO responseDTO = responseJson.get<MaintenanceRequestDTO>();
    Logger::info("Response DTO: " + nlohmann::json(responseJson).dump());
    EXPECT_TRUE(responseDTO == returnedDTO);
}

TEST_F(MaintenanceRequestControllerTests, GetMaintenanceRequestByIdReturns404WhenNotFound) {
    // Arrange
    constexpr int maintenanceRequestId = 0;

    EXPECT_CALL(mockService, getMaintenanceRequestById(maintenanceRequestId)).WillOnce(testing::Return(std::nullopt));
    // Act
    crow::response res = controller.getById(maintenanceRequestId);

    // Assert
    EXPECT_EQ(res.code, 404);
}