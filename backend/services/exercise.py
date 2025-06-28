"""Exercise service with business logic."""

from datetime import datetime, timedelta, date
from typing import List, Dict, Any, Optional
from sqlalchemy.orm import Session
from sqlalchemy import desc, func, and_
import pandas as pd

from backend.models.exercise import Exercise, Workout, WorkoutExercise, DailyActivity
from backend.schemas.exercise import (
    ExerciseCreate, ExerciseUpdate, WorkoutCreate, WorkoutUpdate,
    DailyActivityCreate, DailyActivityUpdate
)


class ExerciseService:
    """Service for exercise business logic."""
    
    def __init__(self, db: Session):
        self.db = db
    
    # Exercise management
    def create_exercise(self, exercise_data: ExerciseCreate) -> Exercise:
        """Create a new exercise in the library."""
        db_exercise = Exercise(
            name=exercise_data.name,
            category=exercise_data.category
        )
        
        self.db.add(db_exercise)
        self.db.commit()
        self.db.refresh(db_exercise)
        return db_exercise
    
    def get_exercises(self, category: Optional[str] = None, active_only: bool = True) -> List[Exercise]:
        """Get exercises with optional filtering."""
        query = self.db.query(Exercise)
        
        if active_only:
            query = query.filter(Exercise.is_active == 1)
        
        if category:
            query = query.filter(Exercise.category == category)
        
        return query.order_by(Exercise.name).all()
    
    def get_exercise(self, exercise_id: int) -> Optional[Exercise]:
        """Get a specific exercise."""
        return self.db.query(Exercise).filter(Exercise.id == exercise_id).first()
    
    def update_exercise(self, exercise_id: int, update_data: ExerciseUpdate) -> Optional[Exercise]:
        """Update an exercise."""
        exercise = self.get_exercise(exercise_id)
        if not exercise:
            return None
        
        update_dict = update_data.dict(exclude_unset=True)
        for field, value in update_dict.items():
            setattr(exercise, field, value)
        
        exercise.updated_at = datetime.now()
        self.db.commit()
        self.db.refresh(exercise)
        return exercise
    
    def delete_exercise(self, exercise_id: int) -> bool:
        """Delete an exercise (soft delete by setting inactive)."""
        exercise = self.get_exercise(exercise_id)
        if not exercise:
            return False
        
        exercise.is_active = 0
        exercise.updated_at = datetime.now()
        self.db.commit()
        return True
    
    # Workout management
    def create_workout(self, workout_data: WorkoutCreate) -> Workout:
        """Create a new workout with exercises."""
        # Create the workout
        db_workout = Workout(
            date=workout_data.date or datetime.now(),
            notes=workout_data.notes
        )
        
        self.db.add(db_workout)
        self.db.flush()  # Get the workout ID
        
        # Add exercises to the workout
        for exercise_data in workout_data.exercises:
            # Handle both old format (sets_data) and new format (weight, reps_per_set)
            if hasattr(exercise_data, 'sets_data') and exercise_data.sets_data:
                # Old format with sets_data
                sets_data = [set_data.dict() for set_data in exercise_data.sets_data]
            else:
                # New format with weight and reps_per_set
                sets_data = [{
                    'weight': exercise_data.weight,
                    'reps': exercise_data.reps_per_set
                }]
            
            workout_exercise = WorkoutExercise(
                workout_id=db_workout.id,
                exercise_id=exercise_data.exercise_id,
                sets_data=sets_data,
                notes=exercise_data.notes
            )
            self.db.add(workout_exercise)
        
        self.db.commit()
        self.db.refresh(db_workout)
        return db_workout
    
    def get_workouts(self, limit: int = 100, offset: int = 0) -> List[Workout]:
        """Get workouts with pagination."""
        return self.db.query(Workout).order_by(desc(Workout.date)).offset(offset).limit(limit).all()
    
    def get_workout(self, workout_id: int) -> Optional[Workout]:
        """Get a specific workout with exercises."""
        return self.db.query(Workout).filter(Workout.id == workout_id).first()
    
    def get_workouts_by_date(self, target_date: date) -> List[Workout]:
        """Get workouts for a specific date."""
        return self.db.query(Workout).filter(
            func.date(Workout.date) == target_date
        ).order_by(Workout.date).all()
    
    def update_workout(self, workout_id: int, update_data: WorkoutUpdate) -> Optional[Workout]:
        """Update a workout."""
        workout = self.get_workout(workout_id)
        if not workout:
            return None
        
        update_dict = update_data.dict(exclude_unset=True)
        for field, value in update_dict.items():
            setattr(workout, field, value)
        
        workout.updated_at = datetime.now()
        self.db.commit()
        self.db.refresh(workout)
        return workout
    
    def delete_workout(self, workout_id: int) -> bool:
        """Delete a workout and all its exercises."""
        workout = self.get_workout(workout_id)
        if not workout:
            return False
        
        self.db.delete(workout)
        self.db.commit()
        return True
    
    # Progress tracking
    def get_exercise_progress(self, exercise_id: int, days: int = 30) -> Dict[str, Any]:
        """Get progress for a specific exercise over time."""
        start_date = datetime.now() - timedelta(days=days)
        
        # Get all workout exercises for this exercise
        workout_exercises = self.db.query(WorkoutExercise).join(Workout).filter(
            WorkoutExercise.exercise_id == exercise_id,
            Workout.date >= start_date
        ).order_by(Workout.date).all()
        
        if not workout_exercises:
            return {"error": f"No workouts found for exercise {exercise_id} in the last {days} days"}
        
        # Analyze progress
        progress_data = []
        for we in workout_exercises:
            workout_date = we.workout.date
            max_weight = 0
            total_volume = 0
            
            for set_data in we.sets_data:
                weight = set_data.get('weight', 0)
                reps = set_data.get('reps', 0)
                
                if weight and reps:
                    max_weight = max(max_weight, weight)
                    total_volume += weight * reps
            
            progress_data.append({
                'date': workout_date,
                'max_weight': max_weight,
                'total_volume': total_volume,
                'sets_count': len(we.sets_data)
            })
        
        # Calculate trends
        df = pd.DataFrame(progress_data)
        
        if len(df) == 0:
            return {"error": "No valid exercise data found"}
        
        trends = {
            'total_workouts': len(workout_exercises),
            'date_range': {
                'start': df['date'].min().isoformat(),
                'end': df['date'].max().isoformat()
            },
            'max_weight': {
                'current': float(df['max_weight'].iloc[-1]) if not df['max_weight'].isna().all() else None,
                'best': float(df['max_weight'].max()) if not df['max_weight'].isna().all() else None,
                'improvement': float(df['max_weight'].iloc[-1] - df['max_weight'].iloc[0]) if len(df) > 1 else 0
            },
            'volume': {
                'current': float(df['total_volume'].iloc[-1]) if not df['total_volume'].isna().all() else None,
                'best': float(df['total_volume'].max()) if not df['total_volume'].isna().all() else None,
                'average': float(df['total_volume'].mean()) if not df['total_volume'].isna().all() else None
            }
        }
        
        return trends
    
    def calculate_1rm(self, weight: float, reps: int) -> float:
        """Calculate one-rep max using Epley formula."""
        if reps <= 1:
            return weight
        return weight * (1 + reps / 30)
    
    def get_workout_statistics(self, days: int = 30) -> Dict[str, Any]:
        """Get overall workout statistics."""
        start_date = datetime.now() - timedelta(days=days)
        
        workouts = self.db.query(Workout).filter(Workout.date >= start_date).all()
        
        if not workouts:
            return {"error": f"No workouts found in the last {days} days"}
        
        total_workouts = len(workouts)
        
        # Count exercises per workout
        exercise_counts = []
        for workout in workouts:
            exercise_count = self.db.query(WorkoutExercise).filter(
                WorkoutExercise.workout_id == workout.id
            ).count()
            exercise_counts.append(exercise_count)
        
        avg_exercises_per_workout = sum(exercise_counts) / len(exercise_counts) if exercise_counts else 0
        
        return {
            'total_workouts': total_workouts,
            'average_exercises_per_workout': avg_exercises_per_workout,
            'workout_frequency_per_week': total_workouts / (days / 7),
            'date_range': {
                'start': start_date.isoformat(),
                'end': datetime.now().isoformat()
            }
        }


class DailyActivityService:
    """Service for daily activity operations."""
    
    @staticmethod
    def get_daily_activities(db: Session, skip: int = 0, limit: int = 100) -> List[DailyActivity]:
        """Get all daily activities."""
        return db.query(DailyActivity).order_by(DailyActivity.date.desc()).offset(skip).limit(limit).all()
    
    @staticmethod
    def get_daily_activity(db: Session, activity_id: int) -> Optional[DailyActivity]:
        """Get a specific daily activity by ID."""
        return db.query(DailyActivity).filter(DailyActivity.id == activity_id).first()
    
    @staticmethod
    def get_daily_activity_by_date(db: Session, target_date: date) -> Optional[DailyActivity]:
        """Get daily activity for a specific date."""
        return db.query(DailyActivity).filter(
            DailyActivity.date >= target_date,
            DailyActivity.date < target_date.replace(day=target_date.day + 1)
        ).first()
    
    @staticmethod
    def create_or_update_daily_activity(db: Session, activity: DailyActivityCreate) -> DailyActivity:
        """Create or update daily activity for a specific date."""
        activity_data = activity.model_dump()
        if not activity_data.get('date'):
            activity_data['date'] = datetime.now()
        
        # Convert to date for comparison
        target_date = activity_data['date'].date()
        
        # Check if activity already exists for this date
        existing_activity = DailyActivityService.get_daily_activity_by_date(db, target_date)
        
        if existing_activity:
            # Update existing activity
            update_data = {k: v for k, v in activity_data.items() if k != 'date' and v is not None}
            for field, value in update_data.items():
                setattr(existing_activity, field, value)
            db.commit()
            db.refresh(existing_activity)
            return existing_activity
        else:
            # Create new activity
            db_activity = DailyActivity(**activity_data)
            db.add(db_activity)
            db.commit()
            db.refresh(db_activity)
            return db_activity
    
    @staticmethod
    def update_daily_activity(db: Session, activity_id: int, activity: DailyActivityUpdate) -> Optional[DailyActivity]:
        """Update a daily activity."""
        db_activity = db.query(DailyActivity).filter(DailyActivity.id == activity_id).first()
        if db_activity:
            update_data = activity.model_dump(exclude_unset=True)
            for field, value in update_data.items():
                setattr(db_activity, field, value)
            db.commit()
            db.refresh(db_activity)
        return db_activity
    
    @staticmethod
    def delete_daily_activity(db: Session, activity_id: int) -> bool:
        """Delete a daily activity."""
        db_activity = db.query(DailyActivity).filter(DailyActivity.id == activity_id).first()
        if db_activity:
            db.delete(db_activity)
            db.commit()
            return True
        return False


class WorkoutService:
    """Service for workout operations."""
    
    @staticmethod
    def get_workouts(db: Session, skip: int = 0, limit: int = 100) -> List[Workout]:
        """Get all workouts."""
        return db.query(Workout).order_by(Workout.date.desc()).offset(skip).limit(limit).all()
    
    @staticmethod
    def get_workout(db: Session, workout_id: int) -> Optional[Workout]:
        """Get a specific workout by ID."""
        return db.query(Workout).filter(Workout.id == workout_id).first()
    
    @staticmethod
    def get_workouts_by_date(db: Session, target_date: date) -> List[Workout]:
        """Get workouts for a specific date."""
        return db.query(Workout).filter(
            Workout.date >= target_date,
            Workout.date < target_date.replace(day=target_date.day + 1)
        ).all()
    
    @staticmethod
    def create_workout(db: Session, workout: WorkoutCreate) -> Workout:
        """Create a new workout with exercises."""
        workout_data = workout.model_dump(exclude={'exercises'})
        if not workout_data.get('date'):
            workout_data['date'] = datetime.now()
        
        db_workout = Workout(**workout_data)
        db.add(db_workout)
        db.commit()
        db.refresh(db_workout)
        
        # Add exercises
        for exercise_data in workout.exercises:
            db_workout_exercise = WorkoutExercise(
                workout_id=db_workout.id,
                exercise_id=exercise_data.exercise_id,
                sets_data=[set_data.model_dump() for set_data in exercise_data.sets_data],
                notes=exercise_data.notes
            )
            db.add(db_workout_exercise)
        
        db.commit()
        db.refresh(db_workout)
        return db_workout
    
    @staticmethod
    def update_workout(db: Session, workout_id: int, workout: WorkoutUpdate) -> Optional[Workout]:
        """Update a workout."""
        db_workout = db.query(Workout).filter(Workout.id == workout_id).first()
        if db_workout:
            update_data = workout.model_dump(exclude_unset=True)
            for field, value in update_data.items():
                setattr(db_workout, field, value)
            db.commit()
            db.refresh(db_workout)
        return db_workout
    
    @staticmethod
    def delete_workout(db: Session, workout_id: int) -> bool:
        """Delete a workout and all its exercises."""
        db_workout = db.query(Workout).filter(Workout.id == workout_id).first()
        if db_workout:
            db.delete(db_workout)
            db.commit()
            return True
        return False 