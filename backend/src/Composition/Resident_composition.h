#pragma once

#include "../Controllers/ResidentController.h"
#include "../Repositories/PostgresDB/PostgresResidentRepository.h"
#include "../Repositories/inmemory/InMemoryResidentRepository.h"
#include "../Services/Resident/ResidentService.h"  // Should it be interface?
#include "../Utils/Logger.h"

struct ResidentModule {
    std::unique_ptr<IResidentRepository> repo;
    ResidentService service;
    ResidentController controller;

    ResidentModule(const char* mode) : repo(selectRepository(mode)), service(*repo), controller(service) {
        Logger::info("[ResidentModule] Initialized;");
    }

   private:
    static std::unique_ptr<IResidentRepository> selectRepository(const char* mode) {
        if (mode && std::string(mode) == "InMemory") {
            Logger::info("[ResidentModule] Using InMemoryResidentRepository");
            return std::make_unique<InMemoryResidentRepository>();
        }

        // Postgres Default
        Logger::info("[ResidentModule] Using PostgresResidentRepository (default)");
        return std::make_unique<PostgresResidentRepository>();
    }
};