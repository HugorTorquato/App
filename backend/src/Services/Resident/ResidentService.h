#pragma once

#include <optional>

#include "../../DTOs/ResidentDTOMapper.h"
#include "../../Repositories/interfaces/IResidentRepository.h"

class ResidentService {
   public:
    explicit ResidentService(IResidentRepository& repo) : repository(repo) {}

    int createResident(const ResidentDTO& resident);
    std::optional<ResidentDTO> getResidentById(int residentId);
    void moveOutResident(int residentId, int moveOutDate);

   private:
    IResidentRepository& repository;
};