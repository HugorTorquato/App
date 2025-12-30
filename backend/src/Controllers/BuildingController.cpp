#include "BuildingController.h"

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "../Utils/Logger.h"
#include "crow/http_request.h"
#include "crow/http_response.h"

crow::response BuildingController::create(const crow::request& req) {
    try {
        auto jsonReq = nlohmann::json::parse(req.body);
        Logger::info("Received Building creation request: " + jsonReq.dump());

        BuildingDTO buildingDTO = jsonReq.get<BuildingDTO>();
        Logger::info("Parsed BuildingDTO: " + nlohmann::json(buildingDTO).dump());

        auto buildingEntityID = buildingService.createBuilding(buildingDTO);
        Logger::info("Created Building with ID: " + std::to_string(buildingEntityID));

        crow::response res;
        res.code = 201;
        res.set_header("Location", "/buildings/" + std::to_string(buildingEntityID));
        res.body = nlohmann::json({"id", buildingEntityID}).dump();
        return res;
    } catch (const std::exception& e) {
        Logger::info(std::string("Error in BuildingController::create: ") + e.what());
        return crow::response(400, "Invalid request data");
    }
}

crow::response BuildingController::getById(int id) {
    try {
        auto buildingOpt = buildingService.getBuildingById(id);
        if (buildingOpt.has_value()) {
            return crow::response(200, nlohmann::json(buildingOpt.value()).dump());
        } else {
            return crow::response(404, "Building not found");
        }
    } catch (const std::exception& e) {
        Logger::info(std::string("Error in BuildingController::getById: ") + e.what());
        return crow::response(500, "Internal server error");
    }
}