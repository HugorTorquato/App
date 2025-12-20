#pragma once

#include <map>

#include "../interfaces/IBuilding.h"

class InMemoryBuildingRepository : public IBuildingRepository {
   private:
    std::map<int, Building> storage;
    int nextId = 0;

   public:
    int save(const Building& building) override;
    std::optional<Building> findById(int id) override;
    std::vector<Building> findAll() override;
    void update(const Building& building) override;
    void remove(int id) override;
};