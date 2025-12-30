#include "MaintenanceRequestController.h"

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "../Utils/Logger.h"
#include "crow/http_request.h"
#include "crow/http_response.h"

crow::response MaintenanceRequestController::create(const crow::request& req) {
    try {
        auto jsonReq = nlohmann::json::parse(req.body);
        Logger::info("Received MaintenanceRequest creation request: " + jsonReq.dump());

        MaintenanceRequestDTO maintenanceRequestDTO = jsonReq.get<MaintenanceRequestDTO>();
        Logger::info("Parsed MaintenanceRequestDTO: " + nlohmann::json(maintenanceRequestDTO).dump());

        auto maintenanceRequestEntityID = maintenanceRequestService.createMaintenanceRequest(maintenanceRequestDTO);
        Logger::info("Created MaintenanceRequest with ID: " + std::to_string(maintenanceRequestEntityID));

        crow::response res;
        res.code = 201;
        res.set_header("Location", "/maintenance_requests/" + std::to_string(maintenanceRequestEntityID));
        res.body = nlohmann::json({{"id", maintenanceRequestEntityID}}).dump();
        return res;
    } catch (const std::exception& e) {
        Logger::info(std::string("Error in MaintenanceRequestController::create: ") + e.what());
        return crow::response(400, "Invalid request data");
    }
}

crow::response MaintenanceRequestController::getById(int id) {
    try {
        auto maintenanceRequestOpt = maintenanceRequestService.getMaintenanceRequestById(id);
        if (maintenanceRequestOpt.has_value()) {
            return crow::response(200, nlohmann::json(maintenanceRequestOpt.value()).dump());
        } else {
            return crow::response(404, "Maintenance Request not found");
        }
    } catch (const std::exception& e) {
        Logger::info(std::string("Error in MaintenanceRequestController::getById: ") + e.what());
        return crow::response(500, "Internal server error");
    }
}
