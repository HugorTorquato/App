--- 20260316_004_Create_MaintenanceRequests.sql
--- Purpose:  Create MaintenanceRequests Table

CREATE TABLE IF NOT EXISTS MaintenanceRequests (
    id                  BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY
    ,resident_id        BIGINT NOT NULL REFERENCES Residents(id) ON DELETE CASCADE
    ,apartment_id       BIGINT NOT NULL REFERENCES Apartments(id) ON DELETE CASCADE
    ,description        TEXT NOT NULL
    ,status             TEXT NOT NULL DEFAULT 'open' CHECK (status IN ('open', 'in_progress', 'closed'))
    ,priority           TEXT NOT NULL DEFAULT 'medium' -- Possible values: 'low', 'medium', 'high'
    ,created_at         TIMESTAMP DEFAULT CURRENT_TIMESTAMP 
    ,updated_at         TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);