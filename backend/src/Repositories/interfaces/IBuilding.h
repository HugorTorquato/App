#pragma once

#include <optional>
#include <vector>

#include "../../Domains/building.h"

class IBuildingRepository {
   public:
    virtual ~IBuildingRepository() = default;

    virtual int save(const Building& building) = 0;
    virtual std::optional<Building> findById(int id) = 0;
    virtual std::vector<Building> findAll() = 0;
    virtual void update(const Building& building) = 0;
    virtual void remove(int id) = 0;
};