import { useState, useEffect } from 'react'
import { Exercise, WorkoutCreate, WorkoutExerciseCreate } from '../../types'
import { exerciseApi, workoutApi } from '../../api/client'

interface ExerciseFormProps {
  date: string
  onSuccess: () => void
}

function ExerciseForm({ date, onSuccess }: ExerciseFormProps) {
  const [exercises, setExercises] = useState<Exercise[]>([])
  const [formData, setFormData] = useState<WorkoutExerciseCreate>({
    exercise_id: 0,
    weight: 0,
    reps_per_set: 0,
  })
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState<string | null>(null)

  useEffect(() => {
    loadExercises()
  }, [])

  const loadExercises = async () => {
    try {
      const exerciseList = await exerciseApi.getAll()
      setExercises(exerciseList)
    } catch (err) {
      console.error('Error loading exercises:', err)
    }
  }

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault()
    
    if (formData.exercise_id === 0 || formData.weight <= 0 || formData.reps_per_set <= 0) {
      setError('Please fill in all fields with valid values')
      return
    }

    try {
      setLoading(true)
      setError(null)
      
      const workoutData: WorkoutCreate = {
        date,
        exercises: [formData],
      }
      
      await workoutApi.create(workoutData)
      onSuccess()
    } catch (err) {
      setError('Failed to save exercise')
      console.error('Error saving exercise:', err)
    } finally {
      setLoading(false)
    }
  }

  const handleInputChange = (field: keyof WorkoutExerciseCreate, value: number) => {
    setFormData(prev => ({ ...prev, [field]: value }))
  }

  return (
    <form onSubmit={handleSubmit} className="space-y-4">
      {error && (
        <div className="bg-red-50 border border-red-200 rounded-md p-3">
          <p className="text-red-600 text-sm">{error}</p>
        </div>
      )}

      <div>
        <label htmlFor="exercise" className="block text-sm font-medium text-gray-700 mb-1">
          Exercise
        </label>
        <select
          id="exercise"
          value={formData.exercise_id || ''}
          onChange={(e) => handleInputChange('exercise_id', parseInt(e.target.value) || 0)}
          className="input"
          required
        >
          <option value="">Select an exercise</option>
          {exercises.map((exercise) => (
            <option key={exercise.id} value={exercise.id}>
              {exercise.name}
            </option>
          ))}
        </select>
      </div>

      <div className="grid grid-cols-2 gap-4">
        <div>
          <label htmlFor="weight" className="block text-sm font-medium text-gray-700 mb-1">
            Weight (lbs)
          </label>
          <input
            type="number"
            id="weight"
            step="0.5"
            min="0"
            value={formData.weight || ''}
            onChange={(e) => handleInputChange('weight', parseFloat(e.target.value) || 0)}
            className="input"
            placeholder="0"
            required
          />
        </div>

        <div>
          <label htmlFor="reps" className="block text-sm font-medium text-gray-700 mb-1">
            Reps per Set
          </label>
          <input
            type="number"
            id="reps"
            min="1"
            value={formData.reps_per_set || ''}
            onChange={(e) => handleInputChange('reps_per_set', parseInt(e.target.value) || 0)}
            className="input"
            placeholder="0"
            required
          />
        </div>
      </div>

      <button
        type="submit"
        disabled={loading}
        className="btn btn-primary w-full"
      >
        {loading ? 'Saving...' : 'Add Exercise'}
      </button>
    </form>
  )
}

export default ExerciseForm 