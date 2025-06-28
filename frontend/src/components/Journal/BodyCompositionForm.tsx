import { useState } from 'react'
import { BodyCompositionCreate } from '../../types'
import { bodyCompositionApi } from '../../api/client'

interface BodyCompositionFormProps {
  date: string
  onSuccess: () => void
}

function BodyCompositionForm({ date, onSuccess }: BodyCompositionFormProps) {
  const [formData, setFormData] = useState<BodyCompositionCreate>({
    date,
    weight: 0,
    waist_circumference: 0,
  })
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState<string | null>(null)

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault()
    
    if (formData.weight <= 0 || formData.waist_circumference <= 0) {
      setError('Please enter valid measurements')
      return
    }

    try {
      setLoading(true)
      setError(null)
      await bodyCompositionApi.create(formData)
      onSuccess()
    } catch (err) {
      setError('Failed to save body composition data')
      console.error('Error saving body composition:', err)
    } finally {
      setLoading(false)
    }
  }

  const handleInputChange = (field: keyof BodyCompositionCreate, value: number) => {
    setFormData(prev => ({ ...prev, [field]: value }))
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
          <label htmlFor="weight" className="block text-sm font-medium text-gray-700 mb-1">
            Weight (lbs)
          </label>
          <input
            type="number"
            id="weight"
            step="0.1"
            min="0"
            value={formData.weight || ''}
            onChange={(e) => handleInputChange('weight', parseFloat(e.target.value) || 0)}
            className="input"
            placeholder="0.0"
            required
          />
        </div>

        <div>
          <label htmlFor="waist" className="block text-sm font-medium text-gray-700 mb-1">
            Waist Circumference (inches)
          </label>
          <input
            type="number"
            id="waist"
            step="0.1"
            min="0"
            value={formData.waist_circumference || ''}
            onChange={(e) => handleInputChange('waist_circumference', parseFloat(e.target.value) || 0)}
            className="input"
            placeholder="0.0"
            required
          />
        </div>
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