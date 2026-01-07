#pragma once

#include "../Controllers/MaintenanceRequestController.h"
#include "../Repositories/PostgresDB/PostgresMaintenanceRequestRepository.h"
#include "../Repositories/inmemory/InMemoryMaintenanceRequestRepository.h"
#include "../Services/MaintenanceRequest/MaintenanceRequestService.h"  // Should it be interface?
#include "../Utils/Logger.h"

struct MaintenanceRequestModule {
    std::unique_ptr<IMaintenanceRequestRepository> repo;
    MaintenanceRequestService service;
    MaintenanceRequestController controller;

    MaintenanceRequestModule(const char* mode) : repo(selectRepository(mode)), service(*repo), controller(service) {
        Logger::info("[MaintenanceRequestModule] Initialized;");
    }

   private:
    static std::unique_ptr<IMaintenanceRequestRepository> selectRepository(const char* mode) {
        if (mode && std::string(mode) == "InMemory") {
            Logger::info("[MaintenanceRequestModule] Using InMemoryMaintenanceRequestRepository");
            return std::make_unique<InMemoryMaintenanceRequestRepository>();
        }

        // Postgres Default
        Logger::info("[MaintenanceRequestModule] Using PostgresMaintenanceRequestRepository (default)");
        return std::make_unique<PostgresMaintenanceRequestRepository>();
    }
};