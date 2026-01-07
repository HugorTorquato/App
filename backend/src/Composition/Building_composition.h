#pragma once

#include "../Controllers/BuildingController.h"
#include "../Repositories/PostgresDB/PostgresBuildingRepository.h"
#include "../Repositories/inmemory/InMemoryBuildingRepository.h"
#include "../Services/Building/BuildingService.h"  // Should it be interface?
#include "../Utils/Logger.h"

struct BuildingModule {
    std::unique_ptr<IBuildingRepository> repo;
    BuildingService service;
    BuildingController controller;

    BuildingModule(const std::string& mode) : repo(selectRepository(mode)), service(*repo), controller(service) {
        Logger::info("[BuildingModule] Initialized;");
    }

   private:
    static std::unique_ptr<IBuildingRepository> selectRepository(const std::string& mode) {
        if (!mode.empty() && mode == "InMemory") {
            Logger::info("[BuildingModule] Using InMemoryBuildingRepository");
            return std::make_unique<InMemoryBuildingRepository>();
        }

        // Postgres Default
        Logger::info("[BuildingModule] Using PostgresBuildingRepository (default)");
        return std::make_unique<PostgresBuildingRepository>();
    }
};