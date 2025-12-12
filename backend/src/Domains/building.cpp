#include "building.h"

#include "../Utils/Logger.h"

Building::Building(const int Id, const std::string& Name, const std::string& Address, const int NumberOfFloors)
    : id(Id), name(Name), address(Address), numberOfFloors(NumberOfFloors) {
    Logger::info("Build constructor");
    createdAt = std::time(nullptr);
    updatedAt = createdAt;
}

int Building::getId() const { return id; }
std::string Building::getName() const { return name; }
std::string Building::getAddress() const { return address; }
int Building::getNumberOfFloors() const { return numberOfFloors; }
time_t Building::getCreatedAt() const { return createdAt; }
time_t Building::getUpdatedAt() const { return updatedAt; }

void Building::updateUpdatedAt() { updatedAt = std::time(nullptr); }
