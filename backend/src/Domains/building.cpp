#include "building.h"

Building::Building(const int Id, const std::string& Name, const std::string& Address, const int NumberOfFloors,
                   const std::vector<Apartment>& Apartments)
    : id(Id), name(Name), address(Address), numberOfFloors(NumberOfFloors), apartments(Apartments) {
    createdAt = std::time(nullptr);
    updatedAt = createdAt;
}

void Building::updateBuildInfos(std::optional<int> Id, std::optional<std::string> Name,
                                std::optional<std::string> Address, std::optional<int> NumberOfFloors,
                                std::optional<std::vector<Apartment>> Apartments) {
    if (Id) this->id = *Id;
    if (Name) this->name = *Name;
    if (Address) this->address = *Address;
    if (NumberOfFloors) this->numberOfFloors = *NumberOfFloors;
    if (Apartments) this->apartments = *Apartments;
    this->updateUpdatedAt();
}

int Building::getId() const { return id; }
const std::string& Building::getName() const { return name; }
const std::string& Building::getAddress() const { return address; }
int Building::getNumberOfFloors() const { return numberOfFloors; }
time_t Building::getCreatedAt() const { return createdAt; }
time_t Building::getUpdatedAt() const { return updatedAt; }
const std::vector<Apartment>& Building::getApartments() const { return apartments; }

void Building::updateUpdatedAt() { updatedAt = std::time(nullptr); }
