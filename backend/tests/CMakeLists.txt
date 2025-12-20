# ======================
#  TESTS EXECUTABLE
# ======================
# Define the executable that will run all your unit tests
add_executable(runTests
    test_hello.cpp
    Utils/test_logger.cpp
    Utils/test_ApiError.cpp
    DTOs/test_building_dto.cpp
    DTOs/test_resident_dto.cpp
    DTOs/requests/test_create_building_request.cpp
    DTOs/responses/test_building_response.cpp
    Domains/test_building_domain.cpp
    Domains/test_apartment_domain.cpp
    Domains/test_resident_domain.cpp
    Domains/test_maintenanceRequest_domain.cpp
    Repositories/InMemory/test_InMemoryResidentRepository.cpp
    Repositories/InMemory/test_InMemoryApartmentRepository.cpp
    Repositories/InMemory/test_InMemoryBuildingRepository.cpp
)

# ======================
#  LINK TESTS WITH GOOGLE TEST AND APP CORE
# ======================
# Link to GoogleTest (which is already fetched via FetchContent)
# Link the 'domain' library to allow unit tests to access your domain logic
target_link_libraries(runTests PRIVATE
    gtest_main  # GoogleTest main library
    domain    # Your core logic library
)

# ======================
#  GOOGLE TEST AUTOMATIC TEST DISCOVERY
# ======================
# Automatically discovers and registers all the tests in 'runTests'
include(GoogleTest)
gtest_discover_tests(runTests)

