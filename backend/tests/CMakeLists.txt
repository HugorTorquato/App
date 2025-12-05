# backend/tests/CMakeLists.txt

add_executable(runTests 
    test_hello.cpp
    Utils/test_logger.cpp
    Utils/test_ApiError.cpp
)

target_link_libraries(runTests PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(runTests)