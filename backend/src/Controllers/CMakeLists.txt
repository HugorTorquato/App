# ======================
#  ADD Repositories SOURCES TO domain
# ======================
# Add the Repositories source files to the 'domain' static library (defined in the parent directory).

target_sources(domain PRIVATE
    ResidentController.cpp
    BuildingController.cpp
    ApartmentController.cpp
    MaintenanceRequestController.cpp
)

target_include_directories(domain INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(domain PUBLIC
    ${CMAKE_SOURCE_DIR}/../vendor/crow/include
)