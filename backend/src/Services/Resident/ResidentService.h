#pragma once

#include <optional>

#include "../../Repositories/interfaces/IResidentRepository.h"

class ResidentService {
   public:
    explicit ResidentService(IResidentRepository& repo) : repository(repo) {}

    int createResident(const Resident& resident);
    std::optional<Resident> getResidentById(int residentId);
    void moveOutResident(int residentId, int moveOutDate);

   private:
    IResidentRepository& repository;
};