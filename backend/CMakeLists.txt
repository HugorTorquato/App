# ======================
#  ROOT PROJECT SETUP
# ======================
cmake_minimum_required(VERSION 3.16)

project(condo_backend 
    VERSION 0.1
    LANGUAGES CXX
)

# ======================
#  COMPILER SETTINGS
# ======================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ======================
#  BUILD OPTIONS
# ======================
option(ENABLE_COVERAGE "Enable test coverage reporting" OFF)

if(ENABLE_COVERAGE)
    message(STATUS "Building with coverage flags enabled")
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

# ======================
#  DEPENDENCIES
# ======================
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(OpenSSL REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(Threads REQUIRED)

find_package(PQXX QUIET)
if(NOT PQXX_FOUND)
    # message(STATUS "PQXX CMake package not found — falling back to manual lookup")
    find_library(PQXX_LIB pqxx REQUIRED)
endif()

# ======================
#  TESTING
# ======================
enable_testing()
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# ======================
#  SUBDIRECTORIES
# ======================
add_subdirectory(src)
add_subdirectory(tests)

# ======================
#  MAIN EXECUTABLE
# ======================
add_executable(condo_backend src/main.cpp)

# Compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(condo_backend PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()

# Include directories
target_include_directories(condo_backend PRIVATE
    ${CMAKE_SOURCE_DIR}/../vendor/crow/include
)

# Link libraries
target_link_libraries(condo_backend PRIVATE
    domain
    Boost::system
    Boost::thread
    OpenSSL::SSL
    OpenSSL::Crypto
    PostgreSQL::PostgreSQL
    Threads::Threads
    ${PQXX_LIB}
    ${PostgreSQL_LIBRARIES} 
)
