# ======================
#  ROOT PROJECT SETUP
# ======================
cmake_minimum_required(VERSION 3.16)

project(condo_backend 
    VERSION 0.1
    LANGUAGES CXX
)

# Use modern C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)   # Required for static libs used in shared contexts

# Output binaries into /build/bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# ======================
#  CODE COVERAGE (OPTIONAL)
# ======================
option(ENABLE_COVERAGE "Enable test coverage reporting" OFF)

if (ENABLE_COVERAGE)
    message(STATUS "Building with coverage flags enabled")
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()


# ======================
#  DEPENDENCY DISCOVERY
# ======================

# Boost (Header + system + thread)
find_package(Boost REQUIRED COMPONENTS system thread)

# OpenSSL (crypto + ssl)
find_package(OpenSSL REQUIRED)

# PostgreSQL client (libpq)
find_package(PostgreSQL REQUIRED)

# POSIX threads – required by Crow and Boost
find_package(Threads REQUIRED)


# ======================
#  UNIT TEST FRAMEWORK (GoogleTest)
# ======================
enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)


# ======================
#  PROJECT STRUCTURE
# ======================
add_subdirectory(src)      # Builds the domain static library
add_subdirectory(tests)    # Builds unit tests


# ======================
#  MAIN EXECUTABLE
# ======================
add_executable(condo_backend src/Server/main.cpp)

# Compile warnings
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(condo_backend PRIVATE -Wall -Wextra -Wpedantic -O2)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(condo_backend PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()


# ======================
#  INCLUDE PATHS
# ======================

# Crow is header-only
target_include_directories(condo_backend PRIVATE
    ${CMAKE_SOURCE_DIR}/../vendor/crow/include
)


# ======================
#  LINKING
# ======================
target_link_libraries(condo_backend PRIVATE
    domain              # Your domain library

    # Boost
    Boost::system
    Boost::thread

    # OpenSSL
    OpenSSL::SSL
    OpenSSL::Crypto

    # PostgreSQL (libpq)
    PostgreSQL::PostgreSQL

    # Pthreads
    Threads::Threads
)

