cmake_minimum_required(VERSION 3.16)
project(condo_backend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Crow header-only include
include_directories(${CMAKE_SOURCE_DIR}/vendor/crow/include)

# Find and link Boost + OpenSSL + PostgreSQL client (optional)
find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(PostgreSQL REQUIRED)

add_executable(condo_backend src/main.cpp)

target_include_directories(condo_backend PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
    ${PostgreSQL_INCLUDE_DIRS}
)

target_link_libraries(condo_backend PRIVATE
    ${OPENSSL_LIBRARIES}
    ${PostgreSQL_LIBRARIES}
)
