#pragma once
#include <nlohmann/json.hpp>
#include <string>

struct ResidentDTO {
    int id;
    std::string name;
    std::string phone;
    std::string apartment_number;

    // When you write testing::Eq(inputDTO), GoogleMock looks for an operator== for that class. If it doesn't find one,
    // the compiler throws this "no match" error. Need to define how to consider two objects equal
    bool operator==(const ResidentDTO& other) const {
        return id == other.id && name == other.name && phone == other.phone &&
               apartment_number == other.apartment_number;
    }
};

inline void to_json(nlohmann::json& j, const ResidentDTO& r) {
    j = {{"id", r.id}, {"name", r.name}, {"phone", r.phone}, {"apartment_number", r.apartment_number}};
}

inline void from_json(const nlohmann::json& j, ResidentDTO& r) {
    j.at("id").get_to(r.id);
    j.at("name").get_to(r.name);
    j.at("phone").get_to(r.phone);
    j.at("apartment_number").get_to(r.apartment_number);
}
