# ======================
#  TESTS EXECUTABLE
# ======================
# Define the executable that will run all your unit tests
add_executable(runTests
    test_hello.cpp
    Utils/test_logger.cpp
    Utils/test_ApiError.cpp
    Utils/test_Env.cpp
    DTOs/test_building_dto.cpp
    DTOs/test_resident_dto.cpp
    DTOs/test_apartment_dto.cpp
    DTOs/test_maintenanceRequest_dto.cpp
    DTOs/Mappers/test_ResidentDTOMapper_dto.cpp
    DTOs/Mappers/test_ApartmentDTOMapper_dto.cpp
    DTOs/Mappers/test_BuildingDTOMapper_dto.cpp
    DTOs/Mappers/test_maintenanceRequestDTOMapper.cpp
    DTOs/requests/test_create_building_request.cpp
    DTOs/responses/test_building_response.cpp
    Domains/test_building_domain.cpp
    Domains/test_apartment_domain.cpp
    Domains/test_resident_domain.cpp
    Domains/test_maintenanceRequest_domain.cpp
    Repositories/InMemory/test_InMemoryResidentRepository.cpp
    Repositories/InMemory/test_InMemoryApartmentRepository.cpp
    Repositories/InMemory/test_InMemoryBuildingRepository.cpp
    Repositories/InMemory/test_InMemoryMaintenanceRequestRepository.cpp
    Services/test_ServiceResident.cpp
    Services/test_ServiceApartment.cpp
    Services/test_ServiceBuilding.cpp
    Services/test_ServiceMantananceRequest.cpp
    Controllers/test_resident_controller.cpp
    Controllers/test_apartment_controller.cpp
    Controllers/test_building_controller.cpp
    Controllers/test_maintenanceRequest_controller.cpp
    Composition/test_resident_composition.cpp
    Composition/test_apartment_composition.cpp
    Composition/test_building_composition.cpp
    Composition/test_maintenanceRequest_composition.cpp
)

# ======================
#  LINK TESTS WITH GOOGLE TEST AND APP CORE
# ======================
# Link to GoogleTest (which is already fetched via FetchContent)
# Link the 'domain' library to allow unit tests to access your domain logic
target_link_libraries(runTests PRIVATE
    gtest_main  # GoogleTest main library
    gmock_main  # GoogleMock main library
    domain    # Your core logic library
)

# ======================
#  GOOGLE TEST AUTOMATIC TEST DISCOVERY
# ======================
# Automatically discovers and registers all the tests in 'runTests'
include(GoogleTest)
gtest_discover_tests(runTests)

