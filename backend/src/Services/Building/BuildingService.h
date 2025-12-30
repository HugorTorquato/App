#pragma once

#include "IBuildingService.h"

class BuildingService : public IBuildingService {
   public:
    explicit BuildingService(IBuildingRepository& repo) : repository(repo) {}

    // Create a new Building and return its ID ( TODO: may change the return type to BuildingDTO )
    int createBuilding(const BuildingDTO& building) override;
    // Fetch a Building by its ID, returning only DTO infos to not expose domain entities
    std::optional<BuildingDTO> getBuildingById(int buildingId) override;

   private:
    IBuildingRepository& repository;
};