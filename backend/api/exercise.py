"""Exercise API router."""

from typing import List, Optional
from fastapi import APIRouter, Depends, HTTPException, Query
from sqlalchemy.orm import Session
from datetime import date

from backend.database import get_db
from backend.services.exercise import ExerciseService, DailyActivityService
from backend.schemas.exercise import (
    ExerciseCreate, ExerciseUpdate, ExerciseResponse,
    WorkoutCreate, WorkoutUpdate, WorkoutResponse,
    DailyActivityCreate, DailyActivityUpdate, DailyActivityResponse
)

router = APIRouter()


# Exercise endpoints
@router.post("/exercises", response_model=ExerciseResponse)
async def create_exercise(
    exercise: ExerciseCreate,
    db: Session = Depends(get_db)
):
    """Create a new exercise in the library."""
    service = ExerciseService(db)
    try:
        return service.create_exercise(exercise)
    except Exception as e:
        raise HTTPException(status_code=400, detail=str(e))


@router.get("/exercises", response_model=List[ExerciseResponse])
async def get_exercises(
    category: Optional[str] = Query(None, description="Filter by exercise category"),
    active_only: bool = Query(True, description="Only return active exercises"),
    db: Session = Depends(get_db)
):
    """Get exercises with optional filtering."""
    service = ExerciseService(db)
    return service.get_exercises(category=category, active_only=active_only)


@router.get("/exercises/{exercise_id}", response_model=ExerciseResponse)
async def get_exercise(
    exercise_id: int,
    db: Session = Depends(get_db)
):
    """Get a specific exercise."""
    service = ExerciseService(db)
    exercise = service.get_exercise(exercise_id)
    if not exercise:
        raise HTTPException(status_code=404, detail="Exercise not found")
    return exercise


@router.put("/exercises/{exercise_id}", response_model=ExerciseResponse)
async def update_exercise(
    exercise_id: int,
    exercise: ExerciseUpdate,
    db: Session = Depends(get_db)
):
    """Update an exercise."""
    service = ExerciseService(db)
    updated_exercise = service.update_exercise(exercise_id, exercise)
    if not updated_exercise:
        raise HTTPException(status_code=404, detail="Exercise not found")
    return updated_exercise


@router.delete("/exercises/{exercise_id}")
async def delete_exercise(
    exercise_id: int,
    db: Session = Depends(get_db)
):
    """Delete an exercise (soft delete)."""
    service = ExerciseService(db)
    success = service.delete_exercise(exercise_id)
    if not success:
        raise HTTPException(status_code=404, detail="Exercise not found")
    return {"message": "Exercise deleted successfully"}


# Daily Activity endpoints
@router.get("/daily-activities", response_model=List[DailyActivityResponse])
def get_daily_activities(skip: int = 0, limit: int = 100, db: Session = Depends(get_db)):
    """Get all daily activities."""
    return DailyActivityService.get_daily_activities(db, skip=skip, limit=limit)


@router.get("/daily-activities/{activity_id}", response_model=DailyActivityResponse)
def get_daily_activity(activity_id: int, db: Session = Depends(get_db)):
    """Get a specific daily activity."""
    activity = DailyActivityService.get_daily_activity(db, activity_id)
    if activity is None:
        raise HTTPException(status_code=404, detail="Daily activity not found")
    return activity


@router.get("/daily-activities/date/{target_date}", response_model=Optional[DailyActivityResponse])
def get_daily_activity_by_date(target_date: date, db: Session = Depends(get_db)):
    """Get daily activity for a specific date."""
    activity = DailyActivityService.get_daily_activity_by_date(db, target_date)
    return activity


@router.post("/daily-activities", response_model=DailyActivityResponse)
def create_or_update_daily_activity(activity: DailyActivityCreate, db: Session = Depends(get_db)):
    """Create or update daily activity for a specific date."""
    return DailyActivityService.create_or_update_daily_activity(db, activity)


@router.put("/daily-activities/{activity_id}", response_model=DailyActivityResponse)
def update_daily_activity(activity_id: int, activity: DailyActivityUpdate, db: Session = Depends(get_db)):
    """Update a daily activity."""
    updated_activity = DailyActivityService.update_daily_activity(db, activity_id, activity)
    if updated_activity is None:
        raise HTTPException(status_code=404, detail="Daily activity not found")
    return updated_activity


@router.delete("/daily-activities/{activity_id}")
def delete_daily_activity(activity_id: int, db: Session = Depends(get_db)):
    """Delete a daily activity."""
    success = DailyActivityService.delete_daily_activity(db, activity_id)
    if not success:
        raise HTTPException(status_code=404, detail="Daily activity not found")
    return {"message": "Daily activity deleted successfully"}


# Workout endpoints
@router.post("/workouts", response_model=WorkoutResponse)
async def create_workout(
    workout: WorkoutCreate,
    db: Session = Depends(get_db)
):
    """Create a new workout."""
    service = ExerciseService(db)
    try:
        return service.create_workout(workout)
    except Exception as e:
        raise HTTPException(status_code=400, detail=str(e))


@router.get("/workouts", response_model=List[WorkoutResponse])
async def get_workouts(
    limit: int = Query(100, ge=1, le=1000),
    offset: int = Query(0, ge=0),
    db: Session = Depends(get_db)
):
    """Get workouts with pagination."""
    service = ExerciseService(db)
    return service.get_workouts(limit=limit, offset=offset)


@router.get("/workouts/{workout_id}", response_model=WorkoutResponse)
async def get_workout(
    workout_id: int,
    db: Session = Depends(get_db)
):
    """Get a specific workout."""
    service = ExerciseService(db)
    workout = service.get_workout(workout_id)
    if not workout:
        raise HTTPException(status_code=404, detail="Workout not found")
    return workout


@router.put("/workouts/{workout_id}", response_model=WorkoutResponse)
async def update_workout(
    workout_id: int,
    workout: WorkoutUpdate,
    db: Session = Depends(get_db)
):
    """Update a workout."""
    service = ExerciseService(db)
    updated_workout = service.update_workout(workout_id, workout)
    if not updated_workout:
        raise HTTPException(status_code=404, detail="Workout not found")
    return updated_workout


@router.delete("/workouts/{workout_id}")
async def delete_workout(
    workout_id: int,
    db: Session = Depends(get_db)
):
    """Delete a workout."""
    service = ExerciseService(db)
    success = service.delete_workout(workout_id)
    if not success:
        raise HTTPException(status_code=404, detail="Workout not found")
    return {"message": "Workout deleted successfully"}


@router.delete("/workout-exercises/{exercise_id}")
async def delete_workout_exercise(
    exercise_id: int,
    db: Session = Depends(get_db)
):
    """Delete an individual exercise from a workout."""
    service = ExerciseService(db)
    success = service.delete_workout_exercise(exercise_id)
    if not success:
        raise HTTPException(status_code=404, detail="Workout exercise not found")
    return {"message": "Exercise deleted from workout successfully"}


# Progress tracking endpoints
@router.get("/exercises/{exercise_id}/progress")
async def get_exercise_progress(
    exercise_id: int,
    days: int = Query(30, ge=1, le=365),
    db: Session = Depends(get_db)
):
    """Get progress for a specific exercise over time."""
    service = ExerciseService(db)
    progress = service.get_exercise_progress(exercise_id, days)
    if "error" in progress:
        raise HTTPException(status_code=404, detail=progress["error"])
    return progress


@router.get("/statistics/workouts")
async def get_workout_statistics(
    days: int = Query(30, ge=1, le=365),
    db: Session = Depends(get_db)
):
    """Get overall workout statistics."""
    service = ExerciseService(db)
    stats = service.get_workout_statistics(days)
    if "error" in stats:
        raise HTTPException(status_code=404, detail=stats["error"])
    return stats


# Utility endpoints
@router.get("/calculate-1rm")
async def calculate_one_rep_max(
    weight: float = Query(..., gt=0, description="Weight used"),
    reps: int = Query(..., ge=1, description="Number of repetitions"),
    db: Session = Depends(get_db)
):
    """Calculate one-rep max using Epley formula."""
    service = ExerciseService(db)
    one_rm = service.calculate_1rm(weight, reps)
    return {
        "weight": weight,
        "reps": reps,
        "one_rep_max": one_rm
    } 