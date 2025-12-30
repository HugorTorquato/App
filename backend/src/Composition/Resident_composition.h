#pragma once

#include "../Controllers/ResidentController.h"
#include "../Repositories/inmemory/InMemoryResidentRepository.h"
#include "../Services/Resident/ResidentService.h"  // Should it be interface?
#include "../Utils/Logger.h"

struct ResidentModule {
    InMemoryResidentRepository repo;
    ResidentService service;
    ResidentController controller;

    ResidentModule() : repo(), service(repo), controller(service) { Logger::info("[ResidentModule] Initialized;"); }
};