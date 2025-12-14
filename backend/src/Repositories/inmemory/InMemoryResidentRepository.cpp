#include "InMemoryResidentRepository.h"

int InMemoryResidentRepository::save(const Resident& resident) {
    int id = nextId++;
    Resident copy = resident;
    copy.updateResidentInfos(id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                             std::nullopt);
    storage.emplace(id, std::move(copy));
    return id;
}

// std::optional<Resident> InMemoryResidentRepository::findById(int id) {
//     // auto it = storage.find(id);
//     // if (it == storage.end()) {
//     //     return std::nullopt;
//     // }
//     // return it->second;
//     return std::nullopt;
// }

std::vector<Resident> InMemoryResidentRepository::findAll() {
    std::vector<Resident> result;
    for (const auto& [_, resident] : storage) {
        result.push_back(resident);
    }
    return result;
}

// void InMemoryResidentRepository::update(const Resident& resident) {
//     storage[resident.getId()] = resident;
// }

// void InMemoryResidentRepository::remove(int id) {
//     storage.erase(id);
// }
