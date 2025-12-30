#include "MaintenanceRequestService.h"

#include "../../Utils/Logger.h"

int MaintenanceRequestService::createMaintenanceRequest(const MaintenanceRequestDTO& request) {
    Logger::info("Creating Maintenance Request for Resident ID: " + std::to_string(request.resident_id));

    MaintenanceRequest newRequest = MaintenanceRequestDTOMapper::fromDTO(request);  // convert DTO -> Domain Entity
    int id = repository.save(newRequest);                                           // persist using repository
    Logger::info("Maintenance Request created with ID: " + std::to_string(id));
    return id;
}

std::optional<MaintenanceRequestDTO> MaintenanceRequestService::getMaintenanceRequestById(int requestId) {
    Logger::info("Fetching Maintenance Request with ID: " + std::to_string(requestId));
    auto requestOpt = repository.findById(requestId);
    if (requestOpt.has_value()) {
        auto request = MaintenanceRequestDTOMapper::toDTO(requestOpt.value());
        Logger::info("Maintenance Request found: " + nlohmann::json(request).dump());
        return request;
    }
    Logger::info("Maintenance Request with ID " + std::to_string(requestId) + " not found");
    return std::nullopt;
}
