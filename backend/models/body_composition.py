"""Body composition database model."""

from datetime import datetime
from sqlalchemy import Column, Integer, Float, DateTime, String, Text
from sqlalchemy.sql import func

from backend.database import Base


class BodyComposition(Base):
    """Body composition measurements model."""
    
    __tablename__ = "body_composition"
    
    id = Column(Integer, primary_key=True, index=True)
    date = Column(DateTime, nullable=False, default=func.now())
    weight_pounds = Column(Float, nullable=False)  # Weight in pounds
    waist_inches = Column(Float)  # Waist circumference in inches
    notes = Column(Text)
    created_at = Column(DateTime, default=func.now())
    updated_at = Column(DateTime, default=func.now(), onupdate=func.now())
    
    def __repr__(self):
        return f"<BodyComposition(id={self.id}, date={self.date}, weight={self.weight_pounds}lbs)>" 