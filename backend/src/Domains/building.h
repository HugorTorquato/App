#pragma once

#include <ctime>
#include <string>

class Building {
   public:
    Building() = delete;
    Building(const int Id, const std::string& Name, const std::string& Address, const int NumberOfFloors);

    int getId() const;
    std::string getName() const;
    std::string getAddress() const;
    int getNumberOfFloors() const;
    time_t getCreatedAt() const;
    time_t getUpdatedAt() const;

    void updateUpdatedAt();  // Every modification into this object should call this method

   private:
    int id;
    std::string name;
    std::string address;
    int numberOfFloors;
    time_t createdAt;
    time_t updatedAt;
};
