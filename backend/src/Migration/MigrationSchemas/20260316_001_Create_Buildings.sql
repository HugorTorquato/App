--- 002_Create_Buildings.sql
--- Purpose:  Create Buildings Table

CREATE TABLE IF NOT EXISTS Buildings (
    id                  BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY
    ,name               TEXT NOT NULL
    ,address            TEXT NOT NULL
    ,numverOfFloors     INT NOT NULL
    ,created_at         TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    ,updated_at         TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);