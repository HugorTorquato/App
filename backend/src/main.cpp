// minimal Crow example (assumes crow headers in vendor/crow/include)
#include "crow.h" // or #include "crow.h" depending on header you used

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Hello from Crow backend!";
    });

    app.port(8080).multithreaded().run();
    return 0;
}
