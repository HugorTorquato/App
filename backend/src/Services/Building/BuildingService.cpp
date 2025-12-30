#include "BuildingService.h"

#include "../../Utils/Logger.h"

int BuildingService::createBuilding(const BuildingDTO& building) {
    Logger::info("Creating Building: " + building.name);

    Building newBuilding = BuildingDTOMapper::fromDTO(building);  // convert DTO -> Domain Entity
    int id = repository.save(newBuilding);                        // persist using repository
    Logger::info("Building created with ID: " + std::to_string(id));
    return id;
}

std::optional<BuildingDTO> BuildingService::getBuildingById(int buildingId) {
    Logger::info("Fetching Building with ID: " + std::to_string(buildingId));
    auto buildingOpt = repository.findById(buildingId);
    if (buildingOpt.has_value()) {
        auto request = BuildingDTOMapper::toDTO(buildingOpt.value());
        Logger::info("Building found: " + nlohmann::json(request).dump());
        return request;
    }
    Logger::info("Building with ID " + std::to_string(buildingId) + " not found");
    return std::nullopt;
}