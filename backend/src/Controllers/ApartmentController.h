#pragma once

#include <crow.h>

#include "../Services/Apartment/IApartmentService.h"

class ApartmentController {
   public:
    ApartmentController(IApartmentService& service) : apartmentService(service) {}
    crow::response create(const crow::request& req);
    crow::response getById(int id);
    // crow::response listAll();

   private:
    IApartmentService& apartmentService;
};