#pragma once

#include <map>

#include "../../Utils/DomainEnums.h"
#include "../interfaces/IMaintenanceRequestRepository.h"

class InMemoryMaintenanceRequestRepository : public IMaintenanceRequestRepository {
   private:
    std::map<int, MaintenanceRequest> storage;
    int nextId = 0;

   public:
    int save(const MaintenanceRequest& maintenanceRequest) override;
    std::optional<MaintenanceRequest> findById(int id) override;
    std::vector<MaintenanceRequest> findAll() override;
    void update(const MaintenanceRequest& maintenanceRequest) override;
    void remove(int id) override;
};