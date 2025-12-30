#pragma once

#include <crow.h>

#include "../Services/Building/IBuildingService.h"

class BuildingController {
   public:
    BuildingController(IBuildingService& service) : buildingService(service) {}
    crow::response create(const crow::request& req);
    crow::response getById(int id);
    // crow::response listAll();

   private:
    IBuildingService& buildingService;
};