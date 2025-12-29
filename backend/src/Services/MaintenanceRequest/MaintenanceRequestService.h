#pragma once

#include <optional>

#include "../../DTOs/Mappers/MaintenanceRequestDTOMapper.h"
#include "../../Repositories/interfaces/IMaintenanceRequestRepository.h"

class MaintenanceRequestService {
   public:
    explicit MaintenanceRequestService(IMaintenanceRequestRepository& repo) : repository(repo) {}

    // Create a new Maintenance Request and return its ID
    int createMaintenanceRequest(const MaintenanceRequestDTO& request);
    // Fetch a Maintenance Request by its ID, returning only DTO infos to not expose domain entities
    std::optional<MaintenanceRequestDTO> getMaintenanceRequestById(int requestId);

   private:
    IMaintenanceRequestRepository& repository;
};
