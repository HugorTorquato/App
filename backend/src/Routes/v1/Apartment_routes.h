#pragma once
#include <crow.h>

#include "../../Controllers/ApartmentController.h"

inline void registerApartmentRoutes(crow::SimpleApp& app, ApartmentController& controller) {
    CROW_ROUTE(app, "/Apartment/Create").methods(crow::HTTPMethod::POST)([&controller](const crow::request& req) {
        return controller.create(req);
    });

    CROW_ROUTE(app, "/Apartment/GetById/<int>").methods(crow::HTTPMethod::GET)([&controller](int id) {
        return controller.getById(id);
    });
}