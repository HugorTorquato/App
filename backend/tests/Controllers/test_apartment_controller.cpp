#include <gmock/gmock.h>

#include "../../../src/Controllers/ApartmentController.h"
#include "../../Services/Apartment/IApartmentService.h"
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

class MockApartmentService : public IApartmentService {
   public:
    MOCK_METHOD(int, createApartment, (const ApartmentDTO& apartmentDTO), (override));
    MOCK_METHOD(std::optional<ApartmentDTO>, getApartmentById, (int id), (override));
};

class ApartmentControllerTests : public ::testing::Test {
   protected:
    // I'm not testing anything from this logic. Use mock to make this unit test self contained
    MockApartmentService mockService;
    ApartmentController controller{mockService};
};

TEST_F(ApartmentControllerTests, CreateApartmentReturns201AndApartmentJsonOnSuccess) {
    // Arrange
    constexpr int expectedApartmentId = 0;
    ApartmentDTO inputDTO{expectedApartmentId, "A101", 10.5};
    ApartmentDTO returnedDTO = inputDTO;

    nlohmann::json requestJson = inputDTO;
    crow::request req;
    req.body = requestJson.dump();

    EXPECT_CALL(mockService, createApartment(testing::Eq(inputDTO))).WillOnce(testing::Return(0));

    // Act
    crow::response res = controller.create(req);

    // Assert
    EXPECT_EQ(res.code, 201);
    Logger::info("Response DTO: " + res.body);
    EXPECT_EQ(res.body, nlohmann::json({{"id", expectedApartmentId}}).dump());
}

TEST_F(ApartmentControllerTests, GetApartmentByIdReturns200AndApartmentJsonOnSuccess) {
    // Arrange
    constexpr int apartmentId = 0;
    ApartmentDTO returnedDTO{apartmentId, "A101", 10.5};

    EXPECT_CALL(mockService, getApartmentById(apartmentId)).WillOnce(testing::Return(returnedDTO));

    // Act
    crow::response res = controller.getById(apartmentId);

    // Assert
    EXPECT_EQ(res.code, 200);
    nlohmann::json responseJson = nlohmann::json::parse(res.body);
    ApartmentDTO responseDTO = responseJson.get<ApartmentDTO>();
    EXPECT_TRUE(responseDTO == returnedDTO);
}

TEST_F(ApartmentControllerTests, GetApartmentByIdReturns404WhenNotFound) {
    // Arrange
    constexpr int apartmentId = 0;

    EXPECT_CALL(mockService, getApartmentById(apartmentId)).WillOnce(testing::Return(std::nullopt));

    // Act
    crow::response res = controller.getById(apartmentId);

    // Assert
    EXPECT_EQ(res.code, 404);
}
