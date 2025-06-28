"""Goals database model."""

from datetime import datetime
from sqlalchemy import Column, Integer, Float, DateTime, String, Text, Boolean
from sqlalchemy.sql import func

from backend.database import Base


class Goal(Base):
    """Fitness goals model."""
    
    __tablename__ = "goals"
    
    id = Column(Integer, primary_key=True, index=True)
    name = Column(String(255), nullable=False)
    description = Column(Text)
    goal_type = Column(String(50), nullable=False)  # weight, body_fat, strength, endurance
    target_value = Column(Float, nullable=False)
    current_value = Column(Float)
    unit = Column(String(20), nullable=False)  # kg, %, lbs, etc.
    start_date = Column(DateTime, nullable=False, default=func.now())
    target_date = Column(DateTime)
    is_completed = Column(Boolean, default=False)
    is_active = Column(Boolean, default=True)
    created_at = Column(DateTime, default=func.now())
    updated_at = Column(DateTime, default=func.now(), onupdate=func.now())
    
    def __repr__(self):
        return f"<Goal(id={self.id}, name='{self.name}', target={self.target_value}{self.unit})>" 