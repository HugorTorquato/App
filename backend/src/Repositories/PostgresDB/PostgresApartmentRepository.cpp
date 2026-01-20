#include "PostgresApartmentRepository.h"

#include "../../Utils/Logger.h"

int PostgresApartmentRepository::save(const Apartment& apartment) {
    // Implementation for saving an apartment to PostgreSQL database
    int id = nextId++;
    Logger::info(__PRETTY_FUNCTION__ + std::string("Apartment saved with id: " + std::to_string(id)));
    return id;
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