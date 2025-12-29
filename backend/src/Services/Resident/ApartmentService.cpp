#include "ApartmentService.h"

#include "../../Utils/Logger.h"

int ApartmentService::createApartment(const ApartmentDTO& apartment) {
    Logger::info("Creating Apartment: " + apartment.number);

    Apartment newApartment = ApartmentDTOMapper::fromDTO(apartment);  // convert DTO -> Domain Entity
    int id = repository.save(newApartment);                           // persist using repository
    Logger::info("Apartment created with ID: " + std::to_string(id));
    return id;
}

std::optional<ApartmentDTO> ApartmentService::getApartmentById(int apartmentId) {
    Logger::info("Fetching Apartment with ID: " + std::to_string(apartmentId));
    auto apartmentOpt = repository.findById(apartmentId);
    if (apartmentOpt.has_value()) {
        return ApartmentDTOMapper::toDTO(apartmentOpt.value());
    }
    Logger::info("Apartment with ID " + std::to_string(apartmentId) + " not found");
    return std::nullopt;
}
