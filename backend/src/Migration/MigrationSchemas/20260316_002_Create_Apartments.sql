--- 002_Create_Apartments.sql
--- Purpose:  Create Apartments Table

CREATE TABLE IF NOT EXISTS Apartments (
    id                  BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY
    ,building_id        BIGINT NOT NULL REFERENCES Buildings(id) ON DELETE CASCADE
    ,number             TEXT NOT NULL
    ,isOccupied         BOOLEAN NOT NULL DEFAULT FALSE
    ,petAllowed         BOOLEAN NOT NULL DEFAULT FALSE
    ,condoFee           NUMERIC(10, 2) NOT NULL
    ,area               NUMERIC(10, 2) NOT NULL
    ,owner              TEXT NOT NULL
    ,created_at         TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    ,updated_at         TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
