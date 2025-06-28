"""Main FastAPI application for the fitness tracker."""

from fastapi import FastAPI, Depends, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from sqlalchemy.orm import Session
import uvicorn

from backend.database import get_db, init_db
from backend.api import body_composition, exercise, journal
from backend.services.body_composition import BodyCompositionService
from backend.services.exercise import ExerciseService

# Create FastAPI app
app = FastAPI(
    title="Fitness Tracker API",
    description="A local-first fitness tracking application API",
    version="0.1.0"
)

# Add CORS middleware for frontend communication
app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:3000", "http://localhost:1420"],  # React dev server and Tauri
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Include API routers
app.include_router(body_composition.router, prefix="/api/body-composition", tags=["body-composition"])
app.include_router(exercise.router, prefix="/api/exercise", tags=["exercise"])
app.include_router(journal.router, prefix="/api/journal", tags=["journal"])


@app.on_event("startup")
async def startup_event():
    """Initialize database on startup."""
    init_db()


@app.get("/")
async def root():
    """Root endpoint."""
    return {
        "message": "Fitness Tracker API",
        "version": "0.1.0",
        "status": "running"
    }


@app.get("/api/health")
async def health_check():
    """Health check endpoint."""
    return {"status": "healthy"}


@app.get("/api/dashboard")
async def get_dashboard_data(db: Session = Depends(get_db)):
    """Get dashboard data combining body composition and exercise data."""
    body_service = BodyCompositionService(db)
    exercise_service = ExerciseService(db)
    
    try:
        # Get latest body composition data
        latest_measurement = body_service.get_latest_measurement()
        body_trends = body_service.get_trends(30)
        
        # Get workout statistics
        workout_stats = exercise_service.get_workout_statistics(30)
        
        # Get recent workouts
        recent_workouts = exercise_service.get_workouts(limit=5)
        
        return {
            "body_composition": {
                "latest": latest_measurement,
                "trends": body_trends
            },
            "workouts": {
                "statistics": workout_stats,
                "recent": recent_workouts
            }
        }
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


if __name__ == "__main__":
    uvicorn.run(
        "backend.main:app",
        host="127.0.0.1",
        port=8000,
        reload=True
    ) 