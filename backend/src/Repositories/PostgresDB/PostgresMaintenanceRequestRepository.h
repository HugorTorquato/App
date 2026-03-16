#pragma once

#include <memory>

#include "../interfaces/IMaintenanceRequestRepository.h"
#include "IDbConnectionFactory.h"

class PostgresMaintenanceRequestRepository : public IMaintenanceRequestRepository {
   public:
    explicit PostgresMaintenanceRequestRepository(std::shared_ptr<IDbConnectionFactory> factory);

    int save(const MaintenanceRequest& maintenanceRequest) override;
    std::optional<MaintenanceRequest> findById(int id) override;
    std::vector<MaintenanceRequest> findAll() override;
    void update(const MaintenanceRequest& maintenanceRequest) override;
    void remove(int id) override;

   private:
    std::shared_ptr<IDbConnectionFactory> m_factory;
};
