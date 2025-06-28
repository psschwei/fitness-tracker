# Fitness Tracker

A local-first, offline fitness tracking application built with Python FastAPI backend and React frontend, designed to track body composition and exercise progress for a single user.

## Features

### Body Composition Tracking
- Weight measurements with date/time tracking
- Waist circumference tracking
- Progress charts and trend analysis
- Goal setting and progress tracking

### Exercise Tracking
- Comprehensive workout logging with exercises, sets, reps, and weights
- Exercise library with common exercises
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
- **Frontend**: React with TypeScript and Tailwind CSS
- **Data Processing**: Python pandas, numpy for analytics
- **Charts**: Python plotly for server-side chart generation

### Data Flow
1. **Python Backend**: Handles all business logic, data processing, and analytics
2. **SQLite Database**: Local file storage with automatic backup
3. **React Frontend**: Modern UI for data entry and display

## Installation

### Prerequisites
- Python 3.11 or higher
- uv (Python package manager)
- Node.js 18 or higher (for frontend)

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

### Frontend Setup

1. **Install frontend dependencies**:
   ```bash
   cd frontend
   npm install
   ```

2. **Start the development server**:
   ```bash
   npm run dev
   ```

The frontend will be available at `http://localhost:3000`

### Running Both Services

To run the complete application, you need both the backend and frontend running simultaneously:

#### Option 1: Quick Start (Recommended)
Use the launcher script to start everything with one command:

```bash
./start_app.sh
```

This script will:
- Install dependencies if needed
- Start the backend server
- Start the frontend development server
- Open your browser to the application
- Provide a clean shutdown with Ctrl+C

#### Option 2: Manual Start
If you prefer to start services manually:

1. **Terminal 1 - Backend**:
   ```bash
   # From the project root
   uv run python -m backend.main
   ```

2. **Terminal 2 - Frontend**:
   ```bash
   # From the project root
   cd frontend
   npm run dev
   ```

3. **Access the application**:
   - Frontend: http://localhost:3000
   - Backend API docs: http://localhost:8000/docs

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
- `body_composition` - Weight and waist circumference measurements

#### Exercise
- `exercises` - Exercise library
- `workouts` - Workout sessions
- `workout_exercises` - Exercise sets within workouts

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
├── frontend/               # React frontend
│   ├── src/
│   │   ├── components/     # React components
│   │   ├── pages/          # Page components
│   │   ├── api/            # API client
│   │   └── types/          # TypeScript types
│   ├── package.json        # Frontend dependencies
│   └── README.md           # Frontend documentation
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

### Frontend Development

The frontend uses Vite for fast development and building:

```bash
cd frontend

# Development
npm run dev          # Start development server
npm run build        # Build for production
npm run preview      # Preview production build
npm run lint         # Run ESLint
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

### Phase 2: Frontend Development ✅
- [x] React frontend with TypeScript
- [x] Responsive UI design
- [x] Data entry forms
- [x] Chart visualization
- [x] Daily journal view
- [x] Date navigation
- [x] Body composition forms
- [x] Exercise entry (single and batch)

### Phase 3: Enhanced Features
- [ ] Goal setting and tracking
- [ ] Workout scheduling
- [ ] Data export/import
- [ ] Advanced analytics
- [ ] Mobile responsive design
- [ ] Offline support improvements
- [ ] Data backup/restore

## Why Web-Only?

After exploring desktop options, we've decided to focus on the web application because:

### **Simplicity**
- Single codebase to maintain
- No complex build processes
- Familiar web technologies

### **Cross-Platform**
- Works on any device with a browser
- No installation required
- Easy updates

### **Development Speed**
- Fast iteration with hot reload
- Rich ecosystem of web tools
- Easy debugging

### **Accessibility**
- Works on desktop, tablet, and mobile
- No system-specific dependencies
- Universal access

The web application provides all the functionality you need for fitness tracking while keeping the codebase simple and maintainable.