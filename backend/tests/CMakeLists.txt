# backend/tests/CMakeLists.txt

add_executable(runTests 
    test_hello.cpp
    Utils/test_logger.cpp
    Utils/test_ApiError.cpp
    DTOs/test_building_dto.cpp
    DTOs/test_resident_dto.cpp
    DTOs/requests/test_create_building_request.cpp
)

target_link_libraries(runTests PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(runTests)