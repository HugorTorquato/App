#pragma once

#include "../Controllers/MaintenanceRequestController.h"
#include "../Repositories/inmemory/InMemoryMaintenanceRequestRepository.h"
#include "../Services/MaintenanceRequest/MaintenanceRequestService.h"  // Should it be interface?
#include "../Utils/Logger.h"

struct MaintenanceRequestModule {
    InMemoryMaintenanceRequestRepository repo;
    MaintenanceRequestService service;
    MaintenanceRequestController controller;

    MaintenanceRequestModule() : repo(), service(repo), controller(service) {
        Logger::info("[MaintenanceRequestModule] Initialized;");
    }
};