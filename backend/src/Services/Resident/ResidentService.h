#pragma once

#include "IResidentService.h"

class ResidentService : public IResidentService {
   public:
    explicit ResidentService(IResidentRepository& repo) : repository(repo) {}

    int createResident(const ResidentDTO& resident);
    std::optional<ResidentDTO> getResidentById(int residentId);

    void updateMoveOutResident(int residentId, int moveOutDate);

   private:
    IResidentRepository& repository;
};