#pragma once

#include <ctime>
#include <optional>
#include <string>

class Apartment {
   public:
    Apartment() = delete;
    Apartment(const int Id, const int BuildingId, const std::string& Number, bool IsOccupied, bool PetAllowed,
              const double CondoFee, const double AreaM2 = 0.0);

    // If a pointer is not null, update the member. Otherwise, keep the member's current value.
    void updateApartmentInfos(std::optional<int> Id = std::nullopt, std::optional<int> BuildingId = std::nullopt,
                              std::optional<std::string> Number = std::nullopt,
                              std::optional<bool> IsOccupied = std::nullopt,
                              std::optional<bool> PetAllowed = std::nullopt,
                              std::optional<double> CondoFee = std::nullopt,
                              std::optional<double> AreaM2 = std::nullopt);

    int getId() const;
    int getBuildingId() const;
    const std::string& getNumber() const;
    bool getIsOccupied() const;
    bool getPetAllowed() const;
    double getCondoFee() const;
    time_t getCreatedAt() const;
    time_t getUpdatedAt() const;
    double getAreaM2() const;

   private:
    void updateUpdatedAt();  // Every modification into this object should call this method

   private:
    int id;
    int buildingId;
    std::string number;  // e.g. "201", "13B"
    bool isOccupied;
    bool petAllowed;
    double condoFee;
    time_t createdAt;
    time_t updatedAt;
    double area_m2;

    // TODO: Include more properties
    // Residents
};