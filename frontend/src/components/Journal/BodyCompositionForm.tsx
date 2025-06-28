import { useState, useEffect } from 'react'
import { BodyCompositionCreate, BodyComposition } from '../../types'
import { bodyCompositionApi } from '../../api/client'
import { useUnits } from '../../contexts/UnitContext'
import { convertWeight, convertLength, getWeightUnitSymbol, getLengthUnitSymbol } from '../../utils/units'

interface BodyCompositionFormProps {
  date: string
  onSuccess: () => void
}

function BodyCompositionForm({ date, onSuccess }: BodyCompositionFormProps) {
  const { units } = useUnits()
  const [formData, setFormData] = useState<BodyCompositionCreate>({
    date,
    weight_pounds: 0,
    waist_inches: 0,
    notes: '',
  })
  const [displayWeight, setDisplayWeight] = useState(0)
  const [displayWaist, setDisplayWaist] = useState(0)
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState<string | null>(null)

  // Load latest body composition data to pre-populate form
  useEffect(() => {
    loadLatestData()
  }, [units.bodyWeight, units.length])

  const loadLatestData = async () => {
    try {
      const latestData = await bodyCompositionApi.getLatest()
      
      // Convert backend units to display units
      const displayWeightValue = convertWeight(latestData.weight_pounds, 'lbs', units.bodyWeight)
      const displayWaistValue = latestData.waist_inches ? convertLength(latestData.waist_inches, 'inches', units.length) : 0
      
      setDisplayWeight(displayWeightValue)
      setDisplayWaist(displayWaistValue)
      
      setFormData(prev => ({
        ...prev,
        weight_pounds: latestData.weight_pounds,
        waist_inches: latestData.waist_inches || 0,
        notes: latestData.notes || ''
      }))
    } catch (err) {
      // If no previous data exists, that's fine - form will start empty
      console.log('No previous body composition data found')
    }
  }

  // Convert display values to backend units when units change
  useEffect(() => {
    setFormData(prev => ({
      ...prev,
      weight_pounds: convertWeight(displayWeight, units.bodyWeight, 'lbs'),
      waist_inches: displayWaist > 0 ? convertLength(displayWaist, units.length, 'inches') : 0
    }))
  }, [units.bodyWeight, units.length, displayWeight, displayWaist])

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault()
    
    if (formData.weight_pounds <= 0 || (formData.waist_inches !== null && formData.waist_inches <= 0)) {
      setError('Please enter valid measurements')
      return
    }

    try {
      setLoading(true)
      setError(null)
      await bodyCompositionApi.create(formData)
      
      // Reset form
      setDisplayWeight(0)
      setDisplayWaist(0)
      setFormData({
        date,
        weight_pounds: 0,
        waist_inches: 0,
        notes: '',
      })
      
      onSuccess()
    } catch (err) {
      setError('Failed to save body composition data')
      console.error('Error saving body composition:', err)
    } finally {
      setLoading(false)
    }
  }

  const handleInputChange = (field: keyof BodyCompositionCreate, value: number | string) => {
    if (field === 'weight_pounds') {
      setDisplayWeight(value as number)
    } else if (field === 'waist_inches') {
      setDisplayWaist(value as number)
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
          <label htmlFor="weight_pounds" className="block text-sm font-medium text-gray-700 mb-1">
            Weight ({getWeightUnitSymbol(units.bodyWeight)})
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
            Waist Circumference ({getLengthUnitSymbol(units.length)})
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
            required
          />
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

      <button
        type="submit"
        disabled={loading}
        className="btn btn-primary w-full"
      >
        {loading ? 'Saving...' : 'Save Body Composition'}
      </button>
    </form>
  )
}

export default BodyCompositionForm 