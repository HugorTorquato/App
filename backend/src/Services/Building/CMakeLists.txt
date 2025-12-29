# ======================
#  DOMAIN LOGIC (APP CORE)
# ======================
# The 'Domains' folder contains all domain-specific logic (e.g., building, apartment).
# We add all the source files here to the 'domain' library, which is built in the parent directory.

# List all source files in the 'Domains' directory
set(DOMAIN_SOURCES
    BuildingService.cpp
    # Add other domain source files here
)

# ======================
#  ADD DOMAIN SOURCES TO domain
# ======================
# Add the domain source files to the 'domain' static library (defined in the parent directory).
target_sources(domain PRIVATE ${DOMAIN_SOURCES})

# ======================
#  INCLUDE DIRECTORY
# ======================
# Tell the domain library where to find the header files for the domains.
target_include_directories(domain INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}  # Current directory is where domain headers are located
)

