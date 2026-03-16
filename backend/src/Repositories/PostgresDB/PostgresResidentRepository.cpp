#include "PostgresResidentRepository.h"

#include <ctime>
#include <pqxx/pqxx>
#include <sstream>

#include "../../Utils/Logger.h"

// -------------------------------------------------------------------
// time_t ↔ PostgreSQL TIMESTAMP conversion
//
// WHY: The domain uses time_t (Unix epoch seconds, an integer). The
// DB column is TIMESTAMP (e.g. "2024-01-15 10:30:00"). The repo is
// the right place to convert — the domain doesn't care about SQL
// string formats, and the DB doesn't care about C++ time_t.
//
// timeToStr: formats time_t as "YYYY-MM-DD HH:MM:SS" in UTC.
// strToTime: parses that same format back to time_t. strptime is
//   POSIX-standard and available on Linux/macOS.
// -------------------------------------------------------------------
static std::string timeToStr(time_t t) {
    char buf[32];
    struct tm* tm_utc = gmtime(&t);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_utc);
    return std::string(buf);
}

static time_t strToTime(const std::string& s) {
    struct tm tm = {};
    strptime(s.c_str(), "%Y-%m-%d %H:%M:%S", &tm);
    return timegm(&tm);  // timegm interprets the struct as UTC, matching gmtime above
}

static Resident rowToResident(const pqxx::row& row) {
    return Resident(
        row["id"].as<int>(),
        row["full_name"].as<std::string>(),
        row["document_number"].as<std::string>(),
        row["apartment_id"].as<std::string>(),  // TEXT, not a foreign key int
        row["is_owner"].as<bool>(),
        row["phone"].as<std::string>(),
        strToTime(row["move_in_date"].as<std::string>()),
        strToTime(row["move_out_date"].as<std::string>())
    );
}

PostgresResidentRepository::PostgresResidentRepository(std::shared_ptr<IDbConnectionFactory> factory)
    : m_factory(std::move(factory)) {}

int PostgresResidentRepository::save(const Resident& resident) {
    auto session = m_factory->createSession();
    auto result = session->execParams(
        "INSERT INTO Residents (full_name, document_number, apartment_id, is_owner, phone, move_in_date, move_out_date)"
        " VALUES ($1, $2, $3, $4, $5, $6, $7) RETURNING id",
        {resident.getFullName(),
         resident.getDocumentNumber(),
         resident.getApartmentId(),
         resident.getIsOwner() ? "true" : "false",
         resident.getPhone(),
         timeToStr(resident.getMoveInDate()),
         timeToStr(resident.getMoveOutDate())}
    );
    session->commit();

    const int id = result[0]["id"].as<int>();
    Logger::info("[PostgresResidentRepository::save] Resident saved with id: " + std::to_string(id));
    return id;
}

std::optional<Resident> PostgresResidentRepository::findById(int id) {
    auto session = m_factory->createSession();
    auto result = session->execParams(
        "SELECT id, full_name, document_number, apartment_id, is_owner, phone, move_in_date, move_out_date"
        " FROM Residents WHERE id = $1",
        std::to_string(id)
    );

    if (result.empty()) {
        Logger::info("[PostgresResidentRepository::findById] Resident not found with id: " + std::to_string(id));
        return std::nullopt;
    }

    Logger::info("[PostgresResidentRepository::findById] Resident found with id: " + std::to_string(id));
    return rowToResident(result[0]);
}

std::vector<Resident> PostgresResidentRepository::findAll() {
    auto session = m_factory->createSession();
    auto result = session->exec(
        "SELECT id, full_name, document_number, apartment_id, is_owner, phone, move_in_date, move_out_date"
        " FROM Residents"
    );

    std::vector<Resident> residents;
    for (const auto& row : result) {
        residents.push_back(rowToResident(row));
    }

    Logger::info("[PostgresResidentRepository::findAll] Found " + std::to_string(residents.size()) + " residents");
    return residents;
}

void PostgresResidentRepository::update(const Resident& resident) {
    auto session = m_factory->createSession();
    session->execParams(
        "UPDATE Residents SET full_name=$1, document_number=$2, apartment_id=$3, is_owner=$4,"
        " phone=$5, move_in_date=$6, move_out_date=$7, updated_at=CURRENT_TIMESTAMP WHERE id=$8",
        {resident.getFullName(),
         resident.getDocumentNumber(),
         resident.getApartmentId(),
         resident.getIsOwner() ? "true" : "false",
         resident.getPhone(),
         timeToStr(resident.getMoveInDate()),
         timeToStr(resident.getMoveOutDate()),
         std::to_string(resident.getId())}
    );
    session->commit();
    Logger::info("[PostgresResidentRepository::update] Resident updated with id: " + std::to_string(resident.getId()));
}

void PostgresResidentRepository::remove(int id) {
    auto session = m_factory->createSession();
    session->execParams("DELETE FROM Residents WHERE id = $1", std::to_string(id));
    session->commit();
    Logger::info("[PostgresResidentRepository::remove] Resident removed with id: " + std::to_string(id));
}
