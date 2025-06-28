"""Exercise and workout database models."""

from datetime import datetime
from sqlalchemy import Column, Integer, Float, DateTime, String, Text, ForeignKey, JSON, Boolean
from sqlalchemy.orm import relationship
from sqlalchemy.sql import func

from backend.database import Base


class Exercise(Base):
    """Exercise library model."""
    
    __tablename__ = "exercises"
    
    id = Column(Integer, primary_key=True, index=True)
    name = Column(String(255), nullable=False, unique=True)
    category = Column(String(50), nullable=False)  # strength, cardio, flexibility
    is_active = Column(Integer, default=1)  # 1 for active, 0 for inactive
    created_at = Column(DateTime, default=func.now())
    updated_at = Column(DateTime, default=func.now(), onupdate=func.now())
    
    # Relationships
    workout_exercises = relationship("WorkoutExercise", back_populates="exercise")
    
    def __repr__(self):
        return f"<Exercise(id={self.id}, name='{self.name}', category='{self.category}')>"


class DailyActivity(Base):
    """Daily activity tracking model."""
    
    __tablename__ = "daily_activities"
    
    id = Column(Integer, primary_key=True, index=True)
    date = Column(DateTime, nullable=False, unique=True, default=func.now())
    steps = Column(Integer)  # Total steps for the day (optional)
    walk_yes_no = Column(Boolean)  # Whether a walk was taken (optional)
    notes = Column(Text)
    created_at = Column(DateTime, default=func.now())
    updated_at = Column(DateTime, default=func.now(), onupdate=func.now())
    
    def __repr__(self):
        return f"<DailyActivity(id={self.id}, date={self.date}, steps={self.steps}, walk_yes_no={self.walk_yes_no})>"


class Workout(Base):
    """Workout session model."""
    
    __tablename__ = "workouts"
    
    id = Column(Integer, primary_key=True, index=True)
    date = Column(DateTime, nullable=False, default=func.now())
    notes = Column(Text)
    created_at = Column(DateTime, default=func.now())
    updated_at = Column(DateTime, default=func.now(), onupdate=func.now())
    
    # Relationships
    exercises = relationship("WorkoutExercise", back_populates="workout", cascade="all, delete-orphan")
    
    def __repr__(self):
        return f"<Workout(id={self.id}, date={self.date})>"


class WorkoutExercise(Base):
    """Workout exercise sets model."""
    
    __tablename__ = "workout_exercises"
    
    id = Column(Integer, primary_key=True, index=True)
    workout_id = Column(Integer, ForeignKey("workouts.id"), nullable=False)
    exercise_id = Column(Integer, ForeignKey("exercises.id"), nullable=False)
    sets_data = Column(JSON, nullable=False)  # List of sets with weight and reps
    notes = Column(Text)
    created_at = Column(DateTime, default=func.now())
    updated_at = Column(DateTime, default=func.now(), onupdate=func.now())
    
    # Relationships
    workout = relationship("Workout", back_populates="exercises")
    exercise = relationship("Exercise", back_populates="workout_exercises")
    
    def __repr__(self):
        return f"<WorkoutExercise(id={self.id}, workout_id={self.workout_id}, exercise_id={self.exercise_id})>" 