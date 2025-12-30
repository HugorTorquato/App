#pragma once

#include <optional>

#include "../../DTOs/Mappers/BuildingDTOMapper.h"
#include "../../Repositories/interfaces/IBuildingRepository.h"

class IBuildingService {
   public:
    virtual ~IBuildingService() = default;

    // Create a new Building and return its ID ( TODO: may change the return type to BuildingDTO )
    virtual int createBuilding(const BuildingDTO& building) = 0;
    // Fetch a Building by its ID, returning only DTO infos to not expose domain entities
    virtual std::optional<BuildingDTO> getBuildingById(int buildingId) = 0;
};