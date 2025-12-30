#include <gmock/gmock.h>

#include "../../../src/Controllers/ResidentController.h"
#include "../../Services//Resident/IResidentService.h"
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

class MockResidentService : public IResidentService {
   public:
    MOCK_METHOD(int, createResident, (const ResidentDTO& residentDTO), (override));
    MOCK_METHOD(std::optional<ResidentDTO>, getResidentById, (int id), (override));
    MOCK_METHOD(void, updateMoveOutResident, (int residentId, int moveOutDate), (override));
};

class ResidentControllerTests : public ::testing::Test {
   protected:
    // I'm not testing anything from this logic. Use mock to make this unit test self contained
    MockResidentService mockService;
    ResidentController controller{mockService};
};

TEST_F(ResidentControllerTests, CreateResidentReturns201AndResidentJsonOnSuccess) {
    // Arrange
    constexpr int expectedResidentId = 0;
    ResidentDTO inputDTO{expectedResidentId, "John Doe", "123-456-7890", "101"};
    ResidentDTO returnedDTO = inputDTO;

    nlohmann::json requestJson = inputDTO;
    crow::request req;
    req.body = requestJson.dump();

    EXPECT_CALL(mockService, createResident(testing::Eq(inputDTO))).WillOnce(testing::Return(0));

    // Act
    crow::response res = controller.create(req);

    // Assert
    EXPECT_EQ(res.code, 201);
    Logger::info("Response DTO: " + res.body);
    EXPECT_EQ(res.body, nlohmann::json({"id", expectedResidentId}).dump());

    // TODO: I would be able to do this if i had returned the DTO, but it's only an integer ID
    // ResidentDTO responseDTO = responseJson.get<ResidentDTO>();
    // Logger::info("Response DTO: " + nlohmann::json(responseJson).dump());
    // EXPECT_TRUE(responseDTO == returnedDTO);
}

TEST_F(ResidentControllerTests, GetByIdReturns200AndResidentJsonWhenFound) {
    // Arrange
    constexpr int residentId = 0;
    ResidentDTO expectedDTO{residentId, "John Doe", "123-456-7890", "101"};

    EXPECT_CALL(mockService, getResidentById(residentId)).WillOnce(testing::Return(expectedDTO));

    // Act
    crow::response res = controller.getById(residentId);

    // Assert
    EXPECT_EQ(res.code, 200);
    nlohmann::json responseJson = nlohmann::json::parse(res.body);
    ResidentDTO responseDTO = responseJson.get<ResidentDTO>();
    Logger::info("Response DTO: " + nlohmann::json(responseJson).dump());
    EXPECT_TRUE(responseDTO == expectedDTO);
}

TEST_F(ResidentControllerTests, GetByIdReturns404WhenNotFound) {
    // Arrange
    constexpr int residentId = 0;

    EXPECT_CALL(mockService, getResidentById(residentId)).WillOnce(testing::Return(std::nullopt));

    // Act
    crow::response res = controller.getById(residentId);

    // Assert
    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Resident not found");
}
