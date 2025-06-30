import { useState, useEffect } from 'react'
import { Workout, WorkoutUpdate, WorkoutExercise, Exercise } from '../../types'
import { workoutApi, exerciseApi } from '../../api/client'
import { convertWeight, getWeightUnitSymbol, EXERCISE_WEIGHT_UNIT } from '../../utils/units'

interface WorkoutEditFormProps {
  workout: Workout
  onSuccess: () => void
  onCancel: () => void
}

interface EditableExercise {
  id: number
  exercise_id: number
  exercise_name: string
  sets_data: Array<{
    weight: number
    reps: number
    sets: number
  }>
  notes: string
  display_weight: number
}

function WorkoutEditForm({ workout, onSuccess, onCancel }: WorkoutEditFormProps) {
  const [exercises, setExercises] = useState<Exercise[]>([])
  const [editableExercises, setEditableExercises] = useState<EditableExercise[]>([])
  const [workoutNotes, setWorkoutNotes] = useState(workout.notes || '')
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState<string | null>(null)

  useEffect(() => {
    loadExercises()
    initializeEditableExercises()
  }, [workout])

  const loadExercises = async () => {
    try {
      const exerciseList = await exerciseApi.getAll()
      setExercises(exerciseList)
    } catch (err) {
      console.error('Error loading exercises:', err)
    }
  }

  const initializeEditableExercises = () => {
    const editable = workout.exercises.map(exercise => ({
      id: exercise.id,
      exercise_id: exercise.exercise_id,
      exercise_name: exercise.exercise_name,
      sets_data: exercise.sets_data.map(set => ({
        weight: set.weight,
        reps: set.reps,
        sets: set.sets
      })),
      notes: exercise.notes || '',
      display_weight: convertWeight(exercise.sets_data[0]?.weight || 0, 'lbs', EXERCISE_WEIGHT_UNIT)
    }))
    setEditableExercises(editable)
  }

  const updateExercise = (index: number, field: keyof EditableExercise, value: any) => {
    setEditableExercises(prev => 
      prev.map((exercise, i) => {
        if (i === index) {
          const updated = { ...exercise, [field]: value }
          // If updating display_weight, also update the backend weight
          if (field === 'display_weight') {
            updated.sets_data = updated.sets_data.map(set => ({
              ...set,
              weight: convertWeight(value as number, EXERCISE_WEIGHT_UNIT, 'lbs')
            }))
          }
          return updated
        }
        return exercise
      })
    )
  }

  const updateSet = (exerciseIndex: number, setIndex: number, field: string, value: any) => {
    setEditableExercises(prev => 
      prev.map((exercise, i) => {
        if (i === exerciseIndex) {
          const updatedSets = exercise.sets_data.map((set, j) => {
            if (j === setIndex) {
              return { ...set, [field]: value }
            }
            return set
          })
          return { ...exercise, sets_data: updatedSets }
        }
        return exercise
      })
    )
  }

  const removeExercise = (index: number) => {
    setEditableExercises(prev => prev.filter((_, i) => i !== index))
  }

  const addExercise = () => {
    const newExercise: EditableExercise = {
      id: Date.now(), // Temporary ID
      exercise_id: 0,
      exercise_name: '',
      sets_data: [{ weight: 0, reps: 0, sets: 1 }],
      notes: '',
      display_weight: 0
    }
    setEditableExercises(prev => [...prev, newExercise])
  }

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault()
    
    if (editableExercises.length === 0) {
      setError('At least one exercise is required')
      return
    }

    try {
      setLoading(true)
      setError(null)
      
      // For now, we can only update workout notes
      // Individual exercise editing would require backend support
      await workoutApi.update(workout.id, { notes: workoutNotes })
      
      onSuccess()
    } catch (err) {
      setError('Failed to update workout')
      console.error('Error updating workout:', err)
    } finally {
      setLoading(false)
    }
  }

  return (
    <form onSubmit={handleSubmit} className="space-y-6">
      {error && (
        <div className="bg-red-50 border border-red-200 rounded-md p-3">
          <p className="text-red-600 text-sm">{error}</p>
        </div>
      )}

      <div>
        <label htmlFor="workout-notes" className="block text-sm font-medium text-gray-700 mb-1">
          Workout Notes
        </label>
        <textarea
          id="workout-notes"
          value={workoutNotes}
          onChange={(e) => setWorkoutNotes(e.target.value)}
          className="input w-full"
          rows={3}
          placeholder="Add notes about this workout..."
        />
      </div>

      <div>
        <div className="flex items-center justify-between mb-4">
          <h3 className="text-lg font-medium text-gray-900">Exercises</h3>
          <button
            type="button"
            onClick={addExercise}
            className="btn btn-secondary text-sm"
          >
            + Add Exercise
          </button>
        </div>

        <div className="space-y-4">
          {editableExercises.map((exercise, exerciseIndex) => (
            <div key={exercise.id} className="bg-gray-50 rounded-lg p-4">
              <div className="flex items-center justify-between mb-3">
                <h4 className="text-sm font-medium text-gray-700">Exercise {exerciseIndex + 1}</h4>
                <button
                  type="button"
                  onClick={() => removeExercise(exerciseIndex)}
                  className="text-red-500 hover:text-red-700 text-sm"
                >
                  Remove
                </button>
              </div>

              <div className="grid grid-cols-1 md:grid-cols-3 gap-3 mb-3">
                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Exercise Name
                  </label>
                  <input
                    type="text"
                    value={exercise.exercise_name}
                    onChange={(e) => updateExercise(exerciseIndex, 'exercise_name', e.target.value)}
                    className="input"
                    placeholder="Exercise name..."
                  />
                </div>

                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Weight ({getWeightUnitSymbol(EXERCISE_WEIGHT_UNIT)})
                  </label>
                  <input
                    type="number"
                    step="0.5"
                    min="0"
                    value={exercise.display_weight || ''}
                    onChange={(e) => updateExercise(exerciseIndex, 'display_weight', parseFloat(e.target.value) || 0)}
                    className="input"
                    placeholder="0"
                  />
                </div>

                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Reps
                  </label>
                  <input
                    type="number"
                    min="1"
                    value={exercise.sets_data[0]?.reps || ''}
                    onChange={(e) => updateSet(exerciseIndex, 0, 'reps', parseInt(e.target.value) || 0)}
                    className="input"
                    placeholder="0"
                  />
                </div>
              </div>

              <div>
                <label className="block text-sm font-medium text-gray-700 mb-1">
                  Notes (Optional)
                </label>
                <textarea
                  value={exercise.notes}
                  onChange={(e) => updateExercise(exerciseIndex, 'notes', e.target.value)}
                  className="input w-full"
                  placeholder="Add notes about this exercise..."
                  rows={2}
                />
              </div>
            </div>
          ))}
        </div>
      </div>

      <div className="flex space-x-3">
        <button
          type="submit"
          disabled={loading}
          className="btn btn-primary flex-1"
        >
          {loading ? 'Updating...' : 'Update Workout'}
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

export default WorkoutEditForm 