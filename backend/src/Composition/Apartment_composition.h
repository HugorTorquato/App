#pragma once

#include "../Controllers/ApartmentController.h"
#include "../Repositories/inmemory/InMemoryApartmentRepository.h"
#include "../Services/Apartment/ApartmentService.h"  // Should it be interface?
#include "../Utils/Logger.h"

struct ApartmentModule {
    InMemoryApartmentRepository repo;
    ApartmentService service;
    ApartmentController controller;

    ApartmentModule() : repo(), service(repo), controller(service) { Logger::info("[ApartmentModule] Initialized;"); }
};