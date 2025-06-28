"""BMI calculation utilities for the backend."""

def calculate_bmi(weight_pounds: float, height_inches: float | None) -> float | None:
    """
    Calculate BMI from weight in pounds and height in inches.
    
    Args:
        weight_pounds: Weight in pounds
        height_inches: Height in inches
        
    Returns:
        BMI value or None if calculation not possible
    """
    if not height_inches or height_inches <= 0 or weight_pounds <= 0:
        return None
    
    # Convert inches to meters
    height_meters = height_inches * 0.0254
    
    # Convert pounds to kg
    weight_kg = weight_pounds * 0.453592
    
    # Calculate BMI: weight (kg) / height (m)²
    bmi = weight_kg / (height_meters * height_meters)
    
    return round(bmi, 1)


def get_bmi_category(bmi: float) -> str:
    """
    Get BMI category based on standard ranges.
    
    Args:
        bmi: BMI value
        
    Returns:
        Category string
    """
    if bmi < 18.5:
        return "Underweight"
    elif bmi < 25:
        return "Normal weight"
    elif bmi < 30:
        return "Overweight"
    else:
        return "Obese" 