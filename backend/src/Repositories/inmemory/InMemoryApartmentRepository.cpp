#include "InMemoryApartmentRepository.h"

#include "../../Utils/Logger.h"

int InMemoryApartmentRepository::save(const Apartment& apartment) {
    //TODO: If duplicated apartments.
    int id = nextId++;
    Apartment copy = apartment;
    copy.updateApartmentInfos(id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
    storage.emplace(id, std::move(copy));
    Logger::info("[InMemoryApartmentRepository::save] Saved apartment with ID: " + std::to_string(id));
    return id;
}

std::optional<Apartment> InMemoryApartmentRepository::findById(int id) {
    auto it = storage.find(id);
    if (it == storage.end()) {
        Logger::info("[InMemoryApartmentRepository::findById] Apartment with ID " + std::to_string(id) + " not found.");
        return std::nullopt;
    }
    Logger::info("[InMemoryApartmentRepository::findById] Found apartment with ID: " + std::to_string(id));
    return it->second;
}

std::vector<Apartment> InMemoryApartmentRepository::findAll() {
    Logger::info("[InMemoryApartmentRepository::findAll] Finding all apartments...");
    std::vector<Apartment> result;
    for (const auto& [_, apartment] : storage) {
        Logger::info("[InMemoryApartmentRepository::findAll] Found apartment with ID: " +
                     std::to_string(apartment.getId()) + " and Number: " + apartment.getNumber());
        result.push_back(apartment);
    }
    Logger::info("[InMemoryApartmentRepository::findAll] Found " + std::to_string(result.size()) + " apartments.");
    return result;
}

void InMemoryApartmentRepository::update(const Apartment& apartment) {
    const auto id = apartment.getId();
    auto it = storage.find(id);
    if (it == storage.end()) {
        Logger::info("[InMemoryApartmentRepository::update] Apartment with ID " + std::to_string(id) + " not found.");
        return;
    }
    it->second = apartment;
}

void InMemoryApartmentRepository::remove(int id) {
    auto it = storage.find(id);
    if (it == storage.end()) {
        Logger::info("[InMemoryApartmentRepository::remove] Apartment with ID " + std::to_string(id) + " not found.");
        return;
    }
    storage.erase(id);
}