#!/bin/bash

# Release script for Fitness Tracker Desktop App

set -e

echo "=== FITNESS TRACKER RELEASE BUILD ==="
echo ""

# Get version from CMakeLists.txt
VERSION=$(grep "project(fitness-tracker VERSION" CMakeLists.txt | sed 's/.*VERSION \([0-9.]*\).*/\1/')
RELEASE_NAME="fitness-tracker-${VERSION}"
RELEASE_DIR="release/${RELEASE_NAME}"

echo "Building release for version: $VERSION"
echo ""

# Clean previous builds
echo "Cleaning previous builds..."
rm -rf build
echo ""

# Build release version
echo "Building release version..."
mkdir -p build
cd build

# Configure with release settings
echo "Configuring with release optimizations..."
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG" ..

# Build
echo "Compiling..."
make -j$(nproc)

cd ..

echo ""
echo "=== BUILD COMPLETE ==="
echo "Executable size:"
ls -lh build/bin/fitness-tracker
echo ""

# Create release package
echo "Creating release package..."
echo "Creating release package for version $VERSION..."

# Ensure we have a release build
if [ ! -f "build/bin/fitness-tracker" ]; then
    echo "Error: No build found. Build failed."
    exit 1
fi

# Create release directory
mkdir -p "$RELEASE_DIR"

# Copy executable
cp build/bin/fitness-tracker "$RELEASE_DIR/"

# Copy documentation
cp README.md "$RELEASE_DIR/"
cp LICENSE "$RELEASE_DIR/"

# Create desktop file for Linux
cat > "$RELEASE_DIR/fitness-tracker.desktop" << EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=Fitness Tracker
Comment=Desktop fitness tracking application
Exec=fitness-tracker
Icon=fitness-tracker
Categories=Health;Sports;
Terminal=false
EOF

# Create install script
cat > "$RELEASE_DIR/install.sh" << 'EOF'
#!/bin/bash

# Install script for Fitness Tracker

set -e

echo "Installing Fitness Tracker..."

# Determine installation directory
if [ "$EUID" -eq 0 ]; then
    INSTALL_DIR="/usr/local/bin"
    DESKTOP_DIR="/usr/share/applications"
    ICON_DIR="/usr/share/icons/hicolor/256x256/apps"
else
    INSTALL_DIR="$HOME/.local/bin"
    DESKTOP_DIR="$HOME/.local/share/applications"
    ICON_DIR="$HOME/.local/share/icons/hicolor/256x256/apps"
fi

# Create directories
mkdir -p "$INSTALL_DIR"
mkdir -p "$DESKTOP_DIR"
mkdir -p "$ICON_DIR"

# Install executable
cp fitness-tracker "$INSTALL_DIR/"
chmod +x "$INSTALL_DIR/fitness-tracker"

# Install desktop file
cp fitness-tracker.desktop "$DESKTOP_DIR/"

# Install icon (if available)
if [ -f "fitness-tracker.png" ]; then
    cp fitness-tracker.png "$ICON_DIR/"
fi

echo "Installation complete!"
echo "You can now run 'fitness-tracker' from anywhere."
echo "The application will also appear in your application menu."
EOF

chmod +x "$RELEASE_DIR/install.sh"

# Create uninstall script
cat > "$RELEASE_DIR/uninstall.sh" << 'EOF'
#!/bin/bash

# Uninstall script for Fitness Tracker

set -e

echo "Uninstalling Fitness Tracker..."

# Determine installation directory
if [ "$EUID" -eq 0 ]; then
    INSTALL_DIR="/usr/local/bin"
    DESKTOP_DIR="/usr/share/applications"
    ICON_DIR="/usr/share/icons/hicolor/256x256/apps"
else
    INSTALL_DIR="$HOME/.local/bin"
    DESKTOP_DIR="$HOME/.local/share/applications"
    ICON_DIR="$HOME/.local/share/icons/hicolor/256x256/apps"
fi

# Remove executable
rm -f "$INSTALL_DIR/fitness-tracker"

# Remove desktop file
rm -f "$DESKTOP_DIR/fitness-tracker.desktop"

# Remove icon
rm -f "$ICON_DIR/fitness-tracker.png"

echo "Uninstallation complete!"
EOF

chmod +x "$RELEASE_DIR/uninstall.sh"

# Create a simple run script for testing
cat > "$RELEASE_DIR/run.sh" << 'EOF'
#!/bin/bash

# Simple run script for testing the release

./fitness-tracker
EOF

chmod +x "$RELEASE_DIR/run.sh"

# Create tarball
cd release
tar -czf "${RELEASE_NAME}.tar.gz" "$RELEASE_NAME"
cd ..

echo ""
echo "=== RELEASE COMPLETE ==="
echo "Release package created: release/fitness-tracker-${VERSION}.tar.gz"
echo "Package contents:"
ls -la "$RELEASE_DIR/"
echo ""
echo "To test the release:"
echo "cd release/fitness-tracker-${VERSION} && ./run.sh"
echo ""
echo "To install:"
echo "cd release/fitness-tracker-${VERSION} && ./install.sh" 