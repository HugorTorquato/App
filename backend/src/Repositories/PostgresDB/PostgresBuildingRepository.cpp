#include "PostgresBuildingRepository.h"

#include "../../Utils/Logger.h"

// Private
Building PostgresBuildingRepository::mapRowToBuilding(const pqxx::row& row) {
    int id = row["id"].as<int>();
    std::string name = row["name"].as<std::string>();
    std::string address = row["address"].as<std::string>();
    int numFloors = row["num_floors"].as<int>();

    return Building(id, name, address, numFloors, std::vector<Apartment>{});
}

PostgresBuildingRepository::PostgresBuildingRepository(std::unique_ptr<IDbConnectionFactory> factory)
    : m_factory(std::move(factory)) {}

int PostgresBuildingRepository::save(const Building& building) {
    Logger::info(__PRETTY_FUNCTION__ + std::string(" Saving building: '") + building.getName() + "' at '" +
                 building.getAddress() + "'");

    auto session = m_factory->createSession();
    auto result =
        session->execParams("INSERT INTO buildings (name, address, num_floors) VALUES ($1, $2, $3) RETURNING id;",
                            {building.getName(), building.getAddress(), std::to_string(building.getNumberOfFloors())});

    if (result.empty()) throw std::runtime_error("save(): INSERT returned no rows — database did not assign an id");

    int id = result[0]["id"].as<int>();
    session->commit();
    Logger::info(__PRETTY_FUNCTION__ + std::string(" Building saved successfully with id: ") + std::to_string(id));
    return id;
}

std::optional<Building> PostgresBuildingRepository::findById(int id) {
    Logger::info(__PRETTY_FUNCTION__ + std::string(" Looking up building with id: ") + std::to_string(id));

    auto session = m_factory->createSession();
    auto result = session->execParams("SELECT id, name, address, num_floors FROM buildings WHERE id = $1;",
                                      std::vector<std::string>{std::to_string(id)});

    if (result.empty()) {
        Logger::info(__PRETTY_FUNCTION__ + std::string(" No building found with id: ") + std::to_string(id));
        return std::nullopt;
    }

    auto building = mapRowToBuilding(result[0]);
    Logger::info(__PRETTY_FUNCTION__ + std::string(" Found building: '") + building.getName() + "' at '" +
                 building.getAddress() + "'");
    return std::make_optional(building);
}

std::vector<Building> PostgresBuildingRepository::findAll() {
    Logger::info(__PRETTY_FUNCTION__ + std::string(" Fetching all buildings"));

    auto session = m_factory->createSession();
    auto result = session->exec("SELECT id, name, address, num_floors FROM buildings;");

    std::vector<Building> buildings;
    for (const auto& row : result) {
        auto building = mapRowToBuilding(row);
        Logger::info(__PRETTY_FUNCTION__ + std::string(" Row mapped: '") + building.getName() + "' at '" +
                     building.getAddress() + "' (" + std::to_string(building.getNumberOfFloors()) + " floors)");
        buildings.push_back(building);
    }

    Logger::info(__PRETTY_FUNCTION__ + std::string(" Returning ") + std::to_string(buildings.size()) + " building(s)");
    return buildings;
}

void PostgresBuildingRepository::update(const Building& building) {
    Logger::info(__PRETTY_FUNCTION__ + std::string(" Updating building id: ") + std::to_string(building.getId()) +
                 " — new name: '" + building.getName() + "'");

    auto session = m_factory->createSession();
    session->execParams("UPDATE buildings SET name = $1, address = $2, num_floors = $3 WHERE id = $4 RETURNING id;",
                        {building.getName(), building.getAddress(), std::to_string(building.getNumberOfFloors()),
                         std::to_string(building.getId())});
    session->commit();
    Logger::info(__PRETTY_FUNCTION__ + std::string(" Building id: ") + std::to_string(building.getId()) +
                 " updated successfully");
}

void PostgresBuildingRepository::remove(int id) {
    // TODO: ID can be 0? or should i scape?
    Logger::info(__PRETTY_FUNCTION__ + std::string(" Removing building with id: ") + std::to_string(id));

    auto session = m_factory->createSession();
    session->execParams("DELETE FROM buildings WHERE id = $1;", std::vector<std::string>{std::to_string(id)});
    session->commit();
    Logger::info(__PRETTY_FUNCTION__ + std::string(" Building id: ") + std::to_string(id) + " removed successfully");
}
