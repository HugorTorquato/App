#pragma once

#include <nlohmann/json.hpp>
#include <string>

class ApartmentDTO {
   public:
    int id;
    std::string number;
    double area_m2;

    // When you write testing::Eq(inputDTO), GoogleMock looks for an operator== for that class. If it doesn't find one,
    // the compiler throws this "no match" error. Need to define how to consider two objects equal
    bool operator==(const ApartmentDTO& other) const {
        return id == other.id && number == other.number && area_m2 == other.area_m2;
    }
};

inline void to_json(nlohmann::json& j, const ApartmentDTO& r) {
    j = {{"id", r.id}, {"number", r.number}, {"area_m2", r.area_m2}};
}

inline void from_json(const nlohmann::json& j, ApartmentDTO& r) {
    j.at("id").get_to(r.id);
    j.at("number").get_to(r.number);
    j.at("area_m2").get_to(r.area_m2);
}
