#pragma once

#include "../Domains/building.h"
#include "../apartment_dto.h"
#include "../building_dto.h"

namespace {
std::vector<ApartmentDTO> convertApartmentVectorToApartmentDTOVector(const Building& building) {
    auto apartments = building.getApartments();
    std::vector<ApartmentDTO> aptDTOs;
    for (const auto& apt : apartments) {
        ApartmentDTO aptDTO;
        aptDTO.id = apt.getId();
        aptDTO.number = apt.getNumber();
        aptDTO.area_m2 = apt.getAreaM2();
        aptDTOs.push_back(aptDTO);
    }

    return aptDTOs;
}
}  // namespace

class BuildingDTOMapper {
   public:
    static BuildingDTO toDTO(const Building& building) {
        BuildingDTO dto;
        dto.id = building.getId();
        dto.name = building.getName();
        dto.address = building.getAddress();
        dto.total_floors = building.getNumberOfFloors();
        dto.apartments = convertApartmentVectorToApartmentDTOVector(building);
        return dto;
    }

    static Building fromDTO(const BuildingDTO& dto) {
        // TODO: Handle apartments mapping if needed
        // Not sure if needed, as apartments would be added in another flow
        return Building(dto.id, dto.name, dto.address, dto.total_floors, {});
    }
};