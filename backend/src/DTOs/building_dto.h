#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "apartment_dto.h"

class BuildingDTO {
   public:
    int id;
    std::string name;
    std::string address;
    int total_floors;
    std::vector<ApartmentDTO> apartments;
};

// JSON serialization for BuildingDTO
inline void to_json(nlohmann::json& j, const BuildingDTO& b) {
    j = nlohmann::json{{"id", b.id},
                       {"name", b.name},
                       {"address", b.address},
                       {"total_floors", b.total_floors},
                       {"apartments", b.apartments}};
}

inline void from_json(const nlohmann::json& j, BuildingDTO& b) {
    j.at("id").get_to(b.id);
    j.at("name").get_to(b.name);
    j.at("address").get_to(b.address);
    j.at("total_floors").get_to(b.total_floors);
    j.at("apartments").get_to(b.apartments);
}
