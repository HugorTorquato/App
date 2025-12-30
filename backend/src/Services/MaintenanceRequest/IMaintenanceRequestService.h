#pragma once

#include <optional>

#include "../../DTOs/Mappers/MaintenanceRequestDTOMapper.h"
#include "../../Repositories/interfaces/IMaintenanceRequestRepository.h"

class IMaintenanceRequestService {
   public:
    virtual ~IMaintenanceRequestService() = default;

    // Create a new Maintenance Request and return its ID
    virtual int createMaintenanceRequest(const MaintenanceRequestDTO& request) = 0;
    // Fetch a Maintenance Request by its ID, returning only DTO infos to not expose domain entities
    virtual std::optional<MaintenanceRequestDTO> getMaintenanceRequestById(int requestId) = 0;
};