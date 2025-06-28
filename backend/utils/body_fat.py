"""Body fat percentage calculation using Navy tape test method."""

import math
from typing import Optional

def calculate_body_fat_percentage(
    waist_inches: float,
    neck_inches: float, 
    height_inches: float,
    is_male: bool
) -> Optional[float]:
    """
    Calculate body fat percentage using the Navy tape test method.
    
    Args:
        waist_inches: Waist circumference in inches
        neck_inches: Neck circumference in inches  
        height_inches: Height in inches
        is_male: True for male, False for female
        
    Returns:
        Body fat percentage or None if calculation fails
    """
    try:
        # Validate inputs
        if not all([waist_inches, neck_inches, height_inches]):
            return None
            
        if waist_inches <= 0 or neck_inches <= 0 or height_inches <= 0:
            return None
            
        if waist_inches <= neck_inches:
            return None
            
        # Navy tape test formula
        # For males: BF% = 86.010 × log10(waist - neck) - 70.041 × log10(height) + 36.76
        # For females: BF% = 163.205 × log10(waist + hip - neck) - 97.684 × log10(height) - 78.387
        
        if is_male:
            waist_neck_diff = waist_inches - neck_inches
            body_fat = (86.010 * math.log10(waist_neck_diff)) - (70.041 * math.log10(height_inches)) + 36.76
            
            # The result is already the body fat percentage
            body_fat_percentage = body_fat
        else:
            # For females, we need hip measurement which we don't have
            # For now, return None for females
            return None
            
        # Validate result is reasonable (0-50% range)
        if 0 <= body_fat_percentage <= 50:
            return round(body_fat_percentage, 1)
        else:
            return None
            
    except (ValueError, ZeroDivisionError, math.DomainError):
        return None


def get_body_fat_category(body_fat: float, is_male: bool = True) -> str:
    """
    Get body fat category based on percentage.
    
    Args:
        body_fat: Body fat percentage
        is_male: True for male, False for female
        
    Returns:
        Category string
    """
    if is_male:
        if body_fat < 6:
            return "Essential fat"
        elif body_fat < 14:
            return "Athlete"
        elif body_fat < 18:
            return "Fitness"
        elif body_fat < 25:
            return "Average"
        else:
            return "Obese"
    else:
        if body_fat < 14:
            return "Essential fat"
        elif body_fat < 21:
            return "Athlete"
        elif body_fat < 25:
            return "Fitness"
        elif body_fat < 32:
            return "Average"
        else:
            return "Obese"


def format_body_fat(body_fat: float | None, is_male: bool = True) -> str:
    """
    Format body fat percentage with category.
    
    Args:
        body_fat: Body fat percentage
        is_male: True for male, False for female
        
    Returns:
        Formatted string
    """
    if body_fat is None:
        return "Not available"
    
    category = get_body_fat_category(body_fat, is_male)
    return f"{body_fat}% ({category})" 