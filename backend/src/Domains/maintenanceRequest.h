#pragma once

#include <ctime>
#include <optional>
#include <string>

class MaintenanceRequest {
   public:
    enum class MaintenanceStatus { Open, InProgress, Completed };

    MaintenanceRequest() = delete;
    MaintenanceRequest(const int Id, const int ResidentId, const int ApartmentId, const std::string& Description,
                       MaintenanceStatus Status, const int Priority);

    // If a pointer is not null, update the member. Otherwise, keep the member's current value.
    void updateMaintenanceRequestInfos(std::optional<int> Id = std::nullopt,
                                       std::optional<int> ResidentId = std::nullopt,
                                       std::optional<int> ApartmentId = std::nullopt,
                                       std::optional<std::string> Description = std::nullopt,
                                       std::optional<MaintenanceStatus> Status = std::nullopt,
                                       std::optional<int> Priority = std::nullopt);

    int getId() const;
    int getResidentId() const;
    int getApartmentId() const;
    const std::string& getDescription() const;
    MaintenanceStatus getStatus() const;
    int getPriority() const;
    time_t getCreatedAt() const;
    time_t getUpdatedAt() const;

   private:
    void updateUpdatedAt();  // Every modification into this object should call this method

   private:
    int id;
    int residentId;
    int apartmentId;
    std::string description;
    MaintenanceStatus status;  // Open, InProgress, Completed
    int priority;              // 1–5
    time_t createdAt;
    time_t updatedAt;
};
