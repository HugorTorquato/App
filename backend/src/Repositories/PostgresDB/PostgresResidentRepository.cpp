#include "PostgresResidentRepository.h"

int PostgresResidentRepository::save(const Resident& resident) {
    // Implementation for saving a resident to PostgreSQL database
    return 0;  // Placeholder return
}

std::optional<Resident> PostgresResidentRepository::findById(int id) {
    // Implementation for finding a resident by ID from PostgreSQL database
    return std::nullopt;  // Placeholder return
}

std::vector<Resident> PostgresResidentRepository::findAll() {
    // Implementation for finding all residents from PostgreSQL database
    return {};  // Placeholder return
}

void PostgresResidentRepository::update(const Resident& resident) {
    // Implementation for updating a resident in PostgreSQL database
}

void PostgresResidentRepository::remove(int id) {
    // Implementation for removing a resident from PostgreSQL database
}
