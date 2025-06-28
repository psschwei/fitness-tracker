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
export const convertWeight = (value: number, from: WeightUnit, to: WeightUnit): number => {
  if (from === to) return value
  
  if (from === 'lbs' && to === 'kg') {
    return value * LBS_TO_KG
  } else if (from === 'kg' && to === 'lbs') {
    return value * KG_TO_LBS
  }
  
  return value
}

// Length conversions
export const convertLength = (value: number, from: LengthUnit, to: LengthUnit): number => {
  if (from === to) return value
  
  if (from === 'inches' && to === 'cm') {
    return value * INCHES_TO_CM
  } else if (from === 'cm' && to === 'inches') {
    return value * CM_TO_INCHES
  }
  
  return value
}

// Format weight with unit
export const formatWeight = (value: number, unit: WeightUnit): string => {
  return `${value.toFixed(1)} ${unit}`
}

// Format length with unit
export const formatLength = (value: number, unit: LengthUnit): string => {
  return `${value.toFixed(1)} ${unit}`
}

// Get unit symbol
export const getWeightUnitSymbol = (unit: WeightUnit): string => {
  return unit === 'lbs' ? 'lbs' : 'kg'
}

export const getLengthUnitSymbol = (unit: LengthUnit): string => {
  return unit === 'inches' ? 'in' : 'cm'
} 