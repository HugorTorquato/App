#include "maintenanceRequest.h"

MaintenanceRequest::MaintenanceRequest(const int Id, const int ResidentId, const int ApartmentId,
                                       const std::string& Description, MaintenanceStatus Status, const int Priority)
    : id(Id),
      residentId(ResidentId),
      apartmentId(ApartmentId),
      description(Description),
      status(Status),
      priority(Priority) {
    createdAt = std::time(nullptr);
    updatedAt = createdAt;
}

void MaintenanceRequest::updateMaintenanceRequestInfos(std::optional<int> Id, std::optional<int> ResidentId,
                                                       std::optional<int> ApartmentId,
                                                       std::optional<std::string> Description,
                                                       std::optional<MaintenanceStatus> Status,
                                                       std::optional<int> Priority) {
    if (Id) this->id = *Id;
    if (ResidentId) this->residentId = *ResidentId;
    if (ApartmentId) this->apartmentId = *ApartmentId;
    if (Description) this->description = *Description;
    if (Status) this->status = *Status;
    if (Priority) this->priority = *Priority;
    this->updateUpdatedAt();
}

int MaintenanceRequest::getId() const { return id; }
int MaintenanceRequest::getResidentId() const { return residentId; }
int MaintenanceRequest::getApartmentId() const { return apartmentId; }
const std::string& MaintenanceRequest::getDescription() const { return description; }
MaintenanceStatus MaintenanceRequest::getStatus() const { return status; }
int MaintenanceRequest::getPriority() const { return priority; }
time_t MaintenanceRequest::getCreatedAt() const { return createdAt; }
time_t MaintenanceRequest::getUpdatedAt() const { return updatedAt; }

void MaintenanceRequest::updateUpdatedAt() { updatedAt = std::time(nullptr); }
