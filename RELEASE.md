# Release Build Guide

This document explains how to build and package the Fitness Tracker application for release.

## Debug vs Release Builds

### Debug Build (Default)
```bash
./build.sh
```

**Characteristics:**
- Includes debug symbols
- No optimizations
- Larger executable size
- Better for development and debugging
- Slower performance

### Release Build
```bash
./release.sh
```

**Characteristics:**
- Optimized with `-O3` flag
- Debug symbols stripped (`-s` flag)
- Smaller executable size
- Better performance
- `NDEBUG` macro defined
- Link-time optimization enabled
- Automatically creates release package

## Building for Release

### Single Command Release Build
```bash
./release.sh
```

This command:
1. Cleans previous builds
2. Builds optimized release version
3. Creates distributable package
4. Shows package location and usage instructions

The release package is created in the `release/` directory.

## Release Package Contents

The release package includes:
- Optimized executable
- README.md
- LICENSE
- Desktop file for Linux integration
- Install script
- Uninstall script
- Run script for testing

## Installation

### System-wide Installation (requires sudo)
```bash
cd release/fitness-tracker-0.1.0
sudo ./install.sh
```

### User Installation (recommended)
```bash
cd release/fitness-tracker-0.1.0
./install.sh
```

## Testing the Release

### Test the Package
```bash
cd release/fitness-tracker-0.1.0
./run.sh
```

### Test the Installed Version
```bash
fitness-tracker
```

## Release Checklist

Before creating a release:

- [ ] Update version number in `CMakeLists.txt`
- [ ] Test debug build: `./build.sh`
- [ ] Create release: `./release.sh`
- [ ] Test the release package: `cd release/fitness-tracker-* && ./run.sh`
- [ ] Verify all features work in release build
- [ ] Check executable size and performance
- [ ] Test installation and uninstallation
- [ ] Update documentation if needed

## Performance Comparison

| Build Type | Executable Size | Performance | Use Case |
|------------|----------------|-------------|----------|
| Debug | ~2-3MB | Slower | Development |
| Release | ~1-1.5MB | Faster | Distribution |

## Troubleshooting

### Common Issues

1. **Build fails with release flags**
   - Ensure you have a recent compiler
   - Try building debug first: `./build.sh`

2. **Executable doesn't run on target system**
   - Check Qt dependencies
   - Use `ldd` to check missing libraries
   - Consider static linking for Qt (advanced)

3. **Installation fails**
   - Check permissions
   - Ensure target directories exist
   - Try user installation instead of system-wide

### Dependencies

The release build requires the same Qt6 dependencies as debug:
- Qt6 Core
- Qt6 Widgets

## Distribution

The release package can be distributed as:
- Source tarball
- Binary tarball (created by `create-release-package.sh`)
- AppImage (future enhancement)
- Flatpak (future enhancement)
- Snap (future enhancement) 