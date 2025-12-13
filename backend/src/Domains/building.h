#pragma once

#include <ctime>
#include <optional>
#include <string>

class Building {
   public:
    Building() = delete;
    Building(const int Id, const std::string& Name, const std::string& Address, const int NumberOfFloors);

    int getId() const;
    const std::string& getName() const;
    const std::string& getAddress() const;
    int getNumberOfFloors() const;
    time_t getCreatedAt() const;
    time_t getUpdatedAt() const;

    // If a pointer is not null, update the member. Otherwise, keep the member's current value.
    void updateBuildInfos(std::optional<int> Id = std::nullopt, std::optional<std::string> Name = std::nullopt,
                          std::optional<std::string> Address = std::nullopt,
                          std::optional<int> NumberOfFloors = std::nullopt);

   private:
    void updateUpdatedAt();  // Every modification into this object should call this method

   private:
    int id;
    std::string name;
    std::string address;
    int numberOfFloors;
    time_t createdAt;
    time_t updatedAt;
};
