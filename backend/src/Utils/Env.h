#pragma once
#include <cstdlib>
#include <string>

#include "Logger.h"

namespace Env {
inline std::string getEnv(const char* key, const char* defaultValue = "") {
    const char* value = std::getenv(key);
    if (!value) {
        Logger::info("Env::getEnv - Key not found: " + std::string(key) + ", using default: " + std::string(defaultValue));
        return std::string(defaultValue);
    }
    Logger::info("Env::getEnv - Key: " + std::string(key) + ", Value: " + std::string(value));
    return std::string(value);
}
}  // namespace Env