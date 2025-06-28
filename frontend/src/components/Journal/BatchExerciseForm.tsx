import { useState, useEffect } from 'react'
import { Exercise, WorkoutCreate } from '../../types'
import { exerciseApi, workoutApi } from '../../api/client'
import { useUnits } from '../../contexts/UnitContext'
import { convertWeight, getWeightUnitSymbol } from '../../utils/units'

interface ExerciseEntry {
  exercise_id: number
  weight: number
  reps_per_set: number
  display_weight: number
  notes: string
}

interface BatchExerciseFormProps {
  date: string
  onSuccess: () => void
}

function BatchExerciseForm({ date, onSuccess }: BatchExerciseFormProps) {
  const { units } = useUnits()
  const [exercises, setExercises] = useState<Exercise[]>([])
  const [exerciseEntries, setExerciseEntries] = useState<ExerciseEntry[]>([
    { exercise_id: 0, weight: 0, reps_per_set: 0, display_weight: 0, notes: '' }
  ])
  const [workoutNotes, setWorkoutNotes] = useState('')
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState<string | null>(null)

  useEffect(() => {
    loadExercises()
  }, [])

  // Convert display weights to backend units when units change
  useEffect(() => {
    setExerciseEntries(prev => 
      prev.map(entry => ({
        ...entry,
        weight: convertWeight(entry.display_weight, units.exerciseWeight, 'lbs')
      }))
    )
  }, [units.exerciseWeight])

  const loadExercises = async () => {
    try {
      const exerciseList = await exerciseApi.getAll()
      setExercises(exerciseList)
    } catch (err) {
      console.error('Error loading exercises:', err)
    }
  }

  const addExerciseEntry = () => {
    setExerciseEntries(prev => [...prev, { exercise_id: 0, weight: 0, reps_per_set: 0, display_weight: 0, notes: '' }])
  }

  const removeExerciseEntry = (index: number) => {
    setExerciseEntries(prev => prev.filter((_, i) => i !== index))
  }

  const updateExerciseEntry = (index: number, field: keyof ExerciseEntry, value: number | string) => {
    setExerciseEntries(prev => 
      prev.map((entry, i) => {
        if (i === index) {
          const updated = { ...entry, [field]: value }
          // If updating display_weight, also update the backend weight
          if (field === 'display_weight') {
            updated.weight = convertWeight(value as number, units.exerciseWeight, 'lbs')
          }
          return updated
        }
        return entry
      })
    )
  }

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault()
    
    const validEntries = exerciseEntries.filter(
      entry => entry.exercise_id > 0 && entry.weight > 0 && entry.reps_per_set > 0
    )

    if (validEntries.length === 0) {
      setError('Please add at least one valid exercise')
      return
    }

    try {
      setLoading(true)
      setError(null)
      
      const workoutData: WorkoutCreate = {
        date,
        notes: workoutNotes || undefined,
        exercises: validEntries.map(entry => ({
          exercise_id: entry.exercise_id,
          weight: entry.weight,
          reps_per_set: entry.reps_per_set,
          notes: entry.notes
        })),
      }
      
      await workoutApi.create(workoutData)
      
      // Reset form
      setExerciseEntries([{ exercise_id: 0, weight: 0, reps_per_set: 0, display_weight: 0, notes: '' }])
      setWorkoutNotes('')
      
      onSuccess()
    } catch (err) {
      setError('Failed to save exercises')
      console.error('Error saving exercises:', err)
    } finally {
      setLoading(false)
    }
  }

  return (
    <form onSubmit={handleSubmit} className="space-y-4">
      {error && (
        <div className="bg-red-50 border border-red-200 rounded-md p-3">
          <p className="text-red-600 text-sm">{error}</p>
        </div>
      )}

      <div className="space-y-4">
        {exerciseEntries.map((entry, index) => (
          <div key={index} className="bg-gray-50 rounded-lg p-4">
            <div className="flex items-center justify-between mb-3">
              <h3 className="text-sm font-medium text-gray-700">Exercise {index + 1}</h3>
              {exerciseEntries.length > 1 && (
                <button
                  type="button"
                  onClick={() => removeExerciseEntry(index)}
                  className="text-red-500 hover:text-red-700 text-sm"
                >
                  Remove
                </button>
              )}
            </div>

            <div className="grid grid-cols-1 md:grid-cols-3 gap-3">
              <div>
                <label className="block text-sm font-medium text-gray-700 mb-1">
                  Exercise
                </label>
                <select
                  value={entry.exercise_id || ''}
                  onChange={(e) => updateExerciseEntry(index, 'exercise_id', parseInt(e.target.value) || 0)}
                  className="input"
                  required
                >
                  <option value="">Select exercise</option>
                  {exercises.map((exercise) => (
                    <option key={exercise.id} value={exercise.id}>
                      {exercise.name}
                    </option>
                  ))}
                </select>
              </div>

              <div>
                <label className="block text-sm font-medium text-gray-700 mb-1">
                  Weight ({getWeightUnitSymbol(units.exerciseWeight)})
                </label>
                <input
                  type="number"
                  step="0.5"
                  min="0"
                  value={entry.display_weight || ''}
                  onChange={(e) => updateExerciseEntry(index, 'display_weight', parseFloat(e.target.value) || 0)}
                  className="input"
                  placeholder="0"
                  required
                />
              </div>

              <div>
                <label className="block text-sm font-medium text-gray-700 mb-1">
                  Reps per Set
                </label>
                <input
                  type="number"
                  min="1"
                  value={entry.reps_per_set || ''}
                  onChange={(e) => updateExerciseEntry(index, 'reps_per_set', parseInt(e.target.value) || 0)}
                  className="input"
                  placeholder="0"
                  required
                />
              </div>
            </div>

            <div className="mt-3">
              <label className="block text-sm font-medium text-gray-700 mb-1">
                Notes (Optional)
              </label>
              <textarea
                value={entry.notes || ''}
                onChange={(e) => updateExerciseEntry(index, 'notes', e.target.value)}
                className="input w-full"
                placeholder="Add any notes about this exercise..."
                rows={2}
              />
            </div>
          </div>
        ))}
      </div>

      <button
        type="button"
        onClick={addExerciseEntry}
        className="btn btn-secondary w-full"
      >
        + Add Another Exercise
      </button>

      <div>
        <label htmlFor="workout-notes" className="block text-sm font-medium text-gray-700 mb-1">
          Workout Notes (Optional)
        </label>
        <textarea
          id="workout-notes"
          value={workoutNotes}
          onChange={(e) => setWorkoutNotes(e.target.value)}
          className="input w-full"
          placeholder="Add any notes about this workout session..."
          rows={3}
        />
      </div>

      <button
        type="submit"
        disabled={loading}
        className="btn btn-primary w-full"
      >
        {loading ? 'Saving...' : 'Save All Exercises'}
      </button>
    </form>
  )
}

export default BatchExerciseForm 