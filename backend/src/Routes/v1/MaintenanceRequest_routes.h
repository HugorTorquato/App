#pragma once
#include <crow.h>

#include "../../Controllers/MaintenanceRequestController.h"

inline void registerMaintenanceRequestRoutes(crow::SimpleApp& app, MaintenanceRequestController& controller) {
    CROW_ROUTE(app, "/MaintenanceRequest/Create")
        .methods(crow::HTTPMethod::POST)([&controller](const crow::request& req) { return controller.create(req); });

    CROW_ROUTE(app, "/MaintenanceRequest/GetById/<int>").methods(crow::HTTPMethod::GET)([&controller](int id) {
        return controller.getById(id);
    });
}