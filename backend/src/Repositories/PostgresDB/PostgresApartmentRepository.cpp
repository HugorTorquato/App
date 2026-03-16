#include "PostgresApartmentRepository.h"

#include <pqxx/pqxx>

#include "../../Utils/Logger.h"

// -------------------------------------------------------------------
// Why bool → "true"/"false" string?
//   Our execParams vector carries std::string values. PostgreSQL
//   accepts the literals "true"/"false" for BOOLEAN columns. pqxx
//   will handle the type coercion on the server side.
// -------------------------------------------------------------------
static std::string boolToStr(bool b) { return b ? "true" : "false"; }

static Apartment rowToApartment(const pqxx::row& row) {
    return Apartment(
        row["id"].as<int>(),
        row["building_id"].as<int>(),
        row["number"].as<std::string>(),
        row["is_occupied"].as<bool>(),
        row["pet_allowed"].as<bool>(),
        row["condo_fee"].as<double>(),
        row["area_m2"].as<double>()
    );
}

PostgresApartmentRepository::PostgresApartmentRepository(std::shared_ptr<IDbConnectionFactory> factory)
    : m_factory(std::move(factory)) {}

int PostgresApartmentRepository::save(const Apartment& apartment) {
    auto session = m_factory->createSession();
    auto result = session->execParams(
        "INSERT INTO Apartments (building_id, number, is_occupied, pet_allowed, condo_fee, area_m2)"
        " VALUES ($1, $2, $3, $4, $5, $6) RETURNING id",
        {std::to_string(apartment.getBuildingId()),
         apartment.getNumber(),
         boolToStr(apartment.getIsOccupied()),
         boolToStr(apartment.getPetAllowed()),
         std::to_string(apartment.getCondoFee()),
         std::to_string(apartment.getAreaM2())}
    );
    session->commit();

    const int id = result[0]["id"].as<int>();
    Logger::info("[PostgresApartmentRepository::save] Apartment saved with id: " + std::to_string(id));
    return id;
}

std::optional<Apartment> PostgresApartmentRepository::findById(int id) {
    auto session = m_factory->createSession();
    auto result = session->execParams(
        "SELECT id, building_id, number, is_occupied, pet_allowed, condo_fee, area_m2"
        " FROM Apartments WHERE id = $1",
        std::to_string(id)
    );

    if (result.empty()) {
        Logger::info("[PostgresApartmentRepository::findById] Apartment not found with id: " + std::to_string(id));
        return std::nullopt;
    }

    Logger::info("[PostgresApartmentRepository::findById] Apartment found with id: " + std::to_string(id));
    return rowToApartment(result[0]);
}

std::vector<Apartment> PostgresApartmentRepository::findAll() {
    auto session = m_factory->createSession();
    auto result = session->exec(
        "SELECT id, building_id, number, is_occupied, pet_allowed, condo_fee, area_m2 FROM Apartments"
    );

    std::vector<Apartment> apartments;
    for (const auto& row : result) {
        apartments.push_back(rowToApartment(row));
    }

    Logger::info("[PostgresApartmentRepository::findAll] Found " + std::to_string(apartments.size()) + " apartments");
    return apartments;
}

void PostgresApartmentRepository::update(const Apartment& apartment) {
    auto session = m_factory->createSession();
    session->execParams(
        "UPDATE Apartments SET building_id=$1, number=$2, is_occupied=$3, pet_allowed=$4,"
        " condo_fee=$5, area_m2=$6, updated_at=CURRENT_TIMESTAMP WHERE id=$7",
        {std::to_string(apartment.getBuildingId()),
         apartment.getNumber(),
         boolToStr(apartment.getIsOccupied()),
         boolToStr(apartment.getPetAllowed()),
         std::to_string(apartment.getCondoFee()),
         std::to_string(apartment.getAreaM2()),
         std::to_string(apartment.getId())}
    );
    session->commit();
    Logger::info("[PostgresApartmentRepository::update] Apartment updated with id: " + std::to_string(apartment.getId()));
}

void PostgresApartmentRepository::remove(int id) {
    auto session = m_factory->createSession();
    session->execParams("DELETE FROM Apartments WHERE id = $1", std::to_string(id));
    session->commit();
    Logger::info("[PostgresApartmentRepository::remove] Apartment removed with id: " + std::to_string(id));
}
