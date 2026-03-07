enable_testing()
include(GoogleTest)

file(GLOB_RECURSE TEST_SOURCES
    DTOs/*.cpp
    Domains/*.cpp
    Repositories/*.cpp
    Services/*.cpp
    Controllers/*.cpp
    Composition/*.cpp
    Utils/*.cpp
)

add_executable(runTests
    test_hello.cpp
    ${TEST_SOURCES}
)

target_link_libraries(runTests PRIVATE
    gtest_main
    gmock_main
    domain    # Your core logic library
    pqxx::pqxx
    PostgreSQL::PostgreSQL
)

gtest_discover_tests(runTests)
