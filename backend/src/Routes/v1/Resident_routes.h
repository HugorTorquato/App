#pragma once
#include <crow.h>

#include "../../Controllers/ResidentController.h"

inline void registerResidentRoutes(crow::SimpleApp& app, ResidentController& controller) {
    CROW_ROUTE(app, "/Resident/Create").methods(crow::HTTPMethod::POST)([&controller](const crow::request& req) {
        return controller.create(req);
    });

    CROW_ROUTE(app, "/Resident/GetById/<int>").methods(crow::HTTPMethod::GET)([&controller](int id) {
        return controller.getById(id);
    });

    // CROW_ROUTE(app, "/residents/<int>/moveout")
    //     .methods(crow::HTTPMethod::PUT)(
    //         [&controller](int id, const crow::request& req) {
    //             return controller.moveOut(id, req);
    //         });
}