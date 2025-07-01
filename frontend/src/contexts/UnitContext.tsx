import React, { createContext, useContext, ReactNode } from 'react'
import { EXERCISE_WEIGHT_UNIT, BODY_WEIGHT_UNIT, LENGTH_UNIT, WeightUnit, LengthUnit } from '../utils/units'

interface UnitContextType {
  exerciseWeightUnit: WeightUnit
  bodyWeightUnit: WeightUnit
  lengthUnit: LengthUnit
}

const UnitContext = createContext<UnitContextType | undefined>(undefined)

export function UnitProvider({ children }: { children: ReactNode }) {
  const units = {
    exerciseWeightUnit: EXERCISE_WEIGHT_UNIT,
    bodyWeightUnit: BODY_WEIGHT_UNIT,
    lengthUnit: LENGTH_UNIT
  }

  return (
    <UnitContext.Provider value={units}>
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