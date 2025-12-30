#pragma once
#include <nlohmann/json.hpp>
#include <string>

#include "../Utils/DomainEnums.h"

// TODO:
// - Be able to receive string for status and convert to enum

struct MaintenanceRequestDTO {
    int id;
    int resident_id;
    int apartment_id;
    std::string description;
    MaintenanceStatus status;  // Open, InProgress, Completed
    int priority;              // 1–5

    // When you write testing::Eq(inputDTO), GoogleMock looks for an operator== for that class. If it doesn't find one,
    // the compiler throws this "no match" error. Need to define how to consider two objects equal
    bool operator==(const MaintenanceRequestDTO& other) const {
        return id == other.id && resident_id == other.resident_id && apartment_id == other.apartment_id &&
               description == other.description && status == other.status && priority == other.priority;
    }
};

inline void to_json(nlohmann::json& j, const MaintenanceRequestDTO& r) {
    j = {{"id", r.id},
         {"resident_id", r.resident_id},
         {"apartment_id", r.apartment_id},
         {"description", r.description},
         {"status", r.status},
         {"priority", r.priority}};
}

inline void from_json(const nlohmann::json& j, MaintenanceRequestDTO& r) {
    j.at("id").get_to(r.id);
    j.at("resident_id").get_to(r.resident_id);
    j.at("apartment_id").get_to(r.apartment_id);
    j.at("description").get_to(r.description);
    j.at("status").get_to(r.status);
    j.at("priority").get_to(r.priority);
}