#pragma once

#include "IApartmentService.h"

class ApartmentService : public IApartmentService {
   public:
    explicit ApartmentService(IApartmentRepository& repo) : repository(repo) {}

    // Create a new Apartment and return its ID ( TODO: may change the return type to ApartmentDTO )
    int createApartment(const ApartmentDTO& apartment) override;
    // Fetch an Apartment by its ID, returning only DTO infos to not expose domain entities
    std::optional<ApartmentDTO> getApartmentById(int apartmentId) override;

   private:
    IApartmentRepository& repository;
};