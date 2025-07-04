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

### ✅ Exercise Library Management
- **Exercise Database**: Add, edit, and delete exercises with categories
- **Exercise Categories**: Strength, Cardio, Flexibility
- **Active/Inactive Status**: Toggle exercise availability
- **Data Validation**: Form validation with error messages
- **Table View**: Clean table display of all exercises with sorting
- **CRUD Operations**: Full create, read, update, delete functionality

### ✅ Workout Tracking
- **Workout Builder**: Comprehensive workout creation interface
- **Exercise Selection**: Dropdown selection from exercise library
- **Set/Rep/Weight Tracking**: Individual tracking for each exercise
- **Workout Notes**: Optional notes for each workout
- **Exercise Notes**: Individual notes for each exercise in a workout
- **Data Validation**: Form validation with helpful error messages
- **Workout Viewing**: Clean display of saved workouts with exercise details
- **Edit & Delete**: Full CRUD operations for workout entries
- **Date-based Storage**: Workouts are stored by date
- **Keyboard Shortcuts**: Tab navigation and Ctrl+S/Escape shortcuts

### ✅ Modern UI with Tabbed Interface
- **Tabbed Navigation**: Body Composition, Workouts, Exercise Library tabs
- **Keyboard Shortcuts**: Quick navigation between tabs (b, w, l keys)
- **Action Shortcuts**: Edit (e), Delete (d), Add (a) shortcuts
- **Navigation Shortcuts**: Today (t), Previous (p), Next (n) day shortcuts
- **Clean Design**: Modern, responsive interface with proper spacing
- **Empty States**: Helpful empty state displays with action buttons

## Architecture

### Components
- **DateManager**: Manages current date state and navigation logic
- **DataManager**: Handles data persistence using JSON files for all data types
- **DateNavigationBar**: UI component for date selection and navigation
- **JournalContentArea**: Main content area with tabbed interface
- **BodyCompositionForm**: Form for entering/editing body composition data with real-time calculations
- **BodyCompositionView**: Display component for viewing saved body composition data
- **WorkoutBuilder**: Comprehensive workout creation and editing interface
- **WorkoutView**: Display component for viewing saved workouts
- **ExerciseLibrary**: Exercise management interface with CRUD operations
- **MainWindow**: Main application window that brings everything together
- **Calculations**: Utility class for BMI and body fat calculations and formatting

### Data Models
- **BodyComposition**: Body composition data model with validation
- **Exercise**: Exercise data model with categories and status
- **Workout**: Workout data model with exercises and status
- **WorkoutExercise**: Exercise within a workout with sets data
- **SetData**: Individual set data (weight, reps, sets)

### Data Flow
```
DateManager (state) ←→ DateNavigationBar (UI)
       ↓
JournalContentArea (tabbed interface)
       ↓
DataManager (persistence) ←→ Forms/Views
       ↓
Calculations (BMI & Body Fat) ←→ Real-time updates
```

### Data Storage
- Data is stored in JSON format in the user's app data directory
- File location: `~/.local/share/fitness-tracker/data.json` (Linux)
- Automatic data directory creation
- Error handling for file operations
- Includes body composition, exercises, and workouts in single file

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
./build.sh
```

### Run
```bash
./build/bin/fitness-tracker-desktop
```

## Project Structure
```
src/
├── main.cpp                    # Application entry point
├── mainwindow.h/cpp           # Main window
├── datemanager.h/cpp          # Date state management
├── datenavigationbar.h/cpp    # Date navigation UI
├── journalcontentarea.h/cpp   # Main content area with tabs
├── bodycompositionform.h/cpp  # Body composition form
├── bodycompositionview.h/cpp  # Body composition display
├── workoutbuilder.h/cpp       # Workout creation interface
├── workoutview.h/cpp          # Workout display
├── exerciselibrary.h/cpp      # Exercise management interface
├── models/
│   ├── bodycomposition.h/cpp  # Body composition data model
│   ├── exercise.h/cpp         # Exercise data model
│   ├── workout.h/cpp          # Workout data model
│   ├── setdata.h/cpp          # Set data model
│   └── datamanager.h/cpp      # Data persistence manager
└── utils/
    └── calculations.h/cpp     # BMI & body fat calculations
```

## Next Steps

### Phase 4: Analytics & Visualization
- **Progress Charts**: Visual progress tracking for body composition metrics
- **Workout Analytics**: Exercise performance trends and progress charts
- **Data Export**: Export data to CSV/Excel formats
- **Goal Tracking**: Set and track fitness goals
- **Statistics Dashboard**: Overview of key metrics and trends

### Phase 5: Advanced Features
- **Multiple Sets**: Support for multiple sets per exercise with individual tracking
- **Rest Timer**: Built-in rest timer for workouts
- **Workout Templates**: Save and reuse workout templates
- **Exercise History**: Detailed exercise performance history
- **Body Fat (Female)**: Implement female body fat calculation with hip measurement
- **Data Backup**: Automatic backup and restore functionality

### Phase 6: User Experience Enhancements
- **Dark Mode**: Theme support with dark/light mode toggle
- **Custom Categories**: User-defined exercise categories
- **Import/Export**: Import data from other fitness apps
- **Mobile Sync**: Future mobile app with cloud sync
- **Notifications**: Workout reminders and progress notifications

## Development Notes

- Uses Qt6 with C++17
- Signal/slot architecture for component communication
- Modular design for easy feature addition
- Cross-platform compatibility (Windows, macOS, Linux)
- JSON-based data persistence for simplicity and portability
- Real-time calculations for immediate user feedback
- Comprehensive keyboard shortcuts for power users
- Tabbed interface for organized feature access 