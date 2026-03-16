#!/bin/bash
set -e

# === ARGUMENT PARSER ===
# Example: ./build_and_run.sh --clean --resetSchemas --tests=MigrationRunner
CLEAN_BUILD=false
RESET_SCHEMAS=false
TEST_FILTER=""

for arg in "$@"; do
    case "$arg" in
        --clean)         CLEAN_BUILD=true ;;
        --resetSchemas)  RESET_SCHEMAS=true ;;
        --tests=*)       TEST_FILTER="${arg#--tests=}" ;;
        *)               echo "Unknown argument: $arg"; exit 1 ;;
    esac
done

cd ..

echo "🔧 Formatting source code..."
find . -regex '.*\.\(cpp\|h\|hpp\)' -exec clang-format -i {} +

# === CONFIG ===
BUILD_DIR="build"
EXECUTABLE_NAME="bin/condo_backend"
EXECUTABLE_MIGRATION="bin/migrate"

echo "🔧 Building C++ Crow API..."

if [[ "$CLEAN_BUILD" == true ]]; then
    echo "🧹 Cleaning previous build..."
    rm -rf $BUILD_DIR
fi

mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  ..

make -j$(nproc)

# === RUN UNIT TESTS ===
echo "🧪 Running unit tests..."
if [[ -n "$TEST_FILTER" ]]; then
    echo "   Filter: $TEST_FILTER"
    ctest --output-on-failure -R "$TEST_FILTER"
else
    ctest --output-on-failure
fi

# === RUN MIGRATIONS ===
if [[ "$RESET_SCHEMAS" == true ]]; then
    ./$EXECUTABLE_MIGRATION --reset
else
    ./$EXECUTABLE_MIGRATION
fi

# === RUN SERVER ===
# echo "🚀 Starting Crow API..."
# ./$EXECUTABLE_NAME
