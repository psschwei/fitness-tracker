# GitHub Actions Setup for Fitness Tracker

This document explains the GitHub Actions workflows configured for the Fitness Tracker project.

## Overview

The project uses three GitHub Actions workflows:

1. **Build and Test** (`build.yml`) - Continuous integration
2. **Release** (`release.yml`) - Automated releases
3. **Test Local Build** (`test-local.yml`) - Manual testing

## Workflows

### 1. Build and Test (`build.yml`)

**Triggers:**
- Push to `main` or `develop` branches
- Pull requests to `main` branch

**Purpose:**
- Ensures code compiles correctly before merging
- Builds both Debug and Release versions
- Uploads build artifacts for download
- Provides early feedback on code changes

**Output:**
- Build artifacts available for download
- Build status reported on pull requests

### 2. Release (`release.yml`)

**Triggers:**
- Push of tags matching pattern `v*` (e.g., `v1.0.0`, `v2.1.3`)

**Purpose:**
- Automatically creates GitHub releases
- Builds optimized release version
- Creates Linux tarball with installation scripts
- Publishes release with download links

**Output:**
- GitHub release with download links
- Release package: `fitness-tracker-{version}.tar.gz`
- Build artifacts for the release

### 3. Test Local Build (`test-local.yml`)

**Triggers:**
- Manual workflow dispatch (GitHub UI)

**Purpose:**
- Test the build process with specific versions
- Validate release package creation
- Debug build issues

**Usage:**
1. Go to Actions tab in GitHub
2. Select "Test Local Build" workflow
3. Click "Run workflow"
4. Enter version to test (e.g., `1.0.0`)

## Release Process

### Automated Release (Recommended)

1. **Use the release script:**
   ```bash
   ./scripts/create-release.sh 1.0.0
   ```

2. **The script will:**
   - Update version in `CMakeLists.txt`
   - Commit the change
   - Create and push tag `v1.0.0`
   - Trigger GitHub Actions release workflow

3. **GitHub Actions will:**
   - Build the application
   - Create release package
   - Publish GitHub release
   - Upload build artifacts

### Manual Release

1. **Update version in `CMakeLists.txt`**
2. **Commit and push:**
   ```bash
   git add CMakeLists.txt
   git commit -m "Bump version to 1.0.0"
   git push origin main
   ```
3. **Create and push tag:**
   ```bash
   git tag v1.0.0
   git push origin v1.0.0
   ```

## Release Package Contents

Each release includes:

- `fitness-tracker` - Main executable
- `install.sh` - Installation script
- `uninstall.sh` - Uninstallation script
- `run.sh` - Test run script
- `README.md` - Documentation
- `LICENSE` - License file
- `fitness-tracker.desktop` - Desktop integration

## Installation from Release

```bash
# Download and extract
tar -xzf fitness-tracker-1.0.0.tar.gz
cd fitness-tracker-1.0.0

# Install for current user
./install.sh

# Or install system-wide (requires sudo)
sudo ./install.sh
```

## Build Environment

The workflows use:
- **OS:** Ubuntu 22.04
- **Qt:** 6.6.2 with all necessary modules
- **CMake:** 3.26.0
- **Compiler:** GCC with C++17 support
- **Build Type:** Release with optimizations (`-O3 -DNDEBUG`)

## Troubleshooting

### Build Failures

1. **Check Qt installation:**
   - Ensure all required Qt modules are available
   - Verify Qt version compatibility

2. **Check CMake configuration:**
   - Verify all source files are included
   - Check for missing dependencies

3. **Check compiler compatibility:**
   - Ensure C++17 support
   - Check for compiler-specific issues

### Release Issues

1. **Tag already exists:**
   - Use a different version number
   - Delete existing tag if needed

2. **Release not created:**
   - Check workflow logs in Actions tab
   - Verify tag format matches `v*` pattern
   - Ensure repository has proper permissions

3. **Package creation fails:**
   - Check if executable was built successfully
   - Verify all required files exist

## Monitoring

- **Build Status:** Check Actions tab for workflow status
- **Release Status:** Monitor release workflow in Actions tab
- **Artifacts:** Download build artifacts from Actions tab
- **Releases:** View published releases in Releases tab

## Security

- Workflows use `GITHUB_TOKEN` for authentication
- No sensitive data is exposed in logs
- Build artifacts are automatically cleaned up
- Release packages are signed and verified

## Customization

### Adding New Platforms

To add support for additional platforms:

1. **Create new workflow file** (e.g., `release-windows.yml`)
2. **Add platform-specific build steps**
3. **Include platform-specific packaging**
4. **Update release workflow to include new artifacts**

### Modifying Build Process

1. **Edit workflow files** in `.github/workflows/`
2. **Update build scripts** if needed
3. **Test changes** using the test workflow
4. **Commit and push** to trigger builds

### Adding Tests

1. **Create test workflow** (e.g., `test.yml`)
2. **Add test dependencies** to workflow
3. **Include test execution** in build process
4. **Configure test reporting** and coverage

## Best Practices

1. **Always test locally** before pushing
2. **Use semantic versioning** for releases
3. **Review build logs** for any issues
4. **Keep dependencies updated**
5. **Monitor build times** and optimize if needed
6. **Use the release script** for consistency
7. **Document any workflow changes** 