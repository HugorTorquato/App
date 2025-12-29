#include <gtest/gtest.h>

#include "../../../src/DTOs/Mappers/BuildingDTOMapper.h"
#include "../../../src/Repositories/inmemory/InMemoryBuildingRepository.h"
#include "../../../src/Services/Building/BuildingService.h"

class BuildingServiceTests : public ::testing::Test {
   protected:
    InMemoryBuildingRepository repository;
    BuildingService service;

    Building B1;
    Building B2;

    BuildingServiceTests()
        : service(repository),
          B1(0, "Building 1", "Building 1 Address", 100, {}),
          B2(1, "Building 2", "Building 2 Address", 200, {}) {}
};

TEST_F(BuildingServiceTests, CreateBuildingStoresBuildingInRepository) {
    BuildingDTO dto1;
    dto1.id = 0;
    dto1.name = "Building 1";
    dto1.address = "Building 1 Address";
    dto1.total_floors = 100;
    dto1.apartments = {};

    BuildingDTO dto2;
    dto2.id = 1;
    dto2.name = "Building 2";
    dto2.address = "Building 2 Address";
    dto2.total_floors = 200;
    dto2.apartments = {};

    auto id1 = service.createBuilding(dto1);
    auto id2 = service.createBuilding(dto2);

    EXPECT_EQ(id1, 0);
    EXPECT_EQ(id2, 1);

    auto allBuildings = repository.findAll();
    EXPECT_EQ(allBuildings.size(), 2);

    EXPECT_EQ(allBuildings[0].getName(), "Building 1");
    EXPECT_EQ(allBuildings[1].getName(), "Building 2");
}

TEST_F(BuildingServiceTests, GetBuildingByIdReturnsCorrectBuilding) {
    BuildingDTO dto1;
    dto1.id = 0;
    dto1.name = "Building 1";
    dto1.address = "Building 1 Address";
    dto1.total_floors = 100;
    dto1.apartments = {};

    BuildingDTO dto2;
    dto2.id = 1;
    dto2.name = "Building 2";
    dto2.address = "Building 2 Address";
    dto2.total_floors = 200;
    dto2.apartments = {};

    auto id1 = service.createBuilding(dto1);
    auto id2 = service.createBuilding(dto2);

    auto fetchedBuilding1Opt = service.getBuildingById(id1);
    ASSERT_TRUE(fetchedBuilding1Opt.has_value());
    auto fetchedBuilding1DTO = fetchedBuilding1Opt.value();
    EXPECT_EQ(fetchedBuilding1DTO.name, "Building 1");

    auto fetchedBuilding2Opt = service.getBuildingById(id2);
    ASSERT_TRUE(fetchedBuilding2Opt.has_value());
    auto fetchedBuilding2DTO = fetchedBuilding2Opt.value();
    EXPECT_EQ(fetchedBuilding2DTO.name, "Building 2");
}

TEST_F(BuildingServiceTests, GetBuildingByIdReturnsNulloptForNonExistentId) {
    BuildingDTO dto1;
    dto1.id = 0;
    dto1.name = "Building 1";
    dto1.address = "Building 1 Address";
    dto1.total_floors = 100;
    dto1.apartments = {};

    BuildingDTO dto2;
    dto2.id = 1;
    dto2.name = "Building 2";
    dto2.address = "Building 2 Address";
    dto2.total_floors = 200;
    dto2.apartments = {};

    auto id1 = service.createBuilding(dto1);
    auto id2 = service.createBuilding(dto2);

    constexpr int nonExistentId = 9999;
    auto fetchedBuildingOpt = service.getBuildingById(nonExistentId);

    EXPECT_FALSE(fetchedBuildingOpt.has_value());
    EXPECT_FALSE(fetchedBuildingOpt);
}

TEST_F(BuildingServiceTests, CreateMultipleBuildingsAndFetchThem) {
    const int numBuildings = 5;
    for (int i = 0; i < numBuildings; ++i) {
        BuildingDTO dto;
        dto.id = i;
        dto.name = "Building " + std::to_string(i);
        dto.address = "Address " + std::to_string(i);
        dto.total_floors = 50 + (i * 10);
        dto.apartments = {};
        service.createBuilding(dto);
    }

    for (int i = 0; i < numBuildings; ++i) {
        auto fetchedBuildingOpt = service.getBuildingById(i);
        ASSERT_TRUE(fetchedBuildingOpt.has_value());
        auto fetchedBuildingDTO = fetchedBuildingOpt.value();
        EXPECT_EQ(fetchedBuildingDTO.name, "Building " + std::to_string(i));
        EXPECT_EQ(fetchedBuildingDTO.address, "Address " + std::to_string(i));
        EXPECT_EQ(fetchedBuildingDTO.total_floors, 50 + (i * 10));
    }
}

TEST_F(BuildingServiceTests, CreateResidentStoresResidentInRepository) {
    auto id1 = service.createBuilding(BuildingDTOMapper::toDTO(B1));
    auto id2 = service.createBuilding(BuildingDTOMapper::toDTO(B2));

    EXPECT_EQ(id1, 0);
    EXPECT_EQ(id2, 1);

    auto allBuildings = repository.findAll();
    EXPECT_EQ(allBuildings.size(), 2);
    EXPECT_EQ(allBuildings[0].getName(), B1.getName());
    EXPECT_EQ(allBuildings[1].getName(), B2.getName());
}

TEST_F(BuildingServiceTests, GetBuildingByIdReturnsCorrectData) {
    auto id1 = service.createBuilding(BuildingDTOMapper::toDTO(B1));
    auto id2 = service.createBuilding(BuildingDTOMapper::toDTO(B2));

    auto fetchedBuilding1Opt = service.getBuildingById(id1);
    ASSERT_TRUE(fetchedBuilding1Opt.has_value());
    auto fetchedBuilding1DTO = fetchedBuilding1Opt.value();
    EXPECT_EQ(fetchedBuilding1DTO.name, B1.getName());
    EXPECT_EQ(fetchedBuilding1DTO.address, B1.getAddress());
    EXPECT_EQ(fetchedBuilding1DTO.total_floors, B1.getNumberOfFloors());

    auto fetchedBuilding2Opt = service.getBuildingById(id2);
    ASSERT_TRUE(fetchedBuilding2Opt.has_value());
    auto fetchedBuilding2DTO = fetchedBuilding2Opt.value();
    EXPECT_EQ(fetchedBuilding2DTO.name, B2.getName());
    EXPECT_EQ(fetchedBuilding2DTO.address, B2.getAddress());
    EXPECT_EQ(fetchedBuilding2DTO.total_floors, B2.getNumberOfFloors());
}

TEST_F(BuildingServiceTests, GetBuildingByIdNonExistentIdReturnsNullopt) {
    auto id1 = service.createBuilding(BuildingDTOMapper::toDTO(B1));
    auto id2 = service.createBuilding(BuildingDTOMapper::toDTO(B2));

    constexpr int nonExistentId = 9999;
    auto fetchedBuildingOpt = service.getBuildingById(nonExistentId);

    EXPECT_FALSE(fetchedBuildingOpt.has_value());
    EXPECT_FALSE(fetchedBuildingOpt);
}
