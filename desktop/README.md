# Fitness Tracker Desktop App

A Qt6-based desktop application for fitness tracking, built from the ground up for optimal performance and user experience.

## Current Features

### ✅ Date Navigation
- Navigate to any date using the date picker
- Previous/Next day navigation buttons
- "Today" button to quickly return to current date
- Date display in format: "Mon, Jan 15, 2024"
- Unlimited date range navigation (past and future)

### ✅ Basic UI Structure
- Clean, modern interface
- Date navigation bar at the top
- Main content area for journal entries
- Empty state display for dates with no data

## Architecture

### Components
- **DateManager**: Manages current date state and navigation logic
- **DateNavigationBar**: UI component for date selection and navigation
- **JournalContentArea**: Main content area (currently shows empty state)
- **MainWindow**: Main application window that brings everything together

### Data Flow
```
DateManager (state) ←→ DateNavigationBar (UI)
       ↓
JournalContentArea (displays content for current date)
```

## Building and Running

### Prerequisites
- Qt6 development libraries
- CMake 3.16 or higher
- C++17 compatible compiler

### Build
```bash
cd desktop
./build.sh
```

### Run
```bash
./build/bin/fitness-tracker-desktop
```

## Project Structure
```
desktop/
├── CMakeLists.txt          # Build configuration
├── build.sh               # Build script
├── src/
│   ├── main.cpp           # Application entry point
│   ├── mainwindow.h/cpp   # Main window
│   ├── datemanager.h/cpp  # Date state management
│   ├── datenavigationbar.h/cpp  # Date navigation UI
│   └── journalcontentarea.h/cpp # Content area
└── resources/             # (Future: icons, stylesheets)
```

## Next Steps

### Phase 2: Body Composition Tracking
- Add body composition entry forms
- Weight and measurements input
- Data persistence
- Basic data validation

### Phase 3: Exercise Tracking
- Workout logging interface
- Exercise library management
- Set/rep/weight tracking

### Phase 4: Analytics
- Charts and progress visualization
- Data export capabilities
- Goal tracking

## Development Notes

- Uses Qt6 with C++17
- Signal/slot architecture for component communication
- Modular design for easy feature addition
- Cross-platform compatibility (Windows, macOS, Linux) 