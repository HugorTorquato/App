# PostgreSQL Integration Review and Implementation Plan

## 1) Branch review: what is already in place

Based on the current branch history and codebase, PostgreSQL integration was started and the project was refactored to support repository implementations selected by configuration.

### Delivered foundations

- **Repository implementation toggle (`REPOSITORY_IMPL`)** in app startup:
  - `InMemory` uses in-memory repositories.
  - Any other value falls back to PostgreSQL repositories.
- **Composition-root wiring per domain** (Resident, Apartment, Building, MaintenanceRequest) to switch repositories by mode.
- **PostgreSQL repository classes** for all four domains, with interfaces already respected.
- **`DbConnectionFactory` + `DbConfig::fromEnv()`** with strict env loading (`DB_HOST`, `DB_PORT`, `DB_NAME`, `DB_USER`, `DB_PASS`).
- **Migration executable target (`migrate`)** added to CMake and docker-compose flow.
- **Docker compose orchestration** for `postgres`, `migrate`, `prod`, and test runner.
- **Basic unit tests for `DbConnectionFactory`** (env loading, connection attempts, tx creation).

## 2) Current architecture (as implemented)

```text
HTTP (Crow routes)
  -> Controllers
    -> Services
      -> Repository interfaces (ports)
        -> Adapters:
             - InMemory repositories
             - Postgres repositories

Startup composition (main + module structs) selects adapter via REPOSITORY_IMPL.
```

### Runtime selection flow

1. `main.cpp` reads `REPOSITORY_IMPL` from environment.
2. Each `*Module` chooses concrete repository in `selectRepository(mode)`.
3. Service + controller are built on top of that repository implementation.
4. Routes are registered against controllers.

This is a valid **hexagonal/clean-ish layering** with infrastructure adapters behind repository interfaces.

## 3) Gaps blocking a real PostgreSQL connection

Even with the foundations, the branch is not yet fully connected to PostgreSQL in production behavior because:

1. **Postgres repositories are mostly placeholders**
   - CRUD methods return dummy values / empty results.
   - No SQL is executed in domain repositories.
2. **`DbConnectionFactory` is not integrated in repositories yet**
   - Repositories do not consume the factory for real transactions/queries.
3. **Migration executable is still TODO**
   - It validates env values and logs, but does not apply SQL files.
4. **No schema contract in backend migration pipeline**
   - `infra/init-scripts/init_db.sql` is currently empty in practice.
5. **Error mapping / transactional consistency missing**
   - No DB error translation (e.g., unique violation -> API-level conflict).
   - No explicit transaction boundaries for multi-step writes.
6. **Coverage gaps for integration behavior**
   - There are unit tests for factory bootstrapping, but little end-to-end verification of DB-backed repositories.

## 4) Recommended enhancements (priority order)

### P0 — Make PostgreSQL real

1. Implement SQL CRUD in all `Postgres*Repository` classes using `pqxx::work`.
2. Inject/hold a `DbConnectionFactory` in each Postgres repository.
3. Add row-to-domain mapping helpers in each repository.
4. Return created IDs from INSERT ... `RETURNING id`.

### P1 — Migrations and schema lifecycle

1. Implement migration runner in `backend/src/Migration/main.cpp`:
   - ensure `schema_migrations` table;
   - discover migration files (ordered);
   - run pending migrations in transaction;
   - record applied version.
2. Create first SQL migrations for tables:
   - buildings, apartments, residents, maintenance_requests;
   - foreign keys + indexes.
3. Make compose test profile fail fast when migration fails.

### P2 — Reliability and operability

1. Add structured DB error handling + service-level exception translation.
2. Add startup health dependency checks (DB reachable before serving).
3. Add connection/statement timeout settings (via env).
4. Add observability:
   - query-level debug logs (sanitized);
   - migration progress logs.

### P3 — Quality and developer UX

1. Add repository integration tests against ephemeral Postgres (docker profile).
2. Add contract tests that run both `InMemory` and `Postgres` implementations.
3. Add a concise backend README section with local/dev/test commands.
4. Normalize naming typos (`Postegres` -> `Postgres`) to reduce confusion.

## 5) Implementation plan (phased)

### Phase 1: Data model and migration baseline

- Define ER model and constraints.
- Add versioned SQL migration files (`001_*.sql`, `002_*.sql`, ...).
- Implement migration runner and verify idempotency.

**Exit criteria:** `docker compose --profile test up migrate` creates schema cleanly from zero.

### Phase 2: Repository persistence path

- Implement real SQL in `PostgresResidentRepository` first (pilot).
- Replicate pattern for Apartment, Building, MaintenanceRequest.
- Wire each repository with shared connection factory.

**Exit criteria:** CRUD for each domain passes integration tests on Postgres.

### Phase 3: Service/controller parity and consistency

- Ensure service behavior is identical between InMemory and Postgres.
- Add error mapping (not found, conflict, validation-like database constraints).
- Add transaction scopes for related writes.

**Exit criteria:** API behavior parity test suite passes for both repository modes.

### Phase 4: CI and operational hardening

- Run migrations + integration tests in CI on each PR.
- Add healthcheck endpoint extension (db status optional detail).
- Document rollback/recovery and seed strategy.

**Exit criteria:** CI proves migration + API + tests with Postgres by default.

## 6) Target architecture after completion

```text
[Client]
   |
[Crow Routes]
   |
[Controllers]
   |
[Services / Use Cases]
   |
[Repository Interfaces]
   |--------------------------|
   |                          |
[InMemory Adapters]     [Postgres Adapters]
                              |
                    [DbConnectionFactory]
                              |
                         [PostgreSQL]

Startup:
- REPOSITORY_IMPL=InMemory | Postgres
- Migration step runs before API in test/prod pipeline
```

This preserves the current separation of concerns while making PostgreSQL a first-class, testable adapter.


## 7) How to start implementation now (with Dockerfile adjustments)

If your immediate goal is to get a **real Postgres-backed vertical slice** working end-to-end, use this order:

1. **Prepare container/runtime reliability first**
   - Keep `backend/Dockerfile` multi-stage, but install runtime shared libs in the runtime image (`libpq5`, `libpqxx`, SSL certs/libs).
   - Keep `backend/Dockerfile.dev` as a pure dev shell image (toolchain + bind mount), not as a copy-from-build image.
   - In `infra/docker-compose.yml`, set DB env defaults to the `postgres` service to avoid blank env values in local runs.

2. **Implement one pilot repository fully (Resident)**
   - Add SQL for `save`, `findById`, `findAll`, `update`, `remove`.
   - Use `DbConnectionFactory` to open transaction objects (`pqxx::work`).
   - Return IDs using `INSERT ... RETURNING id`.

3. **Implement migration runner skeleton**
   - Create `schema_migrations` if missing.
   - Apply one baseline migration (`001_init.sql`) to create resident table first.
   - Store applied version.

4. **Run stack and validate manually**
   - `docker compose --profile test up --build migrate`
   - `docker compose --profile test up --build prod`
   - Hit `POST /v1/residents` and `GET /v1/residents/:id` to validate DB path.

5. **Scale pattern to other domains**
   - Repeat repository + SQL mapping for building, apartment, maintenance request.

### Minimal Docker-focused acceptance checklist

- Runtime image starts and can run both `./migrate` and `./condo_backend`.
- `migrate` exits 0 only when schema is in expected version.
- API starts with `REPOSITORY_IMPL=Postgres` and can persist/read at least one entity.
- `docker compose --profile test up --build --abort-on-container-exit --exit-code-from tests` passes baseline tests.