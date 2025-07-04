#!/bin/bash

# Build script for Fitness Tracker Desktop App

set -e

echo "Building Fitness Tracker Desktop App..."

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build
make -j$(nproc)

echo "Build complete! Executable is in build/bin/fitness-tracker-desktop" 