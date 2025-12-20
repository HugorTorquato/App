#pragma once

#include <optional>
#include <vector>

#include "../../Domains/apartment.h"

class IApartmentRepository {
   public:
    virtual ~IApartmentRepository() = default;

    virtual int save(const Apartment& apartment) = 0;
    virtual std::optional<Apartment> findById(int id) = 0;
    virtual std::vector<Apartment> findAll() = 0;
    virtual void update(const Apartment& apartment) = 0;
    virtual void remove(int id) = 0;
};