#include "ApartmentController.h"

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "../Utils/Logger.h"
#include "crow/http_request.h"
#include "crow/http_response.h"

crow::response ApartmentController::create(const crow::request& req) {
    try {
        auto jsonReq = nlohmann::json::parse(req.body);
        Logger::info("Received Apartment creation request: " + jsonReq.dump());

        ApartmentDTO apartmentDTO = jsonReq.get<ApartmentDTO>();
        Logger::info("Parsed ApartmentDTO: " + nlohmann::json(apartmentDTO).dump());

        auto apartmentEntityID = apartmentService.createApartment(apartmentDTO);
        Logger::info("Created Apartment with ID: " + std::to_string(apartmentEntityID));

        crow::response res;
        res.code = 201;
        res.set_header("Location", "/apartments/" + std::to_string(apartmentEntityID));
        res.body = nlohmann::json({"id", apartmentEntityID}).dump();
        return res;
    } catch (const std::exception& e) {
        Logger::info(std::string("Error in ApartmentController::create: ") + e.what());
        return crow::response(400, "Invalid request data");
    }
}

crow::response ApartmentController::getById(int id) {
    try {
        auto apartmentOpt = apartmentService.getApartmentById(id);
        if (apartmentOpt.has_value()) {
            return crow::response(200, nlohmann::json(apartmentOpt.value()).dump());
        } else {
            return crow::response(404, "Apartment not found");
        }
    } catch (const std::exception& e) {
        Logger::info(std::string("Error in ApartmentController::getById: ") + e.what());
        return crow::response(500, "Internal server error");
    }
}