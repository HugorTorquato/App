#pragma once

#include <optional>
#include <vector>

#include "../../Domains/resident.h"

class IResidentRepository {
   public:
    virtual ~IResidentRepository() = default;

    virtual int save(const Resident& resident) = 0;
    virtual std::optional<Resident> findById(int id) = 0;
    virtual std::vector<Resident> findAll() = 0;
    virtual void update(const Resident& resident) = 0;
    virtual void remove(int id) = 0;
};