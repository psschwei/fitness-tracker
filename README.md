# Fitness Tracker

A local-first, offline fitness tracking application built with Python FastAPI backend and React frontend, designed to track body composition and exercise progress for a single user.

## Features

### Body Composition Tracking
- Weight measurements with date/time tracking
- Body fat percentage, muscle mass, body water percentage
- Bone mass and visceral fat tracking
- BMI calculation
- Body measurements (chest, waist, arms, legs, etc.)
- Progress charts and trend analysis
- Goal setting and progress tracking

### Exercise Tracking
- Comprehensive workout logging with exercises, sets, reps, and weights
- Exercise library with common exercises
- Workout templates and routines
- Progress tracking for individual exercises
- One-rep max calculations
- Workout frequency and duration tracking

### Analytics & Insights
- Interactive charts using Plotly
- Weight and body composition trends
- Exercise progress visualization
- Workout frequency analysis
- Comprehensive dashboard with key metrics
- Data export capabilities

## Architecture

### Tech Stack
- **Backend**: Python FastAPI with SQLAlchemy ORM
- **Database**: SQLite (local file-based)
- **Frontend**: React (minimal UI components)
- **Desktop**: Tauri (Rust-based desktop wrapper)
- **Data Processing**: Python pandas, numpy for analytics
- **Charts**: Python plotly for server-side chart generation

### Data Flow
1. **Python Backend**: Handles all business logic, data processing, and analytics
2. **SQLite Database**: Local file storage with automatic backup
3. **React Frontend**: Minimal UI for data entry and display
4. **Tauri Desktop**: Lightweight desktop wrapper for system integration

## Installation

### Prerequisites
- Python 3.11 or higher
- uv (Python package manager)
- Node.js 18 or higher (for frontend)
- Rust (for Tauri)

### Backend Setup

1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd fitness-tracker
   ```

2. **Install Python dependencies**:
   ```bash
   # Install with all dependencies
   uv sync
   
   # Or for development with dev dependencies
   uv sync --dev
   ```

3. **Run the backend server**:
   ```bash
   uv run python -m backend.main
   ```

The API will be available at `http://localhost:8000`

### Frontend Setup (Coming Soon)

1. **Install frontend dependencies**:
   ```bash
   cd frontend
   npm install
   ```

2. **Start the development server**:
   ```bash
   npm run dev
   ```

### Tauri Desktop App (Coming Soon)

1. **Install Tauri CLI**:
   ```bash
   cargo install tauri-cli
   ```

2. **Build the desktop app**:
   ```bash
   cd src-tauri
   cargo tauri build
   ```

## API Documentation

Once the backend is running, you can access:
- **Interactive API docs**: http://localhost:8000/docs
- **ReDoc documentation**: http://localhost:8000/redoc

### Key Endpoints

#### Body Composition
- `POST /api/body-composition/` - Add new measurement
- `GET /api/body-composition/` - Get measurements
- `GET /api/body-composition/latest` - Get latest measurement
- `GET /api/body-composition/trends/{days}` - Get trends
- `GET /api/body-composition/statistics/overview` - Get statistics

#### Exercise & Workouts
- `POST /api/exercise/exercises` - Create exercise
- `GET /api/exercise/exercises` - Get exercises
- `POST /api/exercise/workouts` - Create workout
- `GET /api/exercise/workouts` - Get workouts
- `GET /api/exercise/exercises/{id}/progress` - Get exercise progress

#### Analytics
- `GET /api/analytics/charts/weight-trend` - Weight trend chart
- `GET /api/analytics/charts/body-composition` - Body composition chart
- `GET /api/analytics/charts/exercise-progress` - Exercise progress chart
- `GET /api/analytics/summary/dashboard` - Dashboard summary

## Data Storage

The application uses SQLite for local data storage. The database file is automatically created at:
- **Linux/macOS**: `~/.fitness-tracker/fitness_tracker.db`
- **Windows**: `%USERPROFILE%\.fitness-tracker\fitness_tracker.db`

### Database Schema

#### Body Composition
- `body_composition` - Weight, body fat, muscle mass measurements
- `body_measurements` - Body measurements (chest, waist, etc.)

#### Exercise
- `exercises` - Exercise library
- `workouts` - Workout sessions
- `workout_exercises` - Exercise sets within workouts
- `workout_templates` - Reusable workout templates

#### Goals
- `goals` - Fitness goals and progress tracking

## Development

### Project Structure
```
fitness-tracker/
├── backend/                 # Python backend
│   ├── main.py             # FastAPI application
│   ├── database.py         # Database configuration
│   ├── models/             # SQLAlchemy models
│   ├── schemas/            # Pydantic schemas
│   ├── services/           # Business logic
│   └── api/                # API routers
├── frontend/               # React frontend (coming soon)
├── src-tauri/              # Tauri desktop wrapper (coming soon)
├── pyproject.toml          # Python project configuration
└── README.md              # This file
```

### Running Tests
```bash
uv run pytest tests/
```

### Code Formatting
```bash
uv run black backend/
uv run isort backend/
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Roadmap

### Phase 1: Backend Foundation ✅
- [x] Python FastAPI backend
- [x] SQLite database with SQLAlchemy
- [x] Body composition tracking
- [x] Exercise and workout tracking
- [x] Analytics and chart generation

### Phase 2: Frontend Development
- [ ] React frontend with TypeScript
- [ ] Responsive UI design
- [ ] Data entry forms
- [ ] Chart visualization

### Phase 3: Desktop Integration
- [ ] Tauri desktop wrapper
- [ ] System tray integration
- [ ] Automatic startup
- [ ] Data backup/restore

### Phase 4: Advanced Features
- [ ] Goal setting and tracking
- [ ] Workout scheduling
- [ ] Data export/import
- [ ] Advanced analytics
- [ ] Mobile responsive design