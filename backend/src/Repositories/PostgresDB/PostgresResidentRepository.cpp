#include "PostgresResidentRepository.h"

#include "../../Utils/Logger.h"

int PostgresResidentRepository::save(const Resident& resident) {
    // Implementation for saving a resident to PostgreSQL database
    Logger::info(__PRETTY_FUNCTION__ + std::string(" Saving resident to PostgreSQL database: " + resident.getFullName()));
    return 0;  // Placeholder return
}

std::optional<Resident> PostgresResidentRepository::findById(int id) {
    // Implementation for finding a resident by ID from PostgreSQL database
    Logger::info(__PRETTY_FUNCTION__ + std::string(" Finding resident by ID in PostgreSQL database: " + std::to_string(id)));
    return std::nullopt;  // Placeholder return
}

std::vector<Resident> PostgresResidentRepository::findAll() {
    // Implementation for finding all residents from PostgreSQL database
    Logger::info(__PRETTY_FUNCTION__ + std::string(" Finding all residents in PostgreSQL database"));
    return {};  // Placeholder return
}

void PostgresResidentRepository::update(const Resident& resident) {
    // Implementation for updating a resident in PostgreSQL database
    Logger::info(__PRETTY_FUNCTION__ + std::string(" Updating resident in PostgreSQL database: " + resident.getFullName()));
}

void PostgresResidentRepository::remove(int id) {
    // Implementation for removing a resident from PostgreSQL database
    Logger::info(__PRETTY_FUNCTION__ + std::string(" Removing resident from PostgreSQL database with ID: ") + std::to_string(id));
}
