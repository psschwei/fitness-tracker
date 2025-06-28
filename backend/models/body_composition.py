"""Body composition database model."""

from datetime import datetime
from sqlalchemy import Column, Integer, Float, DateTime, String, Text, Boolean
from sqlalchemy.sql import func

from backend.database import Base


class BodyComposition(Base):
    """Body composition measurements model."""
    
    __tablename__ = "body_composition"
    
    id = Column(Integer, primary_key=True, index=True)
    date = Column(DateTime, nullable=False, default=func.now())
    weight_pounds = Column(Float, nullable=False)  # Weight in pounds
    height_inches = Column(Float)  # Height in inches
    waist_inches = Column(Float)  # Waist circumference in inches
    neck_inches = Column(Float)  # Neck circumference in inches
    bmi = Column(Float)  # Calculated BMI
    body_fat_percentage = Column(Float)  # Calculated body fat percentage
    is_male = Column(Boolean, default=True)  # Gender for body fat calculation
    notes = Column(Text)
    created_at = Column(DateTime, default=func.now())
    updated_at = Column(DateTime, default=func.now(), onupdate=func.now())
    
    def __repr__(self):
        return f"<BodyComposition(id={self.id}, date={self.date}, weight={self.weight_pounds}lbs)>" 