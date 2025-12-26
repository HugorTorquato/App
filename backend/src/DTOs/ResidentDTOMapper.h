#pragma once
#include "../Domains/resident.h"
#include "resident_dto.h"

class ResidentDTOMapper {
   public:
    static ResidentDTO toDTO(const Resident& resident) {
        ResidentDTO dto;
        dto.id = resident.getId();
        dto.name = resident.getFullName();
        dto.phone = resident.getPhone();
        dto.apartment_number = resident.getApartmentId();
        return dto;
    }

    static Resident fromDTO(const ResidentDTO& dto) {
        return Resident(dto.id, dto.name, "", dto.apartment_number, false, dto.phone, 0, 0);
    }
};