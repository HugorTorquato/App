--- 001_Create_Residents.sql
--- Purpose:  Create Residents Table

CREATE TABLE Residents (
    id                  BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    full_name           TEXT NOT NULL,
    document            TEXT NOT NULL,
    apartment_id        TEXT NOT NULL,
    is_owner            BOOLEAN NOT NULL, -- TODO: Remove this field as the appartment ownership will be managed in the Apartments table
    phone_number        TEXT,
    move_in_date        DATE NOT NULL,
    move_out_date       DATE,
    created_at          TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at          TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);