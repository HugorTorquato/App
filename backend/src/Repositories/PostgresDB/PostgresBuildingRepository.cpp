#include "PostgresBuildingRepository.h"

#include "../../Utils/Logger.h"

PostgresBuildingRepository::PostgresBuildingRepository(std::unique_ptr<IDbConnectionFactory> factory)
    : m_factory(std::move(factory)) {}

int PostgresBuildingRepository::save(const Building& building) {
    // Implementation for saving a building to PostgreSQL database
    auto session = m_factory->createSession();

    auto result =
        session->execParams("INSERT INTO buildings (name, address, num_floors) VALUES ($1, $2, $3) RETURNING id;",
                            {building.getName(), building.getAddress(), std::to_string(building.getNumberOfFloors())});

    if (result.empty()) throw std::runtime_error("save(): INSERT returned no rows — database did not assign an id");

    int id = result[0]["id"].as<int>();
    session->commit();
    Logger::info(__PRETTY_FUNCTION__ + std::string("Building saved with id: " + std::to_string(id)));
    return id;
}

// Protected
Building PostgresBuildingRepository::mapRowToBuilding(const pqxx::row& row) {
    int id = row["id"].as<int>();
    std::string name = row["name"].as<std::string>();
    std::string address = row["address"].as<std::string>();
    int numFloors = row["num_floors"].as<int>();

    return Building(id, name, address, numFloors, std::vector<Apartment>{});
}

std::optional<Building> PostgresBuildingRepository::findById(int id) {
    // Implementation for finding a building by ID from PostgreSQL database
    return std::nullopt;  // Placeholder return
    // No commit()
}

std::vector<Building> PostgresBuildingRepository::findAll() {
    std::vector<Building> buildings;

    auto session = m_factory->createSession();
    auto result = session->exec("SELECT id, name, address, num_floors FROM buildings;");

    for (const auto& row : result) {
        auto building = mapRowToBuilding(row);
        buildings.push_back(building);
        Logger::info(__PRETTY_FUNCTION__ + std::string("Building found: ") + building.getName() + " at " +
                     building.getAddress() + " with " + std::to_string(building.getNumberOfFloors()) + " floors.");
    }

    return buildings;
    // No commit() - No modification to database
}

void PostgresBuildingRepository::update(const Building& building) {
    // Implementation for updating a building in PostgreSQL database
}

void PostgresBuildingRepository::remove(int id) {
    // Implementation for removing a building from PostgreSQL database
}
