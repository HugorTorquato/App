cmake_minimum_required(VERSION 3.16)
project(condo_backend VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(ENABLE_COVERAGE "Enable test coverage reporting" OFF)

if(ENABLE_COVERAGE)
    message(STATUS "Building with coverage flags")
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()
# Put output in reasonable folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Crow header-only include
include_directories(${CMAKE_SOURCE_DIR}/../vendor/crow/include)

# Find and link Boost + OpenSSL + PostgreSQL client (optional)
find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(PostgreSQL REQUIRED)

# # If you need pthreads (commonly required on Linux)
# find_package(Threads REQUIRED)
# target_link_libraries(condo_backend PRIVATE Threads::Threads)

add_executable(condo_backend src/main.cpp)

# Useful compile flags (tweak per compiler) - gives warnings and optimization for dev builds.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(condo_backend PRIVATE -Wall -Wextra -Wpedantic -O2)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(condo_backend PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()

target_include_directories(condo_backend PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
    ${PostgreSQL_INCLUDE_DIRS}
)

target_link_libraries(condo_backend PRIVATE
    ${OPENSSL_LIBRARIES}
    ${PostgreSQL_LIBRARIES}
)

# Enable testing
enable_testing()

# Download and configure GoogleTest via FetchContent (modern way)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(tests)