#pragma once

#include "../../Domains/apartment.h"
#include "../apartment_dto.h"

class ApartmentDTOMapper {
   public:
    static ApartmentDTO toDTO(const Apartment& apartment) {
        ApartmentDTO dto;
        dto.id = apartment.getId();
        dto.number = apartment.getNumber();
        dto.area_m2 = apartment.getAreaM2();
        return dto;
    }

    static Apartment fromDTO(const ApartmentDTO& dto) {
        return Apartment(dto.id, 0, dto.number, false, false, 0.0, dto.area_m2);
    }
};