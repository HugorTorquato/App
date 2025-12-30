#pragma once

#include <crow.h>

#include "../Services/MaintenanceRequest/IMaintenanceRequestService.h"

class MaintenanceRequestController {
   public:
    MaintenanceRequestController(IMaintenanceRequestService& service) : maintenanceRequestService(service) {}
    crow::response create(const crow::request& req);
    crow::response getById(int id);
    // crow::response listAll();

   private:
    IMaintenanceRequestService& maintenanceRequestService;
};