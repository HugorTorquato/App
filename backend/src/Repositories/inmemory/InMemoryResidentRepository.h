#pragma once

#include <map>

#include "../interfaces/IResidentRepository.h"

class InMemoryResidentRepository : public IResidentRepository {
   private:
    std::map<int, Resident> storage;
    int nextId = 0;

   public:
    int save(const Resident& resident) override;
    std::optional<Resident> findById(int id) override;
    std::vector<Resident> findAll() override;
    void update(const Resident& resident) override;
    // void remove(int id) override;
};
