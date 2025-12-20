#include "InMemoryBuildingRepository.h"

int InMemoryBuildingRepository::save(const Building& building) {
    int id = nextId++;
    Building copy = building;
    copy.updateBuildInfos(id, std::nullopt, std::nullopt, std::nullopt);
    storage.emplace(id, std::move(copy));
    return id;
}

std::optional<Building> InMemoryBuildingRepository::findById(int id) {
    auto it = storage.find(id);
    if (it == storage.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::vector<Building> InMemoryBuildingRepository::findAll() {
    std::vector<Building> result;
    for (const auto& [_, building] : storage) {
        result.push_back(building);
    }
    return result;
}

void InMemoryBuildingRepository::update(const Building& building) {
    const auto id = building.getId();
    auto it = storage.find(id);
    if (it == storage.end()) {
        return;
    }
    it->second = building;
}

void InMemoryBuildingRepository::remove(int id) {
    auto it = storage.find(id);
    if (it == storage.end()) {
        return;
    }
    storage.erase(id);
}
