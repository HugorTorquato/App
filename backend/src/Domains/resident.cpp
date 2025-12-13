#include "resident.h"

Resident::Resident(const int Id, const std::string& FullName, const std::string& DocumentNumber, const int ApartmentId,
                   const bool IsOwner, const std::string& Phone, const time_t MoveInDate, const time_t MoveOutDate)
    : id(Id),
      fullName(FullName),
      documentNumber(DocumentNumber),
      apartmentId(ApartmentId),
      isOwner(IsOwner),
      phone(Phone),
      moveInDate(MoveInDate),
      moveOutDate(MoveOutDate) {
    createdAt = std::time(nullptr);
    updatedAt = createdAt;
}

void Resident::updateResidentInfos(std::optional<int> Id, std::optional<std::string> FullName,
                                   std::optional<std::string> DocumentNumber, std::optional<int> ApartmentId,
                                   std::optional<bool> IsOwner, std::optional<std::string> Phone,
                                   std::optional<time_t> MoveInDate, std::optional<time_t> MoveOutDate) {
    if (Id) this->id = *Id;
    if (FullName) this->fullName = *FullName;
    if (DocumentNumber) this->documentNumber = *DocumentNumber;
    if (ApartmentId) this->apartmentId = *ApartmentId;
    if (IsOwner) this->isOwner = *IsOwner;
    if (Phone) this->phone = *Phone;
    if (MoveInDate) this->moveInDate = *MoveInDate;
    if (MoveOutDate) this->moveOutDate = *MoveOutDate;
    this->updateUpdatedAt();
}

int Resident::getId() const { return id; }
const std::string& Resident::getFullName() const { return fullName; }
const std::string& Resident::getDocumentNumber() const { return documentNumber; }
int Resident::getApartmentId() const { return apartmentId; }
bool Resident::getIsOwner() const { return isOwner; }
const std::string& Resident::getPhone() const { return phone; }
time_t Resident::getMoveInDate() const { return moveInDate; }
time_t Resident::getMoveOutDate() const { return moveOutDate; }
time_t Resident::getCreatedAt() const { return createdAt; }
time_t Resident::getUpdatedAt() const { return updatedAt; }

void Resident::updateUpdatedAt() { updatedAt = std::time(nullptr); }