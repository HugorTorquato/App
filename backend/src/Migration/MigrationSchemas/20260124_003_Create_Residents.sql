--- 003_Create_Residents.sql
--- Purpose:  Create Residents Table

CREATE TABLE IF NOT EXISTS Residents (
    id                  BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY
    ,apartment_id        BIGINT NOT NULL REFERENCES Apartments(id) ON DELETE CASCADE
    ,full_name           TEXT NOT NULL
    ,document            TEXT NOT NULL
    ,phone_number        TEXT
    ,move_in_date        DATE NOT NULL
    ,move_out_date       DATE
    ,created_at          TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    ,updated_at          TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
