#pragma once

#include <map>

#include "../interfaces/IBuildingRepository.h"
#include "IDbConnectionFactory.h"

class PostgresBuildingRepository : public IBuildingRepository {
   public:
    // - In production: pass a real `DbConnectionFactory`
    // - In tests: pass a fake/mock factory that doesn't touch a real DB
    // - The repo doesn't know which one it has — it only knows `IDbConnectionFactory` (the interface) - Dependency
    // Injection
    explicit PostgresBuildingRepository(std::unique_ptr<IDbConnectionFactory> factory);

    int save(const Building& building) override;
    std::optional<Building> findById(int id) override;
    std::vector<Building> findAll() override;
    void update(const Building& building) override;
    void remove(int id) override;

   protected:
    Building mapRowToBuilding(const pqxx::row& row);

   private:
    std::unique_ptr<IDbConnectionFactory> m_factory;
};