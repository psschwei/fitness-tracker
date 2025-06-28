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


class DailyEntry(BaseModel):
    """Schema for a daily journal entry."""
    date: date
    body_composition: Optional[dict] = None
    exercises: List[dict] = []
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
    
    # Format exercises for the day
    exercises = []
    for workout in workouts:
        for exercise in workout.exercises:
            exercises.append({
                "exercise_name": exercise.exercise.name,
                "sets": exercise.sets_data,
                "notes": exercise.notes
            })
    
    return DailyEntry(
        date=entry_date,
        body_composition={
            "weight_pounds": body_comp.weight_pounds,
            "waist_inches": body_comp.waist_inches,
            "notes": body_comp.notes
        } if body_comp else None,
        exercises=exercises,
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
        
        # Format exercises for the day
        exercises = []
        for workout in workouts:
            for exercise in workout.exercises:
                exercises.append({
                    "exercise_name": exercise.exercise.name,
                    "sets": exercise.sets_data,
                    "notes": exercise.notes
                })
        
        entries.append(DailyEntry(
            date=current_date,
            body_composition={
                "weight_pounds": body_comp.weight_pounds,
                "waist_inches": body_comp.waist_inches,
                "notes": body_comp.notes
            } if body_comp else None,
            exercises=exercises,
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