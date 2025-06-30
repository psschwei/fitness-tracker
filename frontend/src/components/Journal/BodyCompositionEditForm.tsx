import { useState, useEffect } from 'react'
import { BodyComposition, BodyCompositionUpdate } from '../../types'
import { bodyCompositionApi } from '../../api/client'
import { useUnits } from '../../contexts/UnitContext'
import { convertWeight, convertLength, getWeightUnitSymbol, getLengthUnitSymbol, WeightUnit, LengthUnit } from '../../utils/units'

interface BodyCompositionEditFormProps {
  measurement: BodyComposition
  onSuccess: () => void
  onCancel: () => void
}

function BodyCompositionEditForm({ measurement, onSuccess, onCancel }: BodyCompositionEditFormProps) {
  const { bodyWeightUnit, lengthUnit } = useUnits()
  const [formData, setFormData] = useState<BodyCompositionUpdate>({
    weight_pounds: measurement.weight_pounds,
    height_inches: measurement.height_inches,
    waist_inches: measurement.waist_inches,
    neck_inches: measurement.neck_inches,
    bmi: measurement.bmi,
    body_fat_percentage: measurement.body_fat_percentage,
    is_male: measurement.is_male,
    notes: measurement.notes || '',
  })
  const [displayWeight, setDisplayWeight] = useState(0)
  const [displayHeight, setDisplayHeight] = useState(0)
  const [displayWaist, setDisplayWaist] = useState(0)
  const [displayNeck, setDisplayNeck] = useState(0)
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState<string | null>(null)

  // Initialize display values from measurement
  useEffect(() => {
    const displayWeightValue = convertWeight(measurement.weight_pounds, 'lbs', bodyWeightUnit as WeightUnit)
    const displayHeightValue = measurement.height_inches ? convertLength(measurement.height_inches, 'inches', lengthUnit as LengthUnit) : 0
    const displayWaistValue = measurement.waist_inches ? convertLength(measurement.waist_inches, 'inches', lengthUnit as LengthUnit) : 0
    const displayNeckValue = measurement.neck_inches ? convertLength(measurement.neck_inches, 'inches', lengthUnit as LengthUnit) : 0
    
    setDisplayWeight(displayWeightValue)
    setDisplayHeight(displayHeightValue)
    setDisplayWaist(displayWaistValue)
    setDisplayNeck(displayNeckValue)
  }, [measurement, bodyWeightUnit, lengthUnit])

  // Convert display values to backend units when units change
  useEffect(() => {
    setFormData(prev => ({
      ...prev,
      weight_pounds: convertWeight(displayWeight, bodyWeightUnit as WeightUnit, 'lbs'),
      height_inches: displayHeight > 0 ? convertLength(displayHeight, lengthUnit as LengthUnit, 'inches') : null,
      waist_inches: displayWaist > 0 ? convertLength(displayWaist, lengthUnit as LengthUnit, 'inches') : null,
      neck_inches: displayNeck > 0 ? convertLength(displayNeck, lengthUnit as LengthUnit, 'inches') : null
    }))
  }, [bodyWeightUnit, lengthUnit, displayWeight, displayHeight, displayWaist, displayNeck])

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault()
    
    if (displayWeight <= 0) {
      setError('Please enter a valid weight')
      return
    }

    try {
      setLoading(true)
      setError(null)
      await bodyCompositionApi.update(measurement.id, formData)
      onSuccess()
    } catch (err) {
      setError('Failed to update body composition data')
      console.error('Error updating body composition:', err)
    } finally {
      setLoading(false)
    }
  }

  const handleInputChange = (field: keyof BodyCompositionUpdate, value: number | string) => {
    if (field === 'weight_pounds') {
      setDisplayWeight(value as number)
    } else if (field === 'height_inches') {
      setDisplayHeight(value as number)
    } else if (field === 'waist_inches') {
      setDisplayWaist(value as number)
    } else if (field === 'neck_inches') {
      setDisplayNeck(value as number)
    } else {
      setFormData(prev => ({ ...prev, [field]: value }))
    }
  }

  return (
    <form onSubmit={handleSubmit} className="space-y-4">
      {error && (
        <div className="bg-red-50 border border-red-200 rounded-md p-3">
          <p className="text-red-600 text-sm">{error}</p>
        </div>
      )}

      <div className="grid grid-cols-2 gap-4">
        <div>
          <label htmlFor="height_inches" className="block text-sm font-medium text-gray-700 mb-1">
            Height ({getLengthUnitSymbol(lengthUnit as LengthUnit)})
          </label>
          <input
            type="number"
            id="height_inches"
            step="0.1"
            min="0"
            value={displayHeight || ''}
            onChange={(e) => handleInputChange('height_inches', parseFloat(e.target.value) || 0)}
            className="input"
            placeholder="0.0"
          />
        </div>

        <div>
          <label htmlFor="neck_inches" className="block text-sm font-medium text-gray-700 mb-1">
            Neck Circumference ({getLengthUnitSymbol(lengthUnit as LengthUnit)})
          </label>
          <input
            type="number"
            id="neck_inches"
            step="0.1"
            min="0"
            value={displayNeck || ''}
            onChange={(e) => handleInputChange('neck_inches', parseFloat(e.target.value) || 0)}
            className="input"
            placeholder="0.0"
          />
        </div>
      </div>

      <div className="grid grid-cols-2 gap-4">
        <div>
          <label htmlFor="weight_pounds" className="block text-sm font-medium text-gray-700 mb-1">
            Weight ({getWeightUnitSymbol(bodyWeightUnit as WeightUnit)})
          </label>
          <input
            type="number"
            id="weight_pounds"
            step="0.1"
            min="0"
            value={displayWeight || ''}
            onChange={(e) => handleInputChange('weight_pounds', parseFloat(e.target.value) || 0)}
            className="input"
            placeholder="0.0"
            required
          />
        </div>

        <div>
          <label htmlFor="waist_inches" className="block text-sm font-medium text-gray-700 mb-1">
            Waist Circumference ({getLengthUnitSymbol(lengthUnit as LengthUnit)})
          </label>
          <input
            type="number"
            id="waist_inches"
            step="0.1"
            min="0"
            value={displayWaist || ''}
            onChange={(e) => handleInputChange('waist_inches', parseFloat(e.target.value) || 0)}
            className="input"
            placeholder="0.0"
          />
        </div>
      </div>

      <div>
        <label className="block text-sm font-medium text-gray-700 mb-1">
          Gender (for body fat calculation)
        </label>
        <div className="flex space-x-4">
          <label className="flex items-center">
            <input
              type="radio"
              name="gender"
              value="male"
              checked={formData.is_male}
              onChange={() => setFormData(prev => ({ ...prev, is_male: true }))}
              className="mr-2"
            />
            Male
          </label>
          <label className="flex items-center">
            <input
              type="radio"
              name="gender"
              value="female"
              checked={!formData.is_male}
              onChange={() => setFormData(prev => ({ ...prev, is_male: false }))}
              className="mr-2"
            />
            Female
          </label>
        </div>
      </div>

      <div>
        <label htmlFor="notes" className="block text-sm font-medium text-gray-700 mb-1">
          Notes (optional)
        </label>
        <input
          type="text"
          id="notes"
          value={formData.notes || ''}
          onChange={(e) => handleInputChange('notes', e.target.value)}
          className="input"
          placeholder="Any notes..."
        />
      </div>

      <div className="flex space-x-3">
        <button
          type="submit"
          disabled={loading}
          className="btn btn-primary flex-1"
        >
          {loading ? 'Updating...' : 'Update Body Composition'}
        </button>
        <button
          type="button"
          onClick={onCancel}
          disabled={loading}
          className="btn btn-secondary flex-1"
        >
          Cancel
        </button>
      </div>
    </form>
  )
}

export default BodyCompositionEditForm 