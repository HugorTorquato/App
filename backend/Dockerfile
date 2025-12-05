# backend/Dockerfile

# Stage 1 (build)
# Installs everything needed to compile (gcc, cmake, etc.).
# Compiles your C++ binary.

# Stage 2 (runtime)
# Starts from a clean Ubuntu image (no compiler).
# Copies only the built binary.
# The final image is much smaller and more secure.

# ==========
# Stage 1: Build
# ==========
FROM ubuntu:22.04 AS build

# Non-interactive mode for apt
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies for building C++
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    git \
    gdb \
    vim \
    wget \
    clang \
    ca-certificates \
    curl \
    libboost-all-dev \
    libssl-dev \
    libpq-dev \
    libpqxx-dev \
    libasio-dev \
    nlohmann-json3-dev \
    clang-format \
    && rm -rf /var/lib/apt/lists/*

# Set workdir
WORKDIR /app

# Copy the source code
COPY . .

# Create a build directory and compile
WORKDIR /app/backend

RUN rm -rf build && mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc) && \
    ctest --output-on-failure


# ==========
# Stage 2: Runtime
# ==========
FROM ubuntu:22.04 AS runtime

WORKDIR /app

# Copy the compiled binary from build stage
COPY --from=build /app/backend/build/bin/condo_backend .


# Expose backend port
EXPOSE 8080

# Define the startup command
CMD ["./condo_backend"]
