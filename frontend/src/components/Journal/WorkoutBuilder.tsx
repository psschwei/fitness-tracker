import { useState, useEffect } from 'react'
import { Workout, WorkoutCreate, WorkoutExerciseCreate, Exercise } from '../../types'
import { workoutApi, exerciseApi } from '../../api/client'
import { getWeightUnitSymbol, EXERCISE_WEIGHT_UNIT } from '../../utils/units'

interface WorkoutBuilderProps {
  date: string
  onSuccess: () => void
}

interface ExerciseEntry {
  exercise_id: number
  exercise_name: string
  weight: number
  reps: number
  sets: number
  display_weight: number
  notes: string
}

function WorkoutBuilder({ date, onSuccess }: WorkoutBuilderProps) {
  const [exercises, setExercises] = useState<Exercise[]>([])
  const [currentWorkout, setCurrentWorkout] = useState<Workout | null>(null)
  const [exerciseEntries, setExerciseEntries] = useState<ExerciseEntry[]>([])
  const [workoutNotes, setWorkoutNotes] = useState('')
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState<string | null>(null)
  const [successMessage, setSuccessMessage] = useState<string | null>(null)

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

  const createWorkout = async () => {
    try {
      setLoading(true)
      setError(null)
      
      const workoutData: WorkoutCreate = {
        date,
        exercises: [] // Start with no exercises
      }
      
      const newWorkout = await workoutApi.create(workoutData)
      setCurrentWorkout(newWorkout)
      setWorkoutNotes(newWorkout.notes || '')
      setSuccessMessage('Workout created! Now add exercises.')
      
      // Clear success message after 3 seconds
      setTimeout(() => setSuccessMessage(null), 3000)
    } catch (err) {
      setError('Failed to create workout')
      console.error('Error creating workout:', err)
    } finally {
      setLoading(false)
    }
  }

  const addExerciseToWorkout = async () => {
    if (!currentWorkout) {
      setError('Please create a workout first')
      return
    }

    if (exerciseEntries.length === 0) {
      setError('Please add at least one exercise')
      return
    }

    try {
      setLoading(true)
      setError(null)

      // Add each exercise to the workout
      for (const entry of exerciseEntries) {
        const exerciseData: WorkoutExerciseCreate = {
          exercise_id: entry.exercise_id,
          sets_data: [{
            weight: entry.display_weight,
            reps: entry.reps,
            sets: entry.sets
          }],
          notes: entry.notes
        }
        
        await workoutApi.addExercise(currentWorkout.id, exerciseData)
      }

      setSuccessMessage('Exercises added to workout!')
      setExerciseEntries([])
      onSuccess()
      
      // Clear success message after 3 seconds
      setTimeout(() => setSuccessMessage(null), 3000)
    } catch (err) {
      setError('Failed to add exercises to workout')
      console.error('Error adding exercises:', err)
    } finally {
      setLoading(false)
    }
  }

  const updateWorkoutNotes = async () => {
    if (!currentWorkout) {
      setError('No workout to update')
      return
    }

    try {
      setLoading(true)
      setError(null)
      
      await workoutApi.update(currentWorkout.id, { notes: workoutNotes.trim() || undefined })
      setSuccessMessage('Workout notes updated!')
      
      // Clear success message after 3 seconds
      setTimeout(() => setSuccessMessage(null), 3000)
    } catch (err) {
      setError('Failed to update workout notes')
      console.error('Error updating workout notes:', err)
    } finally {
      setLoading(false)
    }
  }

  const completeWorkout = async () => {
    if (!currentWorkout) {
      setError('No workout to complete')
      return
    }

    try {
      setLoading(true)
      setError(null)
      
      await workoutApi.completeWorkout(currentWorkout.id)
      setSuccessMessage('Workout completed!')
      setCurrentWorkout(null)
      setExerciseEntries([])
      setWorkoutNotes('')
      onSuccess()
      
      // Clear success message after 3 seconds
      setTimeout(() => setSuccessMessage(null), 3000)
    } catch (err) {
      setError('Failed to complete workout')
      console.error('Error completing workout:', err)
    } finally {
      setLoading(false)
    }
  }

  const cancelWorkout = async () => {
    if (!currentWorkout) {
      setError('No workout to cancel')
      return
    }

    try {
      setLoading(true)
      setError(null)
      
      await workoutApi.delete(currentWorkout.id)
      setSuccessMessage('Workout cancelled and deleted.')
      setCurrentWorkout(null)
      setExerciseEntries([])
      setWorkoutNotes('')
      onSuccess()
      
      // Clear success message after 3 seconds
      setTimeout(() => setSuccessMessage(null), 3000)
    } catch (err) {
      setError('Failed to cancel workout')
      console.error('Error cancelling workout:', err)
    } finally {
      setLoading(false)
    }
  }

  const addExerciseEntry = () => {
    setExerciseEntries(prev => [...prev, {
      exercise_id: 0,
      exercise_name: '',
      weight: 0,
      reps: 0,
      sets: 1,
      display_weight: 0,
      notes: ''
    }])
  }

  const removeExerciseEntry = (index: number) => {
    setExerciseEntries(prev => prev.filter((_, i) => i !== index))
  }

  const updateExerciseEntry = (index: number, field: keyof ExerciseEntry, value: any) => {
    setExerciseEntries(prev => 
      prev.map((entry, i) => {
        if (i === index) {
          const updated = { ...entry, [field]: value }
          // If updating display_weight, also update the backend weight (now both in kg)
          if (field === 'display_weight') {
            updated.weight = value as number
          }
          return updated
        }
        return entry
      })
    )
  }

  const handleExerciseNameChange = (index: number, exerciseName: string) => {
    // Find exercise by name
    const exercise = exercises.find(e => e.name.toLowerCase().includes(exerciseName.toLowerCase()))
    
    setExerciseEntries(prev => 
      prev.map((entry, i) => {
        if (i === index) {
          return {
            ...entry,
            exercise_name: exerciseName,
            exercise_id: exercise?.id || 0
          }
        }
        return entry
      })
    )
  }

  return (
    <div className="space-y-6">
      {error && (
        <div className="bg-red-50 border border-red-200 rounded-md p-3">
          <p className="text-red-600 text-sm">{error}</p>
        </div>
      )}

      {successMessage && (
        <div className="bg-green-50 border border-green-200 rounded-md p-3">
          <p className="text-green-600 text-sm">{successMessage}</p>
        </div>
      )}

      {/* Workout Creation */}
      {!currentWorkout ? (
        <div className="card">
          <h3 className="text-lg font-semibold text-gray-900 mb-4">Create New Workout</h3>
          
          <div className="space-y-4">
            <button
              onClick={createWorkout}
              disabled={loading}
              className="btn btn-primary w-full"
            >
              {loading ? 'Creating...' : 'Create Workout'}
            </button>
          </div>
        </div>
      ) : (
        /* Exercise Addition */
        <div className="card">
          <div className="flex items-center justify-between mb-4">
            <h3 className="text-lg font-semibold text-gray-900">
              Add Exercises to Workout
            </h3>
            <div className="flex space-x-2">
              <button
                onClick={cancelWorkout}
                disabled={loading}
                className="btn btn-danger text-sm"
              >
                {loading ? 'Cancelling...' : 'Cancel Workout'}
              </button>
              <button
                onClick={completeWorkout}
                disabled={loading}
                className="btn btn-success text-sm"
              >
                {loading ? 'Completing...' : 'Complete Workout'}
              </button>
            </div>
          </div>

          <div className="mb-4 p-3 bg-blue-50 rounded border-l-4 border-blue-400">
            <p className="text-sm text-blue-800">
              <strong>Workout:</strong> {currentWorkout.notes || 'No notes added yet'}
            </p>
          </div>

          <div className="mb-4">
            <label htmlFor="workout-notes" className="block text-sm font-medium text-gray-700 mb-1">
              Workout Notes (Optional)
            </label>
            <div className="flex space-x-2">
              <textarea
                id="workout-notes"
                value={workoutNotes}
                onChange={(e) => setWorkoutNotes(e.target.value)}
                className="input flex-1"
                rows={3}
                placeholder="Add notes about this workout..."
              />
              <button
                onClick={updateWorkoutNotes}
                disabled={loading}
                className="btn btn-secondary whitespace-nowrap"
              >
                {loading ? 'Updating...' : 'Update Notes'}
              </button>
            </div>
          </div>

          <div className="space-y-4">
            {exerciseEntries.map((entry, index) => (
              <div key={index} className="bg-gray-50 rounded-lg p-4">
                <div className="flex items-center justify-between mb-3">
                  <h4 className="text-sm font-medium text-gray-700">Exercise {index + 1}</h4>
                  <button
                    onClick={() => removeExerciseEntry(index)}
                    className="text-red-500 hover:text-red-700 text-sm"
                  >
                    Remove
                  </button>
                </div>

                <div className="grid grid-cols-1 md:grid-cols-4 gap-3">
                  <div>
                    <label className="block text-sm font-medium text-gray-700 mb-1">
                      Exercise
                    </label>
                    <input
                      type="text"
                      value={entry.exercise_name}
                      onChange={(e) => handleExerciseNameChange(index, e.target.value)}
                      className="input"
                      placeholder="Type exercise name..."
                      required
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
                      value={entry.display_weight || ''}
                      onChange={(e) => updateExerciseEntry(index, 'display_weight', parseFloat(e.target.value) || 0)}
                      className="input"
                      placeholder="0"
                      required
                    />
                  </div>

                  <div>
                    <label className="block text-sm font-medium text-gray-700 mb-1">
                      Reps
                    </label>
                    <input
                      type="number"
                      min="1"
                      value={entry.reps || ''}
                      onChange={(e) => updateExerciseEntry(index, 'reps', parseInt(e.target.value) || 0)}
                      className="input"
                      placeholder="0"
                      required
                    />
                  </div>

                  <div>
                    <label className="block text-sm font-medium text-gray-700 mb-1">
                      Sets
                    </label>
                    <input
                      type="number"
                      min="1"
                      value={entry.sets || ''}
                      onChange={(e) => updateExerciseEntry(index, 'sets', parseInt(e.target.value) || 0)}
                      className="input"
                      placeholder="1"
                      required
                    />
                  </div>
                </div>

                <div className="mt-3">
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Notes (Optional)
                  </label>
                  <textarea
                    value={entry.notes}
                    onChange={(e) => updateExerciseEntry(index, 'notes', e.target.value)}
                    className="input w-full"
                    placeholder="Add any notes about this exercise..."
                    rows={2}
                  />
                </div>
              </div>
            ))}

            <button
              type="button"
              onClick={addExerciseEntry}
              className="btn btn-secondary w-full"
            >
              + Add Exercise
            </button>

            {exerciseEntries.length > 0 && (
              <button
                onClick={addExerciseToWorkout}
                disabled={loading}
                className="btn btn-primary w-full"
              >
                {loading ? 'Adding Exercises...' : 'Add Exercises to Workout'}
              </button>
            )}
          </div>
        </div>
      )}
    </div>
  )
}

export default WorkoutBuilder 