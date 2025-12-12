# ======================
#  DOMAIN LOGIC (APP CORE)
# ======================
# The 'Domains' folder contains all domain-specific logic (e.g., building, apartment).
# We add all the source files here to the 'app_core' library, which is built in the parent directory.

# List all source files in the 'Domains' directory
set(DOMAIN_SOURCES
    building.cpp
    # Add other domain source files here (e.g., apartment.cpp, resident.cpp, etc.)
)

# ======================
#  ADD DOMAIN SOURCES TO app_core
# ======================
# Add the domain source files to the 'app_core' static library (defined in the parent directory).
target_sources(app_core PRIVATE ${DOMAIN_SOURCES})

# ======================
#  INCLUDE DIRECTORY
# ======================
# Tell the app_core library where to find the header files for the domains.
target_include_directories(app_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}  # Current directory is where domain headers are located
)

