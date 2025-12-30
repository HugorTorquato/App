#pragma once

#include <crow.h>

#include "../Services/Resident/IResidentService.h"

class ResidentController {
   public:
    ResidentController(IResidentService& service) : residentService(service) {}
    crow::response create(const crow::request& req);
    crow::response getById(int id);
    // crow::response listAll();

   private:
    IResidentService& residentService;
};