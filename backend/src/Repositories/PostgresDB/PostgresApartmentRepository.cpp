#include "PostgresApartmentRepository.h"

int PostgresApartmentRepository::save(const Apartment& apartment) {
    // Implementation for saving an apartment to PostgreSQL database
    return 0;  // Placeholder return
}

std::optional<Apartment> PostgresApartmentRepository::findById(int id) {
    // Implementation for finding an apartment by ID from PostgreSQL database
    return std::nullopt;  // Placeholder return
}

std::vector<Apartment> PostgresApartmentRepository::findAll() {
    // Implementation for finding all apartments from PostgreSQL database
    return {};  // Placeholder return
}

void PostgresApartmentRepository::update(const Apartment& apartment) {
    // Implementation for updating an apartment in PostgreSQL database
}

void PostgresApartmentRepository::remove(int id) {
    // Implementation for removing an apartment from PostgreSQL database
}
