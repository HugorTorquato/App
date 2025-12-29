#pragma once

#include <optional>

#include "../../DTOs/Mappers/ResidentDTOMapper.h"
#include "../../Repositories/interfaces/IResidentRepository.h"

class ResidentService {
   public:
    explicit ResidentService(IResidentRepository& repo) : repository(repo) {}

    // Create a new Resident and return its ID ( TODO: may change the return type to ResidentDTO )
    int createResident(const ResidentDTO& resident);
    // Fetch a Resident by its ID, returning only DTO infos to not expose domain entities
    std::optional<ResidentDTO> getResidentById(int residentId);

    // Update the move-out date of a Resident by its ID
    void updateMoveOutResident(int residentId, int moveOutDate);

   private:
    IResidentRepository& repository;
};