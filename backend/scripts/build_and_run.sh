#!/bin/bash
set -e

# === CONFIG ===
BUILD_DIR="build"
EXECUTABLE_NAME="condo_backend"
SRC_DIR="/app"
INCLUDE_DIR="$CROW_INCLUDE_DIR"

echo "🔧 Building C++ Crow API..."

# Create build folder if not exists
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# Run CMake
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_STANDARD=20 \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      -DCROW_INCLUDE_DIR=$INCLUDE_DIR \
      ..

# Compile
make -j$(nproc)

# Run
echo "🚀 Starting Crow API..."
./$EXECUTABLE_NAME
