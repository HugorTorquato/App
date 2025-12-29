#pragma once

#include "../Domains/building.h"
#include "../building_dto.h"

class BuildingDTOMapper {
   public:
    static BuildingDTO toDTO(const Building& building) {
        BuildingDTO dto;
        dto.id = building.getId();
        dto.name = building.getName();
        dto.address = building.getAddress();
        dto.total_floors = building.getNumberOfFloors();
        // TODO : Map apartments
        return dto;
    }

    static Building fromDTO(const BuildingDTO& dto) {
        return Building(dto.id, dto.name, dto.address, dto.total_floors);
    }
};