#pragma once

#include "IMaintenanceRequestService.h"

class MaintenanceRequestService : public IMaintenanceRequestService {
   public:
    explicit MaintenanceRequestService(IMaintenanceRequestRepository& repo) : repository(repo) {}

    // Create a new Maintenance Request and return its ID
    int createMaintenanceRequest(const MaintenanceRequestDTO& request) override;
    // Fetch a Maintenance Request by its ID, returning only DTO infos to not expose domain entities
    std::optional<MaintenanceRequestDTO> getMaintenanceRequestById(int requestId) override;

   private:
    IMaintenanceRequestRepository& repository;
};
