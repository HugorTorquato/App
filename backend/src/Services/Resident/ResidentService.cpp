#include "ResidentService.h"

#include "../../utils/Logger.h"

int ResidentService::createResident(const ResidentDTO& resident) {
    Logger::info("Creating Resident: " + resident.name);
    // return repository.save(resident);

    Resident newResident = ResidentDTOMapper::fromDTO(resident);  // convert DTO -> Domain Entity
    int id = repository.save(newResident);                        // persist using repository
    Logger::info("Resident created with ID: " + std::to_string(id));
    return id;
}

std::optional<ResidentDTO> ResidentService::getResidentById(int residentId) {
    Logger::info("Fetching Resident with ID: " + std::to_string(residentId));
    auto residentOpt = repository.findById(residentId);
    if (residentOpt.has_value()) {
        return ResidentDTOMapper::toDTO(residentOpt.value());
    }
    Logger::info("Resident with ID " + std::to_string(residentId) + " not found");
    // TODO: Handle error properly
    //  throw std::runtime_error("Resident not found");
    return std::nullopt;
}

void ResidentService::moveOutResident(int residentId, int moveOutDate) {
    Logger::info("Moving out Resident with ID: " + std::to_string(residentId) +
                 " on date: " + std::to_string(moveOutDate));
    auto residentOpt = repository.findById(residentId);
    if (residentOpt.has_value()) {
        Resident resident = residentOpt.value();
        resident.updateResidentInfos(std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                     std::nullopt, moveOutDate);
        repository.update(resident);
        Logger::info("Resident with ID " + std::to_string(residentId) + " moved out successfully");
    } else {
        Logger::info("Resident with ID " + std::to_string(residentId) + " not found for move out");
    }
}