#pragma once

#include "../Controllers/ApartmentController.h"
#include "../Repositories/PostgresDB/PostgresApartmentRepository.h"
#include "../Repositories/inmemory/InMemoryApartmentRepository.h"
#include "../Services/Apartment/ApartmentService.h"  // Should it be interface?
#include "../Utils/Logger.h"

struct ApartmentModule {
    std::unique_ptr<IApartmentRepository> repo;
    ApartmentService service;
    ApartmentController controller;

    ApartmentModule(const std::string& mode) : repo(selectRepository(mode)), service(*repo), controller(service) {
        Logger::info("[ApartmentModule] Initialized;");
    }

   private:
    static std::unique_ptr<IApartmentRepository> selectRepository(const std::string& mode) {
        if (!mode.empty() && mode == "InMemory") {
            Logger::info("[ApartmentModule] Using InMemoryApartmentRepository");
            return std::make_unique<InMemoryApartmentRepository>();
        }

        // Postgres Default
        Logger::info("[ApartmentModule] Using PostgresApartmentRepository (default)");
        return std::make_unique<PostgresApartmentRepository>();
    }
};