#!/bin/bash

# Build script for Fitness Tracker Desktop App

set -e

echo "Building Fitness Tracker Desktop App..."

# Create build directory
mkdir -p build
cd build

# Configure with CMake (Debug build by default)
echo "Configuring for Debug build..."
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build
echo "Compiling..."
make -j$(nproc)

cd ..

echo "Build complete! Executable is in build/bin/fitness-tracker"
echo ""
echo "For release builds, use: ./release.sh" 