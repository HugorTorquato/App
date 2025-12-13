# ======================
#  domain STATIC LIBRARY
# ======================
# This library contains all core domain/business logic.
# It is linked into the main backend executable and into the unit tests.

add_library(domain STATIC)

# Add subdirectories for modular structure
add_subdirectory(Domains)
# add_subdirectory(Services)
# add_subdirectory(Controllers)

# Let users of domain access headers inside src/
# INTERFACE means: consumers get these include dirs, but this library itself does not compile code there.
target_include_directories(domain INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
