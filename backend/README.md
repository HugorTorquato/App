# Backend Development Environment — Docker + CMake + Crow

This document tracks all commands and setup steps for developing the **C++ Crow backend** inside a Docker container.  
The current setup uses a **development container** that runs indefinitely, so you can attach to it, build, and run the API manually.

---

## 1. Build the Development Image

Build the image defined in `backend/Dockerfile.dev`:

```bash
docker build -t cpp-dev -f backend/Dockerfile .
# -t cpp-dev: names the image (cpp-dev)
# -f backend/Dockerfile.dev: tells Docker which file to use
# The image includes compilers, CMake, gdb, and basic tools.

## Run container and keep it alive
docker run -d --name cpp-dev-container -v ${PWD}/backend:/app cpp-dev

# Explanation:
# -d: detached mode (runs in the background)
# --name: container name (easier to reference later)
# -v ${PWD}/backend:/app: mounts your local backend code into /app inside the container
# cpp-dev: image name
# The container runs indefinitely (tail -f /dev/null), so it’s always ready for you to connect and build.

## Now you can open a shell inside it:

docker exec -it cpp-dev-container bash

## Inside:

cd /app
mkdir build && cd build
cmake ..
make
./your_app

# mkdir -p build: creates a separate build directory (out-of-source build)
# cmake ..: generates Makefiles from your CMakeLists.txt
# make: compiles the project

## Useful Container Commands
Action	Command
Stop container	                   docker stop cpp-dev-container
Start container again	           docker start cpp-dev-container
Remove container	               docker rm -f cpp-dev-container
Rebuild image	                   docker build -t cpp-dev -f backend/Dockerfile.dev .
Remove image	                   docker rmi cpp-dev
List running containers	           docker ps
Open new terminal in container	   docker exec -it cpp-dev-container bash