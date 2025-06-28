// Unit conversion utilities

export type WeightUnit = 'lbs' | 'kg'
export type LengthUnit = 'inches' | 'cm'

export interface UnitPreferences {
  bodyWeight: WeightUnit
  exerciseWeight: WeightUnit
  length: LengthUnit
}

// Conversion factors
const LBS_TO_KG = 0.453592
const KG_TO_LBS = 2.20462
const INCHES_TO_CM = 2.54
const CM_TO_INCHES = 0.393701

// Weight conversions
export const convertWeight = (value: number | null | undefined, from: WeightUnit, to: WeightUnit): number => {
  if (value === null || value === undefined || isNaN(value)) {
    return 0
  }
  
  if (from === to) return value
  
  if (from === 'lbs' && to === 'kg') {
    return value * LBS_TO_KG
  } else if (from === 'kg' && to === 'lbs') {
    return value * KG_TO_LBS
  }
  
  return value
}

// Length conversions
export const convertLength = (value: number | null | undefined, from: LengthUnit, to: LengthUnit): number => {
  if (value === null || value === undefined || isNaN(value)) {
    return 0
  }
  
  if (from === to) return value
  
  if (from === 'inches' && to === 'cm') {
    return value * INCHES_TO_CM
  } else if (from === 'cm' && to === 'inches') {
    return value * CM_TO_INCHES
  }
  
  return value
}

// Format weight with unit
export const formatWeight = (value: number | null | undefined, unit: WeightUnit): string => {
  if (value === null || value === undefined || isNaN(value)) {
    return 'Not recorded'
  }
  return `${value.toFixed(1)} ${unit}`
}

// Format length with unit
export const formatLength = (value: number | null | undefined, unit: LengthUnit): string => {
  if (value === null || value === undefined || isNaN(value)) {
    return 'Not recorded'
  }
  return `${value.toFixed(1)} ${unit}`
}

// Get unit symbol
export const getWeightUnitSymbol = (unit: WeightUnit): string => {
  return unit === 'lbs' ? 'lbs' : 'kg'
}

export const getLengthUnitSymbol = (unit: LengthUnit): string => {
  return unit === 'inches' ? 'in' : 'cm'
}

// BMI calculation
export const calculateBMI = (weight_lbs: number, height_inches: number | null): number | null => {
  if (!height_inches || height_inches <= 0 || weight_lbs <= 0) {
    return null
  }
  
  // Convert inches to meters
  const height_meters = height_inches * 0.0254
  
  // Convert pounds to kg
  const weight_kg = weight_lbs * LBS_TO_KG
  
  // Calculate BMI: weight (kg) / height (m)²
  const bmi = weight_kg / (height_meters * height_meters)
  
  return bmi
}

// Get BMI category
export const getBMICategory = (bmi: number): string => {
  if (bmi < 18.5) return 'Underweight'
  if (bmi < 25) return 'Normal weight'
  if (bmi < 30) return 'Overweight'
  return 'Obese'
}

// Format BMI with category
export const formatBMI = (bmi: number | null): string => {
  if (bmi === null) return 'Not available'
  const category = getBMICategory(bmi)
  return `${bmi.toFixed(1)} (${category})`
} 