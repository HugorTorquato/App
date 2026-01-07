# ======================
#  ADD Repositories SOURCES TO domain
# ======================
# Add the Repositories source files to the 'domain' static library (defined in the parent directory).

target_sources(domain PRIVATE
    DbConnectionFactory.cpp
    PostgresResidentRepository.cpp
    PostgresApartmentRepository.cpp
    PostgresBuildingRepository.cpp
    PostgresMaintenanceRequestRepository.cpp
)

target_include_directories(domain INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
