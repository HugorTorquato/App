#pragma once

#include <map>

#include "../interfaces/IApartmentRepository.h"

class InMemoryApartmentRepository : public IApartmentRepository {
   private:
    std::map<int, Apartment> storage;
    int nextId = 0;

   public:
    int save(const Apartment& apartment) override;
    std::optional<Apartment> findById(int id) override;
    std::vector<Apartment> findAll() override;
    void update(const Apartment& apartment) override;
    void remove(int id) override;
};
