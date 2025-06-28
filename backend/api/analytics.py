"""Analytics API router for charts and advanced analytics."""

from typing import Optional
from fastapi import APIRouter, Depends, HTTPException, Query
from sqlalchemy.orm import Session
import plotly.graph_objects as go
import plotly.express as px
import pandas as pd
from datetime import datetime, timedelta
import json

from backend.database import get_db
from backend.services.body_composition import BodyCompositionService
from backend.services.exercise import ExerciseService

router = APIRouter()


@router.get("/charts/weight-trend")
async def get_weight_trend_chart(
    days: int = Query(30, ge=1, le=365),
    db: Session = Depends(get_db)
):
    """Generate weight trend chart."""
    service = BodyCompositionService(db)
    trends = service.get_trends(days)
    
    if "error" in trends:
        raise HTTPException(status_code=404, detail=trends["error"])
    
    # Get raw data for chart
    start_date = datetime.now() - timedelta(days=days)
    measurements = service.get_measurements(limit=1000)
    
    # Filter by date range
    filtered_measurements = [
        m for m in measurements 
        if m.date >= start_date and m.weight is not None
    ]
    
    if not filtered_measurements:
        raise HTTPException(status_code=404, detail="No weight data found in the specified period")
    
    # Create chart data
    dates = [m.date for m in filtered_measurements]
    weights = [m.weight for m in filtered_measurements]
    
    # Create Plotly chart
    fig = go.Figure()
    
    fig.add_trace(go.Scatter(
        x=dates,
        y=weights,
        mode='lines+markers',
        name='Weight',
        line=dict(color='#3B82F6', width=2),
        marker=dict(size=6, color='#3B82F6')
    ))
    
    fig.update_layout(
        title=f"Weight Trend - Last {days} Days",
        xaxis_title="Date",
        yaxis_title="Weight (kg)",
        template="plotly_white",
        height=400
    )
    
    return {
        "chart_data": json.loads(fig.to_json()),
        "summary": {
            "current_weight": weights[-1] if weights else None,
            "weight_change": weights[-1] - weights[0] if len(weights) > 1 else 0,
            "average_weight": sum(weights) / len(weights) if weights else None
        }
    }


@router.get("/charts/body-composition")
async def get_body_composition_chart(
    days: int = Query(30, ge=1, le=365),
    db: Session = Depends(get_db)
):
    """Generate body composition chart (weight, body fat, muscle mass)."""
    service = BodyCompositionService(db)
    
    # Get measurements
    start_date = datetime.now() - timedelta(days=days)
    measurements = service.get_measurements(limit=1000)
    
    # Filter by date range
    filtered_measurements = [
        m for m in measurements 
        if m.date >= start_date
    ]
    
    if not filtered_measurements:
        raise HTTPException(status_code=404, detail="No body composition data found")
    
    # Create chart data
    dates = [m.date for m in filtered_measurements]
    weights = [m.weight for m in filtered_measurements]
    body_fat = [m.body_fat_percentage for m in filtered_measurements]
    muscle_mass = [m.muscle_mass for m in filtered_measurements]
    
    # Create Plotly chart with multiple traces
    fig = go.Figure()
    
    # Weight trace
    if any(w is not None for w in weights):
        fig.add_trace(go.Scatter(
            x=dates,
            y=weights,
            mode='lines+markers',
            name='Weight (kg)',
            line=dict(color='#3B82F6', width=2),
            yaxis='y'
        ))
    
    # Body fat trace (secondary y-axis)
    if any(bf is not None for bf in body_fat):
        fig.add_trace(go.Scatter(
            x=dates,
            y=body_fat,
            mode='lines+markers',
            name='Body Fat (%)',
            line=dict(color='#EF4444', width=2),
            yaxis='y2'
        ))
    
    # Muscle mass trace
    if any(mm is not None for mm in muscle_mass):
        fig.add_trace(go.Scatter(
            x=dates,
            y=muscle_mass,
            mode='lines+markers',
            name='Muscle Mass (kg)',
            line=dict(color='#10B981', width=2),
            yaxis='y'
        ))
    
    fig.update_layout(
        title=f"Body Composition - Last {days} Days",
        xaxis_title="Date",
        yaxis=dict(title="Weight/Muscle Mass (kg)", side="left"),
        yaxis2=dict(title="Body Fat (%)", side="right", overlaying="y"),
        template="plotly_white",
        height=400,
        legend=dict(x=0.02, y=0.98)
    )
    
    return {
        "chart_data": json.loads(fig.to_json()),
        "data_points": len(filtered_measurements)
    }


@router.get("/charts/exercise-progress")
async def get_exercise_progress_chart(
    exercise_id: int,
    days: int = Query(30, ge=1, le=365),
    db: Session = Depends(get_db)
):
    """Generate exercise progress chart."""
    service = ExerciseService(db)
    progress = service.get_exercise_progress(exercise_id, days)
    
    if "error" in progress:
        raise HTTPException(status_code=404, detail=progress["error"])
    
    # Get exercise details
    exercise = service.get_exercise(exercise_id)
    if not exercise:
        raise HTTPException(status_code=404, detail="Exercise not found")
    
    # Get workout data for this exercise
    start_date = datetime.now() - timedelta(days=days)
    workout_exercises = service.db.query(service.db.query(WorkoutExercise).join(Workout).filter(
        WorkoutExercise.exercise_id == exercise_id,
        Workout.date >= start_date
    ).order_by(Workout.date).all())
    
    if not workout_exercises:
        raise HTTPException(status_code=404, detail="No workout data found for this exercise")
    
    # Process data for chart
    dates = []
    max_weights = []
    total_volumes = []
    
    for we in workout_exercises:
        dates.append(we.workout.date)
        max_weight = 0
        total_volume = 0
        
        for set_data in we.sets_data:
            weight = set_data.get('weight', 0)
            reps = set_data.get('reps', 0)
            
            if weight and reps:
                max_weight = max(max_weight, weight)
                total_volume += weight * reps
        
        max_weights.append(max_weight)
        total_volumes.append(total_volume)
    
    # Create Plotly chart
    fig = go.Figure()
    
    # Max weight trace
    fig.add_trace(go.Scatter(
        x=dates,
        y=max_weights,
        mode='lines+markers',
        name='Max Weight (kg)',
        line=dict(color='#3B82F6', width=2),
        yaxis='y'
    ))
    
    # Volume trace (secondary y-axis)
    fig.add_trace(go.Scatter(
        x=dates,
        y=total_volumes,
        mode='lines+markers',
        name='Total Volume (kg)',
        line=dict(color='#F59E0B', width=2),
        yaxis='y2'
    ))
    
    fig.update_layout(
        title=f"{exercise.name} Progress - Last {days} Days",
        xaxis_title="Date",
        yaxis=dict(title="Max Weight (kg)", side="left"),
        yaxis2=dict(title="Total Volume (kg)", side="right", overlaying="y"),
        template="plotly_white",
        height=400,
        legend=dict(x=0.02, y=0.98)
    )
    
    return {
        "chart_data": json.loads(fig.to_json()),
        "exercise_name": exercise.name,
        "summary": progress
    }


@router.get("/charts/workout-frequency")
async def get_workout_frequency_chart(
    days: int = Query(30, ge=1, le=365),
    db: Session = Depends(get_db)
):
    """Generate workout frequency chart."""
    service = ExerciseService(db)
    
    # Get workouts
    start_date = datetime.now() - timedelta(days=days)
    workouts = service.get_workouts(limit=1000)
    
    # Filter by date range
    filtered_workouts = [
        w for w in workouts 
        if w.date >= start_date
    ]
    
    if not filtered_workouts:
        raise HTTPException(status_code=404, detail="No workout data found")
    
    # Group by date and count workouts
    workout_counts = {}
    for workout in filtered_workouts:
        date_str = workout.date.strftime('%Y-%m-%d')
        workout_counts[date_str] = workout_counts.get(date_str, 0) + 1
    
    # Create chart data
    dates = list(workout_counts.keys())
    counts = list(workout_counts.values())
    
    # Create Plotly chart
    fig = go.Figure()
    
    fig.add_trace(go.Bar(
        x=dates,
        y=counts,
        name='Workouts per Day',
        marker_color='#3B82F6'
    ))
    
    fig.update_layout(
        title=f"Workout Frequency - Last {days} Days",
        xaxis_title="Date",
        yaxis_title="Number of Workouts",
        template="plotly_white",
        height=400
    )
    
    return {
        "chart_data": json.loads(fig.to_json()),
        "summary": {
            "total_workouts": len(filtered_workouts),
            "workout_days": len(workout_counts),
            "average_workouts_per_day": len(filtered_workouts) / days
        }
    }


@router.get("/summary/dashboard")
async def get_dashboard_summary(
    days: int = Query(30, ge=1, le=365),
    db: Session = Depends(get_db)
):
    """Get comprehensive dashboard summary with all key metrics."""
    body_service = BodyCompositionService(db)
    exercise_service = ExerciseService(db)
    
    try:
        # Body composition summary
        latest_measurement = body_service.get_latest_measurement()
        body_trends = body_service.get_trends(days)
        body_stats = body_service.get_statistics()
        
        # Exercise summary
        workout_stats = exercise_service.get_workout_statistics(days)
        recent_workouts = exercise_service.get_workouts(limit=5)
        
        # Calculate key metrics
        summary = {
            "period_days": days,
            "body_composition": {
                "current_weight": latest_measurement.weight if latest_measurement else None,
                "weight_change_30d": body_trends.get('weight', {}).get('change', 0) if 'weight' in body_trends else 0,
                "total_measurements": body_stats.get('total_measurements', 0),
                "latest_measurement_date": latest_measurement.date.isoformat() if latest_measurement else None
            },
            "workouts": {
                "total_workouts": workout_stats.get('total_workouts', 0),
                "workout_frequency_per_week": workout_stats.get('workout_frequency_per_week', 0),
                "average_duration_minutes": workout_stats.get('average_duration_minutes', 0),
                "last_workout_date": recent_workouts[0].date.isoformat() if recent_workouts else None
            },
            "trends": {
                "body_composition": body_trends,
                "workouts": workout_stats
            }
        }
        
        return summary
        
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e)) 