"""Journal API router for daily entries."""

from datetime import datetime, date, timedelta
from typing import List, Optional
from fastapi import APIRouter, Depends, HTTPException, Query
from sqlalchemy.orm import Session
from sqlalchemy import func

from backend.database import get_db
from backend.services.body_composition import BodyCompositionService
from backend.services.exercise import ExerciseService
from pydantic import BaseModel


class WorkoutExercise(BaseModel):
    """Schema for a workout exercise."""
    id: int
    workout_id: int
    exercise_id: int
    exercise_name: str
    weight: float
    reps_per_set: int
    created_at: str
    updated_at: str


class Workout(BaseModel):
    """Schema for a workout."""
    id: int
    date: str
    created_at: str
    updated_at: str
    exercises: List[WorkoutExercise]


class DailyEntry(BaseModel):
    """Schema for a daily journal entry."""
    date: str
    body_composition: Optional[dict] = None
    workouts: List[Workout] = []
    notes: Optional[str] = None


router = APIRouter()


@router.get("/daily/{entry_date}", response_model=DailyEntry)
async def get_daily_entry(
    entry_date: date,
    db: Session = Depends(get_db)
):
    """Get journal entry for a specific date."""
    body_service = BodyCompositionService(db)
    exercise_service = ExerciseService(db)
    
    # Get body composition for the date
    body_comp = body_service.get_measurement_by_date(entry_date)
    
    # Get workouts for the date
    workouts = exercise_service.get_workouts_by_date(entry_date)
    
    # Format workouts for the day
    formatted_workouts = []
    for workout in workouts:
        exercises = []
        for exercise in workout.exercises:
            exercises.append(WorkoutExercise(
                id=exercise.id,
                workout_id=workout.id,
                exercise_id=exercise.exercise_id,
                exercise_name=exercise.exercise.name,
                weight=exercise.weight,
                reps_per_set=exercise.reps_per_set,
                created_at=exercise.created_at.isoformat(),
                updated_at=exercise.updated_at.isoformat()
            ))
        
        formatted_workouts.append(Workout(
            id=workout.id,
            date=workout.date.isoformat(),
            created_at=workout.created_at.isoformat(),
            updated_at=workout.updated_at.isoformat(),
            exercises=exercises
        ))
    
    return DailyEntry(
        date=entry_date.isoformat(),
        body_composition={
            "id": body_comp.id,
            "date": body_comp.date.isoformat(),
            "weight_pounds": body_comp.weight_pounds,
            "height_inches": body_comp.height_inches,
            "waist_inches": body_comp.waist_inches,
            "neck_inches": body_comp.neck_inches,
            "bmi": body_comp.bmi,
            "body_fat_percentage": body_comp.body_fat_percentage,
            "is_male": body_comp.is_male,
            "notes": body_comp.notes,
            "created_at": body_comp.created_at.isoformat(),
            "updated_at": body_comp.updated_at.isoformat()
        } if body_comp else None,
        workouts=formatted_workouts,
        notes=", ".join([w.notes for w in workouts if w.notes]) if workouts else None
    )


@router.get("/daily", response_model=List[DailyEntry])
async def get_daily_entries(
    start_date: Optional[date] = Query(None, description="Start date for entries"),
    end_date: Optional[date] = Query(None, description="End date for entries"),
    limit: int = Query(30, ge=1, le=100, description="Number of days to return"),
    db: Session = Depends(get_db)
):
    """Get journal entries for a date range."""
    body_service = BodyCompositionService(db)
    exercise_service = ExerciseService(db)
    
    # If no dates provided, get the last N days
    if not start_date:
        end_date = end_date or date.today()
        start_date = end_date - timedelta(days=limit-1)
    elif not end_date:
        end_date = start_date + timedelta(days=limit-1)
    
    entries = []
    current_date = start_date
    
    while current_date <= end_date:
        # Get body composition for the date
        body_comp = body_service.get_measurement_by_date(current_date)
        
        # Get workouts for the date
        workouts = exercise_service.get_workouts_by_date(current_date)
        
        # Format workouts for the day
        formatted_workouts = []
        for workout in workouts:
            exercises = []
            for exercise in workout.exercises:
                exercises.append(WorkoutExercise(
                    id=exercise.id,
                    workout_id=workout.id,
                    exercise_id=exercise.exercise_id,
                    exercise_name=exercise.exercise.name,
                    weight=exercise.weight,
                    reps_per_set=exercise.reps_per_set,
                    created_at=exercise.created_at.isoformat(),
                    updated_at=exercise.updated_at.isoformat()
                ))
            
            formatted_workouts.append(Workout(
                id=workout.id,
                date=workout.date.isoformat(),
                created_at=workout.created_at.isoformat(),
                updated_at=workout.updated_at.isoformat(),
                exercises=exercises
            ))
        
        entries.append(DailyEntry(
            date=current_date.isoformat(),
            body_composition={
                "id": body_comp.id,
                "date": body_comp.date.isoformat(),
                "weight_pounds": body_comp.weight_pounds,
                "height_inches": body_comp.height_inches,
                "waist_inches": body_comp.waist_inches,
                "neck_inches": body_comp.neck_inches,
                "bmi": body_comp.bmi,
                "body_fat_percentage": body_comp.body_fat_percentage,
                "is_male": body_comp.is_male,
                "notes": body_comp.notes,
                "created_at": body_comp.created_at.isoformat(),
                "updated_at": body_comp.updated_at.isoformat()
            } if body_comp else None,
            workouts=formatted_workouts,
            notes=", ".join([w.notes for w in workouts if w.notes]) if workouts else None
        ))
        
        current_date += timedelta(days=1)
    
    return entries


@router.get("/today", response_model=DailyEntry)
async def get_today_entry(db: Session = Depends(get_db)):
    """Get today's journal entry."""
    return await get_daily_entry(date.today(), db)


@router.get("/recent", response_model=List[DailyEntry])
async def get_recent_entries(
    days: int = Query(7, ge=1, le=30, description="Number of recent days"),
    db: Session = Depends(get_db)
):
    """Get recent journal entries."""
    end_date = date.today()
    start_date = end_date - timedelta(days=days-1)
    
    return await get_daily_entries(start_date, end_date, days, db) 