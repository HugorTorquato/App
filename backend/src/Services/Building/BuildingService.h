#pragma once

#include <optional>

#include "../../DTOs/Mappers/BuildingDTOMapper.h"
#include "../../Repositories/interfaces/IBuildingRepository.h"

class BuildingService {
   public:
    explicit BuildingService(IBuildingRepository& repo) : repository(repo) {}

    // Create a new Building and return its ID ( TODO: may change the return type to BuildingDTO )
    int createBuilding(const BuildingDTO& building);
    // Fetch a Building by its ID, returning only DTO infos to not expose domain entities
    std::optional<BuildingDTO> getBuildingById(int buildingId);

   private:
    IBuildingRepository& repository;
};