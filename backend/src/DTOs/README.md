## What a DTO Actually Is

     DTO (Data Transfer Object) is a simple struct/class used to:

        Represent request or response data

        Carry data between layers

        Have no logic (just fields)

        Be easily serializable to/from JSON

    DTOs are not entities and not tied to DB schema.

    They are specifically for API ↔ Controller ↔ Service communication.

    DTO ≠ Entity
    DTO = for API
    Entity = for business domain model / DB

    DTO = Data Transfer Object: dumb struct that carries data between layers (HTTP ↔ Controller ↔ Service ↔ Repo).

    No business logic inside DTOs. Validation/constraints live in Controller/Service.

    DTOs are the canonical shape of API input and output — stable, versioned, explicit.
    
    Notes

    to_json and from_json let nlohmann::json automatically convert: json j = dto; and dto = j.get<DTO>();

    Use at() in from_json when field is required (throws out_of_range), or value("field", default) for optional fields.

    STEPS:
    1 - json j (input)

    2 - AptoDTO dto = j.get(AptoDTO)

    3 - send to server and work with info as a class

    4 - convert to send back

    5 - json responseJSON = dto




## DTO Naming Convention

### Request DTOs

    CreateBuildingRequest

    UpdateApartmentRequest

### Response DTOs

    BuildingResponse

    ResidentResponse

    ApartmentSummaryResponse

### Shared / internal DTOs

    ApartmentDTO

    ResidentDTO


## How to Structure DTO Files
    /src/app/models/dto
        building_dto.h
        apartment_dto.h
        resident_dto.h
        requests/
            create_building_request.h
            update_building_request.h
        responses/
            building_response.h
            apartment_response.h

    Keep DTOs small and isolated.


## Todo:

### Use <optional>
    When we don't requires an argument to be passed to the API

## Practical checklist — implement today

    Create dto/ files for BuildingDTO, CreateBuildingRequest, UpdateBuildingRequest.

    Add to_json/from_json functions for each.

    Implement mapper functions dtoToEntity and entityToDto.

    Update controller to parse request JSON → Request DTO and call service.

    Update service to accept DTO, validate business rules, call repo, return DTO.

    Write unit tests: (a) DTO JSON roundtrip, (b) Service create test with mock repo.
