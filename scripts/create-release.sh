#!/bin/bash

# Create Release Script for Fitness Tracker
# This script helps automate the release process

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to get current version from CMakeLists.txt
get_current_version() {
    grep "project(fitness-tracker VERSION" CMakeLists.txt | sed 's/.*VERSION \([0-9.]*\).*/\1/'
}

# Function to update version in CMakeLists.txt
update_version() {
    local new_version=$1
    sed -i "s/project(fitness-tracker VERSION [0-9.]*)/project(fitness-tracker VERSION $new_version)/" CMakeLists.txt
    print_success "Updated version to $new_version in CMakeLists.txt"
}

# Function to validate version format
validate_version() {
    local version=$1
    if [[ ! $version =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
        print_error "Invalid version format. Use semantic versioning (e.g., 1.0.0, 2.1.3)"
        exit 1
    fi
}

# Function to check if git is clean
check_git_status() {
    if [[ -n $(git status --porcelain) ]]; then
        print_error "Git working directory is not clean. Please commit or stash your changes first."
        git status --short
        exit 1
    fi
}

# Function to check if tag already exists
check_tag_exists() {
    local version=$1
    if git tag -l "v$version" | grep -q "v$version"; then
        print_error "Tag v$version already exists. Please use a different version."
        exit 1
    fi
}

# Main script
main() {
    print_status "=== Fitness Tracker Release Script ==="
    echo ""
    
    # Check if we're in the right directory
    if [[ ! -f "CMakeLists.txt" ]]; then
        print_error "CMakeLists.txt not found. Please run this script from the project root."
        exit 1
    fi
    
    # Get current version
    current_version=$(get_current_version)
    print_status "Current version: $current_version"
    echo ""
    
    # Get new version from user
    if [[ $# -eq 1 ]]; then
        new_version=$1
    else
        echo -n "Enter new version (e.g., 1.0.0): "
        read new_version
    fi
    
    # Validate version format
    validate_version "$new_version"
    
    # Check if version is different
    if [[ "$current_version" == "$new_version" ]]; then
        print_warning "Version is already $new_version. Do you want to continue? (y/N)"
        read -r response
        if [[ ! "$response" =~ ^[Yy]$ ]]; then
            print_status "Release cancelled."
            exit 0
        fi
    fi
    
    # Check git status
    check_git_status
    
    # Check if tag already exists
    check_tag_exists "$new_version"
    
    echo ""
    print_status "Preparing release v$new_version..."
    
    # Update version in CMakeLists.txt
    update_version "$new_version"
    
    # Commit version change
    git add CMakeLists.txt
    git commit -m "Bump version to $new_version"
    print_success "Committed version change"
    
    # Create and push tag
    git tag "v$new_version"
    print_success "Created tag v$new_version"
    
    # Push changes and tag
    git push origin main
    git push origin "v$new_version"
    print_success "Pushed changes and tag to remote"
    
    echo ""
    print_success "=== Release v$new_version initiated! ==="
    print_status "GitHub Actions will automatically:"
    print_status "  - Build the application"
    print_status "  - Create a release package"
    print_status "  - Publish a GitHub release"
    print_status ""
    print_status "You can monitor the progress at:"
    print_status "https://github.com/$(git config --get remote.origin.url | sed 's/.*github.com[:/]\([^/]*\/[^/]*\).*/\1/')/actions"
}

# Run main function with all arguments
main "$@" 