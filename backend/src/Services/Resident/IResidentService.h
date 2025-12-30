#pragma once

#include <optional>

#include "../../DTOs/Mappers/ResidentDTOMapper.h"
#include "../../Repositories/interfaces/IResidentRepository.h"

class IResidentService {
   public:
    virtual ~IResidentService() = default;

    // Create a new Resident and return its ID ( TODO: may change the return type to ResidentDTO )
    virtual int createResident(const ResidentDTO& resident) = 0;
    // Fetch a Resident by its ID, returning only DTO infos to not expose domain entities
    virtual std::optional<ResidentDTO> getResidentById(int residentId) = 0;

    // Update the move-out date of a Resident by its ID
    virtual void updateMoveOutResident(int residentId, int moveOutDate) = 0;
};