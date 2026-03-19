--- 004_Create_MaintenanceRequests.sql
--- Purpose:  Create MaintenanceRequests Table

CREATE TABLE IF NOT EXISTS MaintenanceRequests (
    id                  BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY
    ,resident_id        BIGINT NOT NULL REFERENCES Residents(id) ON DELETE CASCADE
    ,apartment_id       BIGINT NOT NULL REFERENCES Apartments(id) ON DELETE CASCADE
    ,description        TEXT NOT NULL
    ,status             TEXT NOT NULL DEFAULT 'open' CHECK (status IN ('open', 'in_progress', 'closed'))
    ,priority           INTEGER NOT NULL DEFAULT 2 CHECK (priority BETWEEN 1 AND 3)
    ,created_at         TIMESTAMP DEFAULT CURRENT_TIMESTAMP 
    ,updated_at         TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
