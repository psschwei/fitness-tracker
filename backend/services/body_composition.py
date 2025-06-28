"""Body composition service with business logic."""

from datetime import datetime, timedelta
from typing import List, Dict, Any, Optional
from sqlalchemy.orm import Session
from sqlalchemy import desc, func
import pandas as pd

from backend.models.body_composition import BodyComposition
from backend.schemas.body_composition import BodyCompositionCreate, BodyCompositionUpdate


class BodyCompositionService:
    """Service for body composition business logic."""
    
    def __init__(self, db: Session):
        self.db = db
    
    def add_measurement(self, measurement_data: BodyCompositionCreate) -> BodyComposition:
        """Add a new body composition measurement."""
        db_measurement = BodyComposition(
            date=measurement_data.date or datetime.now(),
            weight_pounds=measurement_data.weight_pounds,
            waist_inches=measurement_data.waist_inches,
            notes=measurement_data.notes
        )
        
        self.db.add(db_measurement)
        self.db.commit()
        self.db.refresh(db_measurement)
        return db_measurement
    
    def get_measurements(self, limit: int = 100, offset: int = 0) -> List[BodyComposition]:
        """Get body composition measurements with pagination."""
        return self.db.query(BodyComposition).order_by(desc(BodyComposition.date)).offset(offset).limit(limit).all()
    
    def get_measurement(self, measurement_id: int) -> Optional[BodyComposition]:
        """Get a specific body composition measurement."""
        return self.db.query(BodyComposition).filter(BodyComposition.id == measurement_id).first()
    
    def update_measurement(self, measurement_id: int, update_data: BodyCompositionUpdate) -> Optional[BodyComposition]:
        """Update a body composition measurement."""
        measurement = self.get_measurement(measurement_id)
        if not measurement:
            return None
        
        update_dict = update_data.dict(exclude_unset=True)
        for field, value in update_dict.items():
            setattr(measurement, field, value)
        
        measurement.updated_at = datetime.now()
        self.db.commit()
        self.db.refresh(measurement)
        return measurement
    
    def delete_measurement(self, measurement_id: int) -> bool:
        """Delete a body composition measurement."""
        measurement = self.get_measurement(measurement_id)
        if not measurement:
            return False
        
        self.db.delete(measurement)
        self.db.commit()
        return True
    
    def get_latest_measurement(self) -> Optional[BodyComposition]:
        """Get the most recent body composition measurement."""
        return self.db.query(BodyComposition).order_by(desc(BodyComposition.date)).first()
    
    def get_measurement_by_date(self, date: datetime) -> Optional[BodyComposition]:
        """Get body composition measurement for a specific date."""
        return self.db.query(BodyComposition).filter(
            func.date(BodyComposition.date) == func.date(date)
        ).first()
    
    def get_trends(self, days: int = 30) -> Dict[str, Any]:
        """Get body composition trends over the specified number of days."""
        start_date = datetime.now() - timedelta(days=days)
        
        measurements = self.db.query(BodyComposition).filter(
            BodyComposition.date >= start_date
        ).order_by(BodyComposition.date).all()
        
        if not measurements:
            return {"error": "No measurements found in the specified period"}
        
        # Convert to pandas DataFrame for analysis
        df = pd.DataFrame([
            {
                'date': m.date,
                'weight_pounds': m.weight_pounds,
                'waist_inches': m.waist_inches
            }
            for m in measurements
        ])
        
        # Calculate trends
        trends = {
            'total_measurements': len(measurements),
            'date_range': {
                'start': df['date'].min().isoformat(),
                'end': df['date'].max().isoformat()
            },
            'weight': {
                'current': float(df['weight_pounds'].iloc[-1]) if not df['weight_pounds'].isna().all() else None,
                'change': float(df['weight_pounds'].iloc[-1] - df['weight_pounds'].iloc[0]) if len(df) > 1 else 0,
                'average': float(df['weight_pounds'].mean()) if not df['weight_pounds'].isna().all() else None,
                'min': float(df['weight_pounds'].min()) if not df['weight_pounds'].isna().all() else None,
                'max': float(df['weight_pounds'].max()) if not df['weight_pounds'].isna().all() else None
            }
        }
        
        # Add waist trends if available
        if not df['waist_inches'].isna().all():
            trends['waist'] = {
                'current': float(df['waist_inches'].iloc[-1]),
                'change': float(df['waist_inches'].iloc[-1] - df['waist_inches'].iloc[0]) if len(df) > 1 else 0,
                'average': float(df['waist_inches'].mean()),
                'min': float(df['waist_inches'].min()),
                'max': float(df['waist_inches'].max())
            }
        
        return trends
    
    def get_statistics(self) -> Dict[str, Any]:
        """Get overall body composition statistics."""
        total_measurements = self.db.query(BodyComposition).count()
        
        if total_measurements == 0:
            return {"error": "No measurements available"}
        
        # Get first and latest measurements
        first_measurement = self.db.query(BodyComposition).order_by(BodyComposition.date).first()
        latest_measurement = self.get_latest_measurement()
        
        # Calculate overall change
        weight_change = latest_measurement.weight_pounds - first_measurement.weight_pounds if first_measurement != latest_measurement else 0
        
        # Get average weight
        avg_weight = self.db.query(func.avg(BodyComposition.weight_pounds)).scalar()
        
        return {
            'total_measurements': total_measurements,
            'first_measurement_date': first_measurement.date.isoformat(),
            'latest_measurement_date': latest_measurement.date.isoformat(),
            'weight_change_total': weight_change,
            'average_weight': float(avg_weight) if avg_weight else None,
            'current_weight': latest_measurement.weight_pounds,
            'starting_weight': first_measurement.weight_pounds
        } 