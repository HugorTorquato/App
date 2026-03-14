#include "PostgresBuildingRepository.h"

#include "../../Utils/Logger.h"

int PostgresBuildingRepository::save(const Building& building) {
    // Implementation for saving a building to PostgreSQL database
    int id = nextId++;
    Logger::info(__PRETTY_FUNCTION__ + std::string("Building saved with id: " + std::to_string(id)));
    return id;
}

std::optional<Building> PostgresBuildingRepository::findById(int id) {
    // Implementation for finding a building by ID from PostgreSQL database
    return std::nullopt;  // Placeholder return
}

std::vector<Building> PostgresBuildingRepository::findAll() {
    // Implementation for finding all buildings from PostgreSQL database
    return {};  // Placeholder return
}

void PostgresBuildingRepository::update(const Building& building) {
    // Implementation for updating a building in PostgreSQL database
}

void PostgresBuildingRepository::remove(int id) {
    // Implementation for removing a building from PostgreSQL database
}
