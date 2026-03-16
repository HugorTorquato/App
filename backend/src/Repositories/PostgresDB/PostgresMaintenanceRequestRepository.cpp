#include "PostgresMaintenanceRequestRepository.h"

#include <pqxx/pqxx>

#include "../../Utils/Logger.h"

// -------------------------------------------------------------------
// Enum ↔ string conversion
//
// WHY: The DB stores status as TEXT (with a CHECK constraint for valid
// values). C++ uses a strongly-typed enum. The repo is the bridge —
// it converts at the boundary, keeping the domain model clean (no
// strings leaking into business logic) and the DB schema readable
// (no magic integers stored).
// -------------------------------------------------------------------
static std::string statusToStr(MaintenanceStatus s) {
    switch (s) {
        case MaintenanceStatus::InProgress: return "InProgress";
        case MaintenanceStatus::Completed:  return "Completed";
        default:                            return "Open";
    }
}

static MaintenanceStatus strToStatus(const std::string& s) {
    if (s == "InProgress") return MaintenanceStatus::InProgress;
    if (s == "Completed")  return MaintenanceStatus::Completed;
    return MaintenanceStatus::Open;
}

static MaintenanceRequest rowToMaintenanceRequest(const pqxx::row& row) {
    return MaintenanceRequest(
        row["id"].as<int>(),
        row["resident_id"].as<int>(),
        row["apartment_id"].as<int>(),
        row["description"].as<std::string>(),
        strToStatus(row["status"].as<std::string>()),
        row["priority"].as<int>()
    );
}

PostgresMaintenanceRequestRepository::PostgresMaintenanceRequestRepository(
    std::shared_ptr<IDbConnectionFactory> factory)
    : m_factory(std::move(factory)) {}

int PostgresMaintenanceRequestRepository::save(const MaintenanceRequest& req) {
    auto session = m_factory->createSession();
    auto result = session->execParams(
        "INSERT INTO MaintenanceRequests (resident_id, apartment_id, description, status, priority)"
        " VALUES ($1, $2, $3, $4, $5) RETURNING id",
        {std::to_string(req.getResidentId()),
         std::to_string(req.getApartmentId()),
         req.getDescription(),
         statusToStr(req.getStatus()),
         std::to_string(req.getPriority())}
    );
    session->commit();

    const int id = result[0]["id"].as<int>();
    Logger::info("[PostgresMaintenanceRequestRepository::save] MaintenanceRequest saved with id: " + std::to_string(id));
    return id;
}

std::optional<MaintenanceRequest> PostgresMaintenanceRequestRepository::findById(int id) {
    auto session = m_factory->createSession();
    auto result = session->execParams(
        "SELECT id, resident_id, apartment_id, description, status, priority"
        " FROM MaintenanceRequests WHERE id = $1",
        std::to_string(id)
    );

    if (result.empty()) {
        Logger::info("[PostgresMaintenanceRequestRepository::findById] Not found with id: " + std::to_string(id));
        return std::nullopt;
    }

    Logger::info("[PostgresMaintenanceRequestRepository::findById] Found with id: " + std::to_string(id));
    return rowToMaintenanceRequest(result[0]);
}

std::vector<MaintenanceRequest> PostgresMaintenanceRequestRepository::findAll() {
    auto session = m_factory->createSession();
    auto result = session->exec(
        "SELECT id, resident_id, apartment_id, description, status, priority FROM MaintenanceRequests"
    );

    std::vector<MaintenanceRequest> requests;
    for (const auto& row : result) {
        requests.push_back(rowToMaintenanceRequest(row));
    }

    Logger::info("[PostgresMaintenanceRequestRepository::findAll] Found " +
                 std::to_string(requests.size()) + " maintenance requests");
    return requests;
}

void PostgresMaintenanceRequestRepository::update(const MaintenanceRequest& req) {
    auto session = m_factory->createSession();
    session->execParams(
        "UPDATE MaintenanceRequests SET resident_id=$1, apartment_id=$2, description=$3,"
        " status=$4, priority=$5, updated_at=CURRENT_TIMESTAMP WHERE id=$6",
        {std::to_string(req.getResidentId()),
         std::to_string(req.getApartmentId()),
         req.getDescription(),
         statusToStr(req.getStatus()),
         std::to_string(req.getPriority()),
         std::to_string(req.getId())}
    );
    session->commit();
    Logger::info("[PostgresMaintenanceRequestRepository::update] MaintenanceRequest updated with id: " +
                 std::to_string(req.getId()));
}

void PostgresMaintenanceRequestRepository::remove(int id) {
    auto session = m_factory->createSession();
    session->execParams("DELETE FROM MaintenanceRequests WHERE id = $1", std::to_string(id));
    session->commit();
    Logger::info("[PostgresMaintenanceRequestRepository::remove] MaintenanceRequest removed with id: " +
                 std::to_string(id));
}
