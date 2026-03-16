#pragma once

#include <memory>

#include "../interfaces/IResidentRepository.h"
#include "IDbConnectionFactory.h"

class PostgresResidentRepository : public IResidentRepository {
   public:
    explicit PostgresResidentRepository(std::shared_ptr<IDbConnectionFactory> factory);

    int save(const Resident& resident) override;
    std::optional<Resident> findById(int id) override;
    std::vector<Resident> findAll() override;
    void update(const Resident& resident) override;
    void remove(int id) override;

   private:
    std::shared_ptr<IDbConnectionFactory> m_factory;
};
