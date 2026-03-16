# ======================
#  ADD Runner SOURCES TO migrate
# ======================
# Add the Runner source files to the 'migrate' static library (defined in the parent directory).

target_sources(domain PRIVATE
    MigrationRunner.cpp
)

target_include_directories(domain INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(domain PUBLIC
    ${CMAKE_SOURCE_DIR}/../vendor/crow/include
)
