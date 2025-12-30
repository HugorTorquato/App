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

    // When you write testing::Eq(inputDTO), GoogleMock looks for an operator== for that class. If it doesn't find one,
    // the compiler throws this "no match" error. Need to define how to consider two objects equal
    bool operator==(const BuildingDTO& other) const {
        return id == other.id && name == other.name && address == other.address && total_floors == other.total_floors &&
               apartments == other.apartments;
    }
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
