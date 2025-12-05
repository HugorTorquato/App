// minimal Crow example (assumes crow headers in vendor/crow/include)
#include "../../vendor/crow/include/crow.h" // or #include "crow.h" depending on header you used
#include "Utils/Logger.h"

#include "Routes/Utils/General_API_Info_V1.h"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Hello from Crow backend!";
    });

    registerGeneralAPIInfoV1(app);

    Logger::info("Starting Crow server on port 8080");
    app.port(8080).multithreaded().run();
    return 0;
}
