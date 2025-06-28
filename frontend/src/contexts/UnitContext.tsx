import React, { createContext, useContext, useState, useEffect, ReactNode } from 'react'
import { UnitPreferences, WeightUnit, LengthUnit } from '../utils/units'

interface UnitContextType {
  units: UnitPreferences
  setBodyWeightUnit: (unit: WeightUnit) => void
  setExerciseWeightUnit: (unit: WeightUnit) => void
  setLengthUnit: (unit: LengthUnit) => void
  toggleBodyWeightUnit: () => void
  toggleExerciseWeightUnit: () => void
  toggleLengthUnit: () => void
}

const UnitContext = createContext<UnitContextType | undefined>(undefined)

const DEFAULT_UNITS: UnitPreferences = {
  bodyWeight: 'lbs',
  exerciseWeight: 'lbs',
  length: 'inches'
}

export function UnitProvider({ children }: { children: ReactNode }) {
  const [units, setUnits] = useState<UnitPreferences>(DEFAULT_UNITS)

  // Load units from localStorage on mount
  useEffect(() => {
    const savedUnits = localStorage.getItem('fitness-tracker-units')
    if (savedUnits) {
      try {
        const parsed = JSON.parse(savedUnits)
        setUnits({ ...DEFAULT_UNITS, ...parsed })
      } catch (error) {
        console.error('Failed to parse saved units:', error)
      }
    }
  }, [])

  // Save units to localStorage whenever they change
  useEffect(() => {
    localStorage.setItem('fitness-tracker-units', JSON.stringify(units))
  }, [units])

  const setBodyWeightUnit = (unit: WeightUnit) => {
    setUnits(prev => ({ ...prev, bodyWeight: unit }))
  }

  const setExerciseWeightUnit = (unit: WeightUnit) => {
    setUnits(prev => ({ ...prev, exerciseWeight: unit }))
  }

  const setLengthUnit = (unit: LengthUnit) => {
    setUnits(prev => ({ ...prev, length: unit }))
  }

  const toggleBodyWeightUnit = () => {
    setUnits(prev => ({ 
      ...prev, 
      bodyWeight: prev.bodyWeight === 'lbs' ? 'kg' : 'lbs' 
    }))
  }

  const toggleExerciseWeightUnit = () => {
    setUnits(prev => ({ 
      ...prev, 
      exerciseWeight: prev.exerciseWeight === 'lbs' ? 'kg' : 'lbs' 
    }))
  }

  const toggleLengthUnit = () => {
    setUnits(prev => ({ 
      ...prev, 
      length: prev.length === 'inches' ? 'cm' : 'inches' 
    }))
  }

  return (
    <UnitContext.Provider value={{
      units,
      setBodyWeightUnit,
      setExerciseWeightUnit,
      setLengthUnit,
      toggleBodyWeightUnit,
      toggleExerciseWeightUnit,
      toggleLengthUnit
    }}>
      {children}
    </UnitContext.Provider>
  )
}

export function useUnits() {
  const context = useContext(UnitContext)
  if (context === undefined) {
    throw new Error('useUnits must be used within a UnitProvider')
  }
  return context
} 