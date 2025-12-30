## Depends on
DTOs
Mappers
Services

## Should not depend on
DB
Entity 
Crow

## Main Responsibilities

A controller:

Receives HTTP request

Parses

path params

query params

JSON body

Validates inputs (syntactic, not business)

Calls the appropriate service

Converts service result to DTO / JSON

Returns HTTP response

That’s it.

If a controller starts making decisions or business rules → something is wrong.

## What controllers must NOT do

They should never:

talk directly to the database

create or modify domain entities directly

contain business rules

orchestrate multiple entities with domain rules

perform cross-entity validation

contain transaction logic

Those belong to application services.

Controllers are basically:

request → dto → service → dto → response