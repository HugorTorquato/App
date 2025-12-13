#pragma once

#include <ctime>
#include <optional>
#include <string>

class Resident {
   public:
    Resident() = delete;
    Resident(const int Id, const std::string& FullName, const std::string& DocumentNumber, const int ApartmentId,
             const bool IsOwner, const std::string& Phone, const time_t MoveInDate, const time_t MoveOutDate);

    // If a pointer is not null, update the member. Otherwise, keep the member's current value.
    void updateResidentInfos(std::optional<int> Id = std::nullopt, std::optional<std::string> FullName = std::nullopt,
                             std::optional<std::string> DocumentNumber = std::nullopt,
                             std::optional<int> ApartmentId = std::nullopt, std::optional<bool> IsOwner = std::nullopt,
                             std::optional<std::string> Phone = std::nullopt,
                             std::optional<time_t> MoveInDate = std::nullopt,
                             std::optional<time_t> MoveOutDate = std::nullopt);

    int getId() const;
    const std::string& getFullName() const;
    const std::string& getDocumentNumber() const;
    int getApartmentId() const;
    bool getIsOwner() const;
    const std::string& getPhone() const;
    time_t getMoveInDate() const;
    time_t getMoveOutDate() const;
    time_t getCreatedAt() const;
    time_t getUpdatedAt() const;

   private:
    void updateUpdatedAt();  // Every modification into this object should call this method

   private:
    int id;
    std::string fullName;
    std::string documentNumber;  // CPF or ID
    int apartmentId;
    bool isOwner;
    std::string phone;
    time_t moveInDate;
    time_t moveOutDate;
    time_t createdAt;
    time_t updatedAt;
};
