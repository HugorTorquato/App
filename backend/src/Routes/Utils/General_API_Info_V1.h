#pragma once
#include "crow.h"
#include "../../Utils/Logger.h"

void registerGeneralAPIInfoV1(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/api/v1/info")([](){
        Logger::info("registerGeneralAPIInfoV1 /api/v1/info");
        return crow::json::wvalue{
            {"name", "Condo Management API"},
            {"version", "1.0.0"},
            {"description", "API for managing condo operations"}
        };
    });
}