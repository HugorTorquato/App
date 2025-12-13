#include "apartment.h"

Apartment::Apartment(const int Id, const int BuildingId, const std::string& Number, bool IsOccupied, bool PetAllowed,
                     const double CondoFee)
    : id(Id),
      buildingId(BuildingId),
      number(Number),
      isOccupied(IsOccupied),
      petAllowed(PetAllowed),
      condoFee(CondoFee) {
    createdAt = std::time(nullptr);
    updatedAt = createdAt;
}

void Apartment::updateApartmentInfos(std::optional<int> Id, std::optional<int> BuildingId,
                                     std::optional<std::string> Number, std::optional<bool> IsOccupied,
                                     std::optional<bool> PetAllowed, std::optional<double> CondoFee) {
    if (Id) this->id = *Id;
    if (BuildingId) this->buildingId = *BuildingId;
    if (Number) this->number = *Number;
    if (IsOccupied) this->isOccupied = *IsOccupied;
    if (PetAllowed) this->petAllowed = *PetAllowed;
    if (CondoFee) this->condoFee = *CondoFee;
    this->updateUpdatedAt();
}

int Apartment::getId() const { return id; }
int Apartment::getBuildingId() const { return buildingId; }
const std::string& Apartment::getNumber() const { return number; }
bool Apartment::getIsOccupied() const { return isOccupied; }
bool Apartment::getPetAllowed() const { return petAllowed; }
double Apartment::getCondoFee() const { return condoFee; }
time_t Apartment::getCreatedAt() const { return createdAt; }
time_t Apartment::getUpdatedAt() const { return updatedAt; }

void Apartment::updateUpdatedAt() { updatedAt = std::time(nullptr); }