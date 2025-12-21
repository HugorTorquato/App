#include "InMemoryBuildingRepository.h"

#include "../../Utils/Logger.h"

int InMemoryBuildingRepository::save(const Building& building) {
    int id = nextId++;
    Building copy = building;
    copy.updateBuildInfos(id, std::nullopt, std::nullopt, std::nullopt);
    storage.emplace(id, std::move(copy));
    Logger::info("Building saved with id: " + std::to_string(id));
    return id;
}

std::optional<Building> InMemoryBuildingRepository::findById(int id) {
    auto it = storage.find(id);
    if (it == storage.end()) {
        Logger::info("Building not found with id: " + std::to_string(id));
        return std::nullopt;
    }
    Logger::info("Building found with id: " + std::to_string(id));
    return it->second;
}

std::vector<Building> InMemoryBuildingRepository::findAll() {
    std::vector<Building> result;
    for (const auto& [_, building] : storage) {
        result.push_back(building);
    }
    Logger::info("Retrieved " + std::to_string(result.size()) + " buildings");
    return result;
}

void InMemoryBuildingRepository::update(const Building& building) {
    const auto id = building.getId();
    auto it = storage.find(id);
    if (it == storage.end()) {
        Logger::info("Cannot update: Building not found with id: " + std::to_string(id));
        return;
    }
    it->second = building;
    Logger::info("Building updated with id: " + std::to_string(id));
}

void InMemoryBuildingRepository::remove(int id) {
    auto it = storage.find(id);
    if (it == storage.end()) {
        Logger::info("Cannot remove: Building not found with id: " + std::to_string(id));
        return;
    }
    storage.erase(id);
    Logger::info("Building removed with id: " + std::to_string(id));
}
