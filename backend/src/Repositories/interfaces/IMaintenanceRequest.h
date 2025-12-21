#pragma once

#include <optional>
#include <vector>

#include "../../Domains/maintenanceRequest.h"

class IMaintenanceRequestRepository {
   public:
    virtual ~IMaintenanceRequestRepository() = default;

    virtual int save(const MaintenanceRequest& maintenanceRequest) = 0;
    virtual std::optional<MaintenanceRequest> findById(int id) = 0;
    virtual std::vector<MaintenanceRequest> findAll() = 0;
    virtual void update(const MaintenanceRequest& maintenanceRequest) = 0;
    virtual void remove(int id) = 0;
};