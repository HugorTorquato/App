#pragma once
#include <stdexcept>
#include <string>

struct ApiError : public std::runtime_error {
    int code;
    ApiError(int c, const std::string& m) : std::runtime_error(m), code(c) {}
};