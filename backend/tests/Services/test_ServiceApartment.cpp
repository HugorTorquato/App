#include <gtest/gtest.h>

#include "../../../src/DTOs/Mappers/ApartmentDTOMapper.h"
#include "../../../src/Repositories/inmemory/InMemoryApartmentRepository.h"
#include "../../../src/Services/Apartment/ApartmentService.h"

class ServiceApartmentTest : public ::testing::Test {
   protected:
    InMemoryApartmentRepository repository;
    ApartmentService service;

    Apartment apartment1;
    Apartment apartment2;

    ServiceApartmentTest()
        : service(repository),
          apartment1(0, 1, "101", false, true, 250.0, 75.5),
          apartment2(0, 1, "202", true, false, 300.0, 85.0) {}

    void SetUp() override {}
};

TEST_F(ServiceApartmentTest, CreateApartmentStoresApartmentInRepository) {
    auto id1 = service.createApartment(ApartmentDTOMapper::toDTO(apartment1));
    auto id2 = service.createApartment(ApartmentDTOMapper::toDTO(apartment2));

    EXPECT_EQ(id1, 0);
    EXPECT_EQ(id2, 1);

    auto allApartments = repository.findAll();
    EXPECT_EQ(allApartments.size(), 2);

    EXPECT_EQ(allApartments[0].getNumber(), "101");
    EXPECT_EQ(allApartments[1].getNumber(), "202");
}

TEST_F(ServiceApartmentTest, GetApartmentByIdReturnsCorrectApartment) {
    auto id1 = service.createApartment(ApartmentDTOMapper::toDTO(apartment1));
    auto id2 = service.createApartment(ApartmentDTOMapper::toDTO(apartment2));

    auto fetchedApartment1Opt = service.getApartmentById(id1);
    ASSERT_TRUE(fetchedApartment1Opt.has_value());
    auto fetchedApartment1DTO = fetchedApartment1Opt.value();
    auto fetchedApartment1 = ApartmentDTOMapper::fromDTO(fetchedApartment1DTO);
    EXPECT_EQ(fetchedApartment1.getNumber(), "101");

    auto fetchedApartment2Opt = service.getApartmentById(id2);
    ASSERT_TRUE(fetchedApartment2Opt.has_value());
    auto fetchedApartment2DTO = fetchedApartment2Opt.value();
    auto fetchedApartment2 = ApartmentDTOMapper::fromDTO(fetchedApartment2DTO);
    EXPECT_EQ(fetchedApartment2.getNumber(), "202");
}

TEST_F(ServiceApartmentTest, GetApartmentByIdReturnsNulloptForNonExistentId) {
    auto id1 = service.createApartment(ApartmentDTOMapper::toDTO(apartment1));
    auto id2 = service.createApartment(ApartmentDTOMapper::toDTO(apartment2));

    constexpr int nonExistentId = 9999;
    auto fetchedApartmentOpt = service.getApartmentById(nonExistentId);
    EXPECT_FALSE(fetchedApartmentOpt.has_value());
}

// TODO:
// 1- Remember that is possible to retreave information from the repository directly to validate domain entity fields
// not exposed in the DTO