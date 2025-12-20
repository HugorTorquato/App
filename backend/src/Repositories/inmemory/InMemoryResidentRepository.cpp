#include "InMemoryResidentRepository.h"

#include "../../Utils/Logger.h"

int InMemoryResidentRepository::save(const Resident& resident) {
    int id = nextId++;
    Resident copy = resident;
    copy.updateResidentInfos(id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                             std::nullopt);
    storage.emplace(id, std::move(copy));
    return id;
}

std::optional<Resident> InMemoryResidentRepository::findById(int id) {
    auto it = storage.find(id);
    if (it == storage.end()) {
        return std::nullopt;
    }
    return it->second;
    return std::nullopt;
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
    auto it = storage.find(id);

    if (it == storage.end()) {
        Logger::info("[InMemoryResidentRepository::update] Resident with ID " + std::to_string(id) + " not found.");
        return;
    }

    it->second = resident;
}

// void InMemoryResidentRepository::remove(int id) {
//     storage.erase(id);
// }
