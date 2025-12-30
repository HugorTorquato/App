#pragma once

#include <optional>

#include "../../DTOs/Mappers/ApartmentDTOMapper.h"
#include "../../Repositories/interfaces/IApartmentRepository.h"

class IApartmentService {
   public:
    virtual ~IApartmentService() = default;

    // Create a new Apartment and return its ID ( TODO: may change the return type to ApartmentDTO )
    virtual int createApartment(const ApartmentDTO& apartment) = 0;
    // Fetch an Apartment by its ID, returning only DTO infos to not expose domain entities
    virtual std::optional<ApartmentDTO> getApartmentById(int apartmentId) = 0;
};