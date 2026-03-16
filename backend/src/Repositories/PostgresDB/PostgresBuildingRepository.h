#pragma once

#include <memory>

#include "../interfaces/IBuildingRepository.h"
#include "IDbConnectionFactory.h"

class PostgresBuildingRepository : public IBuildingRepository {
   public:
    // Dependency injection: the factory is created outside and passed in.
    // The repo doesn't know if it's a real DB or a test fake — only the interface matters.
    explicit PostgresBuildingRepository(std::shared_ptr<IDbConnectionFactory> factory);

    int save(const Building& building) override;
    std::optional<Building> findById(int id) override;
    std::vector<Building> findAll() override;
    void update(const Building& building) override;
    void remove(int id) override;

   private:
    std::shared_ptr<IDbConnectionFactory> m_factory;
};
