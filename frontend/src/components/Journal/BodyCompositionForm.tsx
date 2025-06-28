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
    weight_pounds: 0,
    waist_inches: 0,
    notes: '',
  })
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState<string | null>(null)

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
      onSuccess()
    } catch (err) {
      setError('Failed to save body composition data')
      console.error('Error saving body composition:', err)
    } finally {
      setLoading(false)
    }
  }

  const handleInputChange = (field: keyof BodyCompositionCreate, value: number | string) => {
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
          <label htmlFor="weight_pounds" className="block text-sm font-medium text-gray-700 mb-1">
            Weight (lbs)
          </label>
          <input
            type="number"
            id="weight_pounds"
            step="0.1"
            min="0"
            value={formData.weight_pounds || ''}
            onChange={(e) => handleInputChange('weight_pounds', parseFloat(e.target.value) || 0)}
            className="input"
            placeholder="0.0"
            required
          />
        </div>

        <div>
          <label htmlFor="waist_inches" className="block text-sm font-medium text-gray-700 mb-1">
            Waist Circumference (inches)
          </label>
          <input
            type="number"
            id="waist_inches"
            step="0.1"
            min="0"
            value={formData.waist_inches || ''}
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