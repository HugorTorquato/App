# C++ API — Docker Development & Production Setup

This project uses **two Docker environments**:
- `dev` → for day-to-day coding and debugging
- `prod` → for building and running a release-optimized version of the API

---

## 1. Start Development Environment

### Build and run the dev container
```bash
# docker compose up -d dev
docker compose --profile dev up -d

# Access the running container

docker exec -it app_dev bash

# Now you can compile and run your API manually inside the container:

# cmake -B build -DCMAKE_BUILD_TYPE=Debug
# cmake --build build
# ./build/my_api

# To stop the container:

docker compose down


########## Build production (multi-stage) image

docker compose build prod

# Run the production container
docker compose up prod

# To rebuild and restart cleanly:

docker compose down
docker compose build prod
docker compose up prod

# 3. Container Management Cheatsheet
# List all containers	
docker ps -a
# Attach to running dev container	
docker exec -it app_dev bash
# Stop all containers	
docker compose down
# Remove all unused images
docker image prune -a
# Clean build cache
docker builder prune


## Run integration test stack independently
# --build to rebuild the image
docker compose --profile test up --build --abort-on-container-exit --exit-code-from tests

# No rebuild No prod restart Just tests again
# This is perfect while writing test code.
docker compose run --rm tests

# Build the production env so i can leave the api running
docker compose up prod --build

# Build the integration test container so i can run integration tests
docker compose run --build --rm tests

# Run local dev env with database
docker compose --profile dev up -d
# Service	Purpose
# postgres	database
# dev	your development shell

## Run locally in TEST mode (integration tests)
docker compose --profile test up --build
# What happens automatically:
# Postgres starts
# waits until healthy
# backend prod container starts
# waits until /health OK
# integration test container runs HTTP calls
# Docker shuts everything down

########## Access database commands

# Access the DB docker in a bash terminal
docker exec -it condo_db bash

# Connect to the database using the default condo_user
psql -U condo_user -d condo

#If you see the following in termianl you are connected
condo=#
