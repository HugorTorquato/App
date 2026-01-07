#pragma once
#include <cstdlib>
#include <string>

namespace Env {
inline std::string getEnv(const char* key, const char* defaultValue = "") {
    const char* value = std::getenv(key);
    if (!value) return std::string(defaultValue);
    return std::string(value);
}
}  // namespace Env