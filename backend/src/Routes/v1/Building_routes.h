#pragma once
#include <crow.h>

#include "../../Controllers/BuildingController.h"

inline void registerBuildingRoutes(crow::SimpleApp& app, BuildingController& controller) {
    CROW_ROUTE(app, "/Building/Create").methods(crow::HTTPMethod::POST)([&controller](const crow::request& req) {
        return controller.create(req);
    });

    CROW_ROUTE(app, "/Building/GetById/<int>").methods(crow::HTTPMethod::GET)([&controller](int id) {
        return controller.getById(id);
    });
}