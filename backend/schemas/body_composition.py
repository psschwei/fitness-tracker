"""Pydantic schemas for body composition API."""

from datetime import datetime
from typing import Optional
from pydantic import BaseModel, Field


class BodyCompositionBase(BaseModel):
    """Base body composition schema."""
    weight_pounds: float = Field(..., gt=0, description="Weight in pounds")
    height_inches: Optional[float] = Field(None, gt=0, description="Height in inches")
    waist_inches: Optional[float] = Field(None, gt=0, description="Waist circumference in inches")
    neck_inches: Optional[float] = Field(None, gt=0, description="Neck circumference in inches")
    bmi: Optional[float] = Field(None, description="Calculated BMI")
    body_fat_percentage: Optional[float] = Field(None, description="Calculated body fat percentage")
    is_male: bool = Field(True, description="Gender for body fat calculation")
    notes: Optional[str] = Field(None, description="Additional notes")


class BodyCompositionCreate(BodyCompositionBase):
    """Schema for creating a new body composition entry."""
    date: Optional[datetime] = Field(None, description="Measurement date (defaults to now)")


class BodyCompositionUpdate(BaseModel):
    """Schema for updating a body composition entry."""
    weight_pounds: Optional[float] = Field(None, gt=0, description="Weight in pounds")
    height_inches: Optional[float] = Field(None, gt=0, description="Height in inches")
    waist_inches: Optional[float] = Field(None, gt=0, description="Waist circumference in inches")
    neck_inches: Optional[float] = Field(None, gt=0, description="Neck circumference in inches")
    bmi: Optional[float] = Field(None, description="Calculated BMI")
    body_fat_percentage: Optional[float] = Field(None, description="Calculated body fat percentage")
    is_male: Optional[bool] = Field(None, description="Gender for body fat calculation")
    notes: Optional[str] = Field(None, description="Additional notes")


class BodyCompositionResponse(BodyCompositionBase):
    """Schema for body composition response."""
    id: int
    date: datetime
    created_at: datetime
    updated_at: datetime

    class Config:
        from_attributes = True 