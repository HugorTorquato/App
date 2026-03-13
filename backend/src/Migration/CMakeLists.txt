# ======================
#  MIGRATION EXECUTABLE
# ======================
# This executable runs database migrations.

add_subdirectory(Runner)

add_executable(migrate main.cpp)

target_link_libraries(migrate
    domain
    pqxx
    pq
    Boost::system
)