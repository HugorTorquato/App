#pragma once

#include "../../Domains/maintenanceRequest.h"
#include "../maintenanceRequest_dto.h"

class MaintenanceRequestDTOMapper {
   public:
    static MaintenanceRequestDTO toDTO(const MaintenanceRequest& request);
    static MaintenanceRequest fromDTO(const MaintenanceRequestDTO& dto);
};

inline MaintenanceRequestDTO MaintenanceRequestDTOMapper::toDTO(const MaintenanceRequest& request) {
    MaintenanceRequestDTO dto;
    dto.id = request.getId();
    dto.resident_id = request.getResidentId();
    dto.apartment_id = request.getApartmentId();
    dto.description = request.getDescription();
    dto.status = request.getStatus();
    dto.priority = request.getPriority();
    return dto;
}

inline MaintenanceRequest MaintenanceRequestDTOMapper::fromDTO(const MaintenanceRequestDTO& dto) {
    return MaintenanceRequest(dto.id, dto.resident_id, dto.apartment_id, dto.description, dto.status, dto.priority);
}