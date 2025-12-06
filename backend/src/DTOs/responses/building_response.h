#pragma once

#include "../building_dto.h"

struct BuildingResponse {
    BuildingDTO data;
};

inline void to_json(nlohmann::json& j, const BuildingResponse& r) { j = nlohmann::json{{"building", r.data}}; }
