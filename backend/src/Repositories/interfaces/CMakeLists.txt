# Interfaces are header-only
target_include_directories(domain INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# No .cpp → nothing to compile → headers only.
# It just defines the contract