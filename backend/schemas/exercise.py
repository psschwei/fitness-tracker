"""Pydantic schemas for exercise and workout API."""

from datetime import datetime
from typing import Optional, List, Dict, Any
from pydantic import BaseModel, Field


class ExerciseBase(BaseModel):
    """Base exercise schema."""
    name: str = Field(..., min_length=1, max_length=255, description="Exercise name")
    category: str = Field(..., description="Exercise category (strength, cardio, flexibility)")


class ExerciseCreate(ExerciseBase):
    """Schema for creating a new exercise."""
    pass


class ExerciseUpdate(BaseModel):
    """Schema for updating an exercise."""
    name: Optional[str] = Field(None, min_length=1, max_length=255, description="Exercise name")
    category: Optional[str] = Field(None, description="Exercise category")
    is_active: Optional[bool] = Field(None, description="Whether the exercise is active")


class ExerciseResponse(ExerciseBase):
    """Schema for exercise response."""
    id: int
    is_active: bool
    created_at: datetime
    updated_at: datetime

    class Config:
        from_attributes = True


class SetData(BaseModel):
    """Schema for a single set in a workout."""
    weight: float = Field(..., ge=0, description="Weight in pounds")
    reps: int = Field(..., ge=1, description="Number of repetitions")


class WorkoutExerciseBase(BaseModel):
    """Base workout exercise schema."""
    exercise_id: int = Field(..., description="Exercise ID")
    sets_data: List[SetData] = Field(..., min_items=1, description="List of sets with weight and reps")
    notes: Optional[str] = Field(None, description="Exercise-specific notes")


class WorkoutBase(BaseModel):
    """Base workout schema."""
    notes: Optional[str] = Field(None, description="Workout notes")


class WorkoutCreate(WorkoutBase):
    """Schema for creating a new workout."""
    date: Optional[datetime] = Field(None, description="Workout date (defaults to now)")
    exercises: List[WorkoutExerciseBase] = Field(..., min_items=1, description="List of exercises")


class WorkoutUpdate(BaseModel):
    """Schema for updating a workout."""
    notes: Optional[str] = Field(None, description="Workout notes")


class WorkoutExerciseResponse(WorkoutExerciseBase):
    """Schema for workout exercise response."""
    id: int
    exercise: ExerciseResponse
    created_at: datetime
    updated_at: datetime

    class Config:
        from_attributes = True


class WorkoutResponse(WorkoutBase):
    """Schema for workout response."""
    id: int
    date: datetime
    exercises: List[WorkoutExerciseResponse]
    created_at: datetime
    updated_at: datetime

    class Config:
        from_attributes = True 