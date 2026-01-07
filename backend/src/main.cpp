// minimal Crow example (assumes crow headers in vendor/crow/include)
#include "../../vendor/crow/include/crow.h"  // or #include "crow.h" depending on header you used
#include "Composition/Apartment_composition.h"
#include "Composition/Building_composition.h"
#include "Composition/MaintenanceRequest_composition.h"
#include "Composition/Resident_composition.h"
#include "Routes/Utils/General_API_Info_V1.h"
#include "Routes/v1/Apartment_routes.h"
#include "Routes/v1/Building_routes.h"
#include "Routes/v1/MaintenanceRequest_routes.h"
#include "Routes/v1/Resident_routes.h"
#include "Utils/Logger.h"

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() { return "Hello from Crow backend!"; });

    CROW_ROUTE(app, "/health")([] { return "ok"; });

    registerGeneralAPIInfoV1(app);

    // Dependencies rules - This instantiat each domain with the respective repositories, services and controllers
    // const char* mode = std::getenv("REPOSITORY_IMPL");
    const char* mode = "InMemory";
    ResidentModule resident(mode);
    ApartmentModule apartment(mode);
    BuildingModule building(mode);
    MaintenanceRequestModule maintenanceRequest(mode);
    // Routes
    registerResidentRoutes(app, resident.controller);
    registerApartmentRoutes(app, apartment.controller);
    registerBuildingRoutes(app, building.controller);
    registerMaintenanceRequestRoutes(app, maintenanceRequest.controller);

    Logger::info("Starting Crow server on port 8080");
    app.port(8080).multithreaded().run();
    return 0;
}
