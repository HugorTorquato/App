# Start from small but feature-complete distro to avoid missing libs
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install build prerequisites
RUN apt-get update && apt-get install -y \
    build-essential cmake g++ git wget curl \
    libboost-all-dev libssl-dev libpq-dev libpqxx-dev libasio-dev \
    && rm -rf /var/lib/apt/lists/*


# Copy project files
WORKDIR /app
COPY . /app

# If you used submodule, make sure it's initialized; otherwise the headers should be present
# Build
RUN cmake -B build -S . && cmake --build build -- -j$(nproc)

EXPOSE 8080
CMD ["./build/condo_backend"]

# # Environment variables for Crow
# ENV CROW_INCLUDE_DIR=/opt/crow/include

# # Default command
# CMD ["/bin/bash"]


# Why these choices

# Ubuntu base ensures predictable packages.

# We install libpq-dev and libpqxx-dev so compiled binary can link against Postgres client libs.

# Building inside Docker gives a reproducible dev image identical to CI.


