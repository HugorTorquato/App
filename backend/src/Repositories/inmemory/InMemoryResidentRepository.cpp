#include "InMemoryResidentRepository.h"

#include "../../Utils/Logger.h"

int InMemoryResidentRepository::save(const Resident& resident) {
    // TODO: If duplicated residents.
    int id = nextId++;
    Logger::info("[InMemoryResidentRepository::save] Saving resident with new ID: " + std::to_string(id));
    Resident copy = resident;
    copy.updateResidentInfos(id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                             std::nullopt);
    storage.emplace(id, std::move(copy));
    Logger::info("[InMemoryResidentRepository::save] Resident saved successfully with ID: " + std::to_string(id));
    return id;
}

std::optional<Resident> InMemoryResidentRepository::findById(int id) {
    Logger::info("[InMemoryResidentRepository::findById] Finding resident with ID: " + std::to_string(id));
    auto it = storage.find(id);
    if (it == storage.end()) {
        Logger::info("[InMemoryResidentRepository::findById] Resident with ID: " + std::to_string(id) + " not found.");
        return std::nullopt;
    }
    Logger::info("[InMemoryResidentRepository::findById] Found resident with ID: " + std::to_string(id));
    return it->second;
}

std::vector<Resident> InMemoryResidentRepository::findAll() {
    Logger::info("[InMemoryResidentRepository::findAll] Finding all residents...");
    std::vector<Resident> result;
    for (const auto& [_, resident] : storage) {
        Logger::info("[InMemoryResidentRepository::findAll] Found resident with ID: " +
                     std::to_string(resident.getId()) + " and Name: " + resident.getFullName());
        result.push_back(resident);
    }
    Logger::info("[InMemoryResidentRepository::findAll] Found " + std::to_string(result.size()) + " residents.");
    return result;
}

void InMemoryResidentRepository::update(const Resident& resident) {
    const auto id = resident.getId();
    Logger::info("[InMemoryResidentRepository::update] Updating resident with ID: " + std::to_string(id));
    auto it = storage.find(id);
    if (it == storage.end()) {
        Logger::info("[InMemoryResidentRepository::update] Resident with ID " + std::to_string(id) + " not found.");
        return;
    }
    it->second = resident;
    Logger::info("[InMemoryResidentRepository::update] Resident with ID " + std::to_string(id) +
                 " updated successfully.");
}

void InMemoryResidentRepository::remove(int id) {
    Logger::info("[InMemoryResidentRepository::remove] Removing resident with ID: " + std::to_string(id));
    auto it = storage.find(id);
    if (it == storage.end()) {
        Logger::info("[InMemoryResidentRepository::remove] Resident with ID " + std::to_string(id) + " not found.");
        return;
    }
    storage.erase(id);
    Logger::info("[InMemoryResidentRepository::remove] Resident with ID " + std::to_string(id) +
                 " removed successfully.");
}
