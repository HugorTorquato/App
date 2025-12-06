#pragma once
#include <nlohmann/json.hpp>
#include <string>

struct CreateBuildingRequest {
    std::string name;
    std::string address;
    int total_floors;
};

inline void from_json(const nlohmann::json& j, CreateBuildingRequest& r) {
    j.at("name").get_to(r.name);
    j.at("address").get_to(r.address);
    j.at("total_floors").get_to(r.total_floors);
}
