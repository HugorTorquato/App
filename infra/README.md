# C++ API — Docker Development & Production Setup

This project uses **two Docker environments**:
- `dev` → for day-to-day coding and debugging
- `prod` → for building and running a release-optimized version of the API

---

## 1. Start Development Environment

### Build and run the dev container
```bash
docker compose up -d dev

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