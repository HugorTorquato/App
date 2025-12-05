# Routes

Thin mapping from HTTP to controllers

**Files:** `src/app/routes/*.h`

## Responsibility

- Accept HTTP method + path
- Extract path params (e.g., `<int>`)
- Forward the request to a controller method (no business logic)

## Why thin routes?

- Keeps HTTP routing independent from business logic
- Easier to test controllers/services (call them directly in unit tests)

## Pitfall to avoid

Don't parse or validate heavy logic here — keep controllers to do that.

## Getting started

Start with 2 routes: `building` and `residents` to represent both build info and residents of that building.