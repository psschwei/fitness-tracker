"""Body composition API router."""

from typing import List, Optional
from fastapi import APIRouter, Depends, HTTPException, Query
from sqlalchemy.orm import Session

from backend.database import get_db
from backend.services.body_composition import BodyCompositionService
from backend.schemas.body_composition import (
    BodyCompositionCreate, BodyCompositionUpdate, BodyCompositionResponse
)

router = APIRouter()


@router.post("/", response_model=BodyCompositionResponse)
async def create_body_composition(
    measurement: BodyCompositionCreate,
    db: Session = Depends(get_db)
):
    """Create a new body composition measurement."""
    service = BodyCompositionService(db)
    try:
        return service.add_measurement(measurement)
    except Exception as e:
        raise HTTPException(status_code=400, detail=str(e))


@router.get("/", response_model=List[BodyCompositionResponse])
async def get_body_compositions(
    limit: int = Query(100, ge=1, le=1000),
    offset: int = Query(0, ge=0),
    db: Session = Depends(get_db)
):
    """Get body composition measurements with pagination."""
    service = BodyCompositionService(db)
    return service.get_measurements(limit=limit, offset=offset)


@router.get("/latest", response_model=BodyCompositionResponse)
async def get_latest_body_composition(db: Session = Depends(get_db)):
    """Get the most recent body composition measurement."""
    service = BodyCompositionService(db)
    measurement = service.get_latest_measurement()
    if not measurement:
        raise HTTPException(status_code=404, detail="No measurements found")
    return measurement


@router.get("/{measurement_id}", response_model=BodyCompositionResponse)
async def get_body_composition(
    measurement_id: int,
    db: Session = Depends(get_db)
):
    """Get a specific body composition measurement."""
    service = BodyCompositionService(db)
    measurement = service.get_measurement(measurement_id)
    if not measurement:
        raise HTTPException(status_code=404, detail="Measurement not found")
    return measurement


@router.put("/{measurement_id}", response_model=BodyCompositionResponse)
async def update_body_composition(
    measurement_id: int,
    measurement: BodyCompositionUpdate,
    db: Session = Depends(get_db)
):
    """Update a body composition measurement."""
    service = BodyCompositionService(db)
    updated_measurement = service.update_measurement(measurement_id, measurement)
    if not updated_measurement:
        raise HTTPException(status_code=404, detail="Measurement not found")
    return updated_measurement


@router.delete("/{measurement_id}")
async def delete_body_composition(
    measurement_id: int,
    db: Session = Depends(get_db)
):
    """Delete a body composition measurement."""
    service = BodyCompositionService(db)
    success = service.delete_measurement(measurement_id)
    if not success:
        raise HTTPException(status_code=404, detail="Measurement not found")
    return {"message": "Measurement deleted successfully"}


@router.get("/trends/{days}")
async def get_body_composition_trends(
    days: int,
    db: Session = Depends(get_db)
):
    """Get body composition trends over the specified number of days."""
    if days < 1 or days > 365:
        raise HTTPException(status_code=400, detail="Days must be between 1 and 365")
    
    service = BodyCompositionService(db)
    trends = service.get_trends(days)
    if "error" in trends:
        raise HTTPException(status_code=404, detail=trends["error"])
    return trends


@router.get("/statistics/overview")
async def get_body_composition_statistics(db: Session = Depends(get_db)):
    """Get overall body composition statistics."""
    service = BodyCompositionService(db)
    stats = service.get_statistics()
    if "error" in stats:
        raise HTTPException(status_code=404, detail=stats["error"])
    return stats 