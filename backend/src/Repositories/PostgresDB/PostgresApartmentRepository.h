#pragma once

#include <memory>

#include "../interfaces/IApartmentRepository.h"
#include "IDbConnectionFactory.h"

class PostgresApartmentRepository : public IApartmentRepository {
   public:
    explicit PostgresApartmentRepository(std::shared_ptr<IDbConnectionFactory> factory);

    int save(const Apartment& apartment) override;
    std::optional<Apartment> findById(int id) override;
    std::vector<Apartment> findAll() override;
    void update(const Apartment& apartment) override;
    void remove(int id) override;

   private:
    std::shared_ptr<IDbConnectionFactory> m_factory;
};
