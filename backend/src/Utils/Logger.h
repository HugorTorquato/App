#pragma once
#include <iostream>

class Logger {
   public:
    // TODO: Evaluate if it's better to implement a logger class or use an existing one
    static void info(const std::string& msg) { std::cout << "[INFO] " << msg << "\n"; }
};