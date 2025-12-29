#pragma once

#include <optional>

#include "../../DTOs/Mappers/ApartmentDTOMapper.h"
#include "../../Repositories/interfaces/IApartmentRepository.h"

class ApartmentService {
   public:
    explicit ApartmentService(IApartmentRepository& repo) : repository(repo) {}

    // Create a new Apartment and return its ID ( TODO: may change the return type to ApartmentDTO )
    int createApartment(const ApartmentDTO& apartment);
    // Fetch an Apartment by its ID, returning only DTO infos to not expose domain entities
    std::optional<ApartmentDTO> getApartmentById(int apartmentId);

   private:
    IApartmentRepository& repository;
};