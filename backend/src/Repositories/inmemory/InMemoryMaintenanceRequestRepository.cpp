#include "InMemoryMaintenanceRequestRepository.h"

#include "../../Utils/Logger.h"

int InMemoryMaintenanceRequestRepository::save(const MaintenanceRequest& maintenanceRequest) {
    int id = nextId++;
    MaintenanceRequest copy = maintenanceRequest;
    copy.updateMaintenanceRequestInfos(id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
    storage.emplace(id, std::move(copy));
    Logger::info("MaintenanceRequest saved with id: " + std::to_string(id));
    return id;
}

std::optional<MaintenanceRequest> InMemoryMaintenanceRequestRepository::findById(int id) {
    auto it = storage.find(id);
    if (it == storage.end()) {
        Logger::info("MaintenanceRequest not found with id: " + std::to_string(id));
        return std::nullopt;
    }
    Logger::info("MaintenanceRequest found with id: " + std::to_string(id));
    return it->second;
}

std::vector<MaintenanceRequest> InMemoryMaintenanceRequestRepository::findAll() {
    std::vector<MaintenanceRequest> result;
    for (const auto& [_, maintenanceRequest] : storage) {
        result.push_back(maintenanceRequest);
    }
    Logger::info("Retrieved " + std::to_string(result.size()) + " maintenance requests");
    return result;
}

void InMemoryMaintenanceRequestRepository::update(const MaintenanceRequest& maintenanceRequest) {
    const auto id = maintenanceRequest.getId();
    auto it = storage.find(id);
    if (it == storage.end()) {
        Logger::info("MaintenanceRequest not found for update with id: " + std::to_string(id));
        return;
    }
    it->second = maintenanceRequest;
    Logger::info("MaintenanceRequest updated with id: " + std::to_string(id));
}

void InMemoryMaintenanceRequestRepository::remove(int id) {
    auto it = storage.find(id);
    if (it == storage.end()) {
        Logger::info("MaintenanceRequest not found for removal with id: " + std::to_string(id));
        return;
    }
    storage.erase(id);
    Logger::info("MaintenanceRequest removed with id: " + std::to_string(id));
}
