# Fitness Tracker Desktop App

A Qt6-based desktop application for fitness tracking, built from the ground up for optimal performance and user experience.

## Current Features

### ✅ Date Navigation
- Navigate to any date using the date picker
- Previous/Next day navigation buttons
- "Today" button to quickly return to current date
- Date display in format: "Mon, Jan 15, 2024"
- Unlimited date range navigation (past and future)

### ✅ Body Composition Tracking
- **Data Entry**: Complete form for weight, waist, height, neck measurements, and notes
- **Gender Selection**: Male/Female radio buttons for accurate body fat calculations
- **Real-time Validation**: Form validation with helpful error messages
- **Data Persistence**: Automatic saving to JSON file in user's app data directory
- **Data Viewing**: Clean display of saved body composition data
- **Edit & Delete**: Full CRUD operations for body composition entries
- **Date-based Storage**: Each date can have one body composition entry
- **Smart Prefill**: New entries are pre-filled with yesterday's values for convenience

### ✅ BMI & Body Fat Calculations
- **Automatic BMI Calculation**: Real-time BMI calculation using standard formula
- **BMI Categories**: Underweight, Normal weight, Overweight, Obese
- **Navy Tape Test**: Body fat percentage calculation using waist, neck, and height measurements
- **Body Fat Categories**: Essential fat, Athlete, Fitness, Average, Obese (gender-specific)
- **Real-time Updates**: Calculations update instantly as you type
- **Formatted Display**: BMI and body fat shown with categories in both form and view

### ✅ Basic UI Structure
- Clean, modern interface
- Date navigation bar at the top
- Main content area for journal entries
- Empty state display for dates with no data
- Add new entry button for empty dates

## Architecture

### Components
- **DateManager**: Manages current date state and navigation logic
- **DataManager**: Handles data persistence using JSON files
- **DateNavigationBar**: UI component for date selection and navigation
- **JournalContentArea**: Main content area with stacked widget for different states
- **BodyCompositionForm**: Form for entering/editing body composition data with real-time calculations
- **BodyCompositionView**: Display component for viewing saved data with calculations
- **MainWindow**: Main application window that brings everything together
- **Calculations**: Utility class for BMI and body fat calculations and formatting

### Data Flow
```
DateManager (state) ←→ DateNavigationBar (UI)
       ↓
JournalContentArea (displays content for current date)
       ↓
DataManager (persistence) ←→ BodyCompositionForm/View
       ↓
Calculations (BMI & Body Fat) ←→ Real-time updates
```

### Data Storage
- Data is stored in JSON format in the user's app data directory
- File location: `~/.local/share/fitness-tracker/data.json` (Linux)
- Automatic data directory creation
- Error handling for file operations
- Includes BMI and body fat calculations in stored data

### Calculation Formulas
- **BMI**: weight (kg) / height (m)²
- **Body Fat (Male)**: 86.010 × log10(waist - neck) - 70.041 × log10(height) + 36.76
- **Body Fat (Female)**: Not implemented (requires hip measurement)

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
│   ├── journalcontentarea.h/cpp # Content area
│   ├── bodycompositionform.h/cpp # Body composition form
│   ├── bodycompositionview.h/cpp # Body composition display
│   ├── models/
│   │   ├── bodycomposition.h/cpp # Body composition data model
│   │   └── datamanager.h/cpp     # Data persistence manager
│   └── utils/
│       └── calculations.h/cpp    # BMI & body fat calculations
└── resources/             # (Future: icons, stylesheets)
```

## Next Steps

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
- JSON-based data persistence for simplicity and portability
- Real-time calculations for immediate user feedback 