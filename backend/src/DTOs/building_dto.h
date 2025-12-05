#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class ApartmentDTO {
public:
    int id;
    std::string number;
    double area_m2;
};

class BuildingDTO {
public:
    int id;
    std::string name;
    std::string address;
    int total_floors;
    std::vector<ApartmentDTO> apartments;
};

// JSON serialization for ApartmentDTO
inline void to_json(nlohmann::json& j, const ApartmentDTO& a) {
    j = nlohmann::json{
        {"id", a.id},
        {"number", a.number},
        {"area_m2", a.area_m2}
    };
}

inline void from_json(const nlohmann::json& j, ApartmentDTO& a) {
    j.at("id").get_to(a.id);
    j.at("number").get_to(a.number);
    j.at("area_m2").get_to(a.area_m2);
}

// JSON serialization for BuildingDTO
inline void to_json(nlohmann::json& j, const BuildingDTO& b) {
    j = nlohmann::json{
        {"id", b.id},
        {"name", b.name},
        {"address", b.address},
        {"total_floors", b.total_floors},
        {"apartments", b.apartments}
    };
}

inline void from_json(const nlohmann::json& j, BuildingDTO& b) {
    j.at("id").get_to(b.id);
    j.at("name").get_to(b.name);
    j.at("address").get_to(b.address);
    j.at("total_floors").get_to(b.total_floors);
    j.at("apartments").get_to(b.apartments);
}
