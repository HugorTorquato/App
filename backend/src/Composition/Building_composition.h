#pragma once

#include "../Controllers/BuildingController.h"
#include "../Repositories/inmemory/InMemoryBuildingRepository.h"
#include "../Services/Building/BuildingService.h"  // Should it be interface?
#include "../Utils/Logger.h"

struct BuildingModule {
    InMemoryBuildingRepository repo;
    BuildingService service;
    BuildingController controller;

    BuildingModule() : repo(), service(repo), controller(service) { Logger::info("[BuildingModule] Initialized;"); }
};