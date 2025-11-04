# Backend Development Environment — Docker + CMake + Crow

This document tracks all commands and setup steps for developing the **C++ Crow backend** inside a Docker container.  
The current setup uses a **development container** that runs indefinitely, so you can attach to it, build, and run the API manually.

---

## 🐳 1. Build the Development Image

Build the image defined in `backend/Dockerfile.dev`:

```bash
docker build -t cpp-dev -f backend/Dockerfile .

# Run container and keep it alive
docker run -d --name cpp-dev-container -v ${PWD}/backend:/app cpp-dev