#pragma once

#include <unordered_map>

#include "../interfaces/IResidentRepository.h"

class InMemoryResidentRepository : public IResidentRepository {
   private:
    std::unordered_map<int, Resident> storage;
    int nextId = 1;

   public:
    int save(const Resident& resident) override;
    // std::optional<Resident> findById(int id) override;
    std::vector<Resident> findAll() override;
    // void update(const Resident& resident) override;
    // void remove(int id) override;
};
