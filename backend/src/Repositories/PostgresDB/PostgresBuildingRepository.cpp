#include "PostgresBuildingRepository.h"

#include <pqxx/pqxx>

#include "../../Utils/Logger.h"

// -------------------------------------------------------------------
// Row mapper (file-local, not exposed in the header)
//
// WHY: Both findById and findAll need to convert a DB row into a
// Building. Extracting this avoids repeating the column names twice.
//
// WHY file-local static and not a member: it uses pqxx::row, which
// we don't want leaking into the public header. Keeping it here means
// consumers of the header don't need to include pqxx.
// -------------------------------------------------------------------
static Building rowToBuilding(const pqxx::row& row) {
    return Building(
        row["id"].as<int>(),
        row["name"].as<std::string>(),
        row["address"].as<std::string>(),
        row["number_of_floors"].as<int>()
    );
}

PostgresBuildingRepository::PostgresBuildingRepository(std::shared_ptr<IDbConnectionFactory> factory)
    : m_factory(std::move(factory)) {}

// -------------------------------------------------------------------
// save
//
// Why "INSERT ... RETURNING id" instead of INSERT then SELECT?
//   PostgreSQL generates the id with GENERATED ALWAYS AS IDENTITY.
//   RETURNING lets us get that id in the same round-trip — no extra
//   SELECT needed.
//
// Why we don't pass 'id' from the domain object:
//   The DB owns the id. The in-memory repo assigned ids itself
//   (nextId++), but Postgres does that automatically. We only pass
//   the non-id columns.
// -------------------------------------------------------------------
int PostgresBuildingRepository::save(const Building& building) {
    auto session = m_factory->createSession();
    auto result = session->execParams(
        "INSERT INTO Buildings (name, address, number_of_floors) VALUES ($1, $2, $3) RETURNING id",
        {building.getName(), building.getAddress(), std::to_string(building.getNumberOfFloors())}
    );
    session->commit();

    const int id = result[0]["id"].as<int>();
    Logger::info("[PostgresBuildingRepository::save] Building saved with id: " + std::to_string(id));
    return id;
}

// -------------------------------------------------------------------
// findById
//
// Why std::optional?
//   The building might not exist. Returning nullopt is the explicit,
//   type-safe way to signal "not found" — no null pointers, no
//   throwing exceptions for a normal "not found" case.
//
// Why result.empty()?
//   A SELECT WHERE id=X returns 0 rows if not found, 1 if found.
//   We check before accessing result[0] to avoid out-of-range access.
// -------------------------------------------------------------------
std::optional<Building> PostgresBuildingRepository::findById(int id) {
    auto session = m_factory->createSession();
    auto result = session->execParams(
        "SELECT id, name, address, number_of_floors FROM Buildings WHERE id = $1",
        std::to_string(id)
    );

    if (result.empty()) {
        Logger::info("[PostgresBuildingRepository::findById] Building not found with id: " + std::to_string(id));
        return std::nullopt;
    }

    Logger::info("[PostgresBuildingRepository::findById] Building found with id: " + std::to_string(id));
    return rowToBuilding(result[0]);
}

// -------------------------------------------------------------------
// findAll
//
// No params needed — we select everything. exec() (no params) is used
// instead of execParams. No commit needed since we're only reading.
// -------------------------------------------------------------------
std::vector<Building> PostgresBuildingRepository::findAll() {
    auto session = m_factory->createSession();
    auto result = session->exec("SELECT id, name, address, number_of_floors FROM Buildings");

    std::vector<Building> buildings;
    for (const auto& row : result) {
        buildings.push_back(rowToBuilding(row));
    }

    Logger::info("[PostgresBuildingRepository::findAll] Found " + std::to_string(buildings.size()) + " buildings");
    return buildings;
}

// -------------------------------------------------------------------
// update
//
// Why updated_at=CURRENT_TIMESTAMP in SQL instead of using the domain
// object's updatedAt value?
//   The DB timestamp is the authoritative "wall clock" for when the
//   record changed in the DB — avoids clock skew between app and DB.
//
// Why is id the LAST parameter ($4)?
//   It goes in the WHERE clause. Convention: bind columns first,
//   then the filter. Makes it easy to match $N to column order.
// -------------------------------------------------------------------
void PostgresBuildingRepository::update(const Building& building) {
    auto session = m_factory->createSession();
    session->execParams(
        "UPDATE Buildings SET name=$1, address=$2, number_of_floors=$3, updated_at=CURRENT_TIMESTAMP WHERE id=$4",
        {building.getName(), building.getAddress(),
         std::to_string(building.getNumberOfFloors()), std::to_string(building.getId())}
    );
    session->commit();
    Logger::info("[PostgresBuildingRepository::update] Building updated with id: " + std::to_string(building.getId()));
}

// -------------------------------------------------------------------
// remove
//
// Single param → use the single-string execParams overload.
// -------------------------------------------------------------------
void PostgresBuildingRepository::remove(int id) {
    auto session = m_factory->createSession();
    session->execParams("DELETE FROM Buildings WHERE id = $1", std::to_string(id));
    session->commit();
    Logger::info("[PostgresBuildingRepository::remove] Building removed with id: " + std::to_string(id));
}
