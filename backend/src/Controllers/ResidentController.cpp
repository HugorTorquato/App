#include "ResidentController.h"

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "../Utils/Logger.h"
#include "crow/http_request.h"
#include "crow/http_response.h"

crow::response ResidentController::create(const crow::request& req) {
    try {
        auto jsonReq = nlohmann::json::parse(req.body);
        Logger::info("Received Resident creation request: " + jsonReq.dump());

        ResidentDTO residentDTO = jsonReq.get<ResidentDTO>();
        Logger::info("Parsed ResidentDTO: " + nlohmann::json(residentDTO).dump());

        auto residentEntityID = residentService.createResident(residentDTO);
        Logger::info("Created Resident with ID: " + std::to_string(residentEntityID));

        crow::response res;
        res.code = 201;
        res.set_header("Location", "/residents/" + std::to_string(residentEntityID));
        res.body = nlohmann::json({"id", residentEntityID}).dump();
        return res;
    } catch (const std::exception& e) {
        Logger::info(std::string("Error in ResidentController::create: ") + e.what());
        return crow::response(400, "Invalid request data");
    }
}

crow::response ResidentController::getById(int id) {
    try {
        auto residentOpt = residentService.getResidentById(id);
        if (residentOpt.has_value()) {
            return crow::response(200, nlohmann::json(residentOpt.value()).dump());
        } else {
            return crow::response(404, "Resident not found");
        }
    } catch (const std::exception& e) {
        Logger::info(std::string("Error in ResidentController::getById: ") + e.what());
        return crow::response(500, "Internal server error");
    }
}

// crow::response ResidentController::listAll() {
//     try {
//         auto residents = residentService.listAllResidents();
//         return crow::response(200, nlohmann::json(residents).dump());
//     } catch (const std::exception& e) {
//         Logger::info(std::string("Error in ResidentController::listAll: ") + e.what());
//         return crow::response(500, "Internal server error");
//     }
// }