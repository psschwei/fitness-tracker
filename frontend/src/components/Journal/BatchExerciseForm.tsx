import { useState, useEffect } from 'react'
import { Exercise, WorkoutCreate, WorkoutExerciseCreate } from '../../types'
import { exerciseApi, workoutApi } from '../../api/client'
import { useUnits } from '../../contexts/UnitContext'
import { convertWeight, getWeightUnitSymbol } from '../../utils/units'

interface ExerciseEntry {
  id?: string // Local ID for tracking saved state
  exercise_id: number
  exercise_name?: string // For new exercises
  weight: number
  reps_per_set: number
  sets: number // Number of sets to perform
  display_weight: number
  notes: string
  isSaved: boolean // Track if this exercise has been saved
  savedId?: number // ID of the saved workout exercise
}

interface BatchExerciseFormProps {
  date: string
  onSuccess: () => void
}

function BatchExerciseForm({ date, onSuccess }: BatchExerciseFormProps) {
  const { units } = useUnits()
  const [exercises, setExercises] = useState<Exercise[]>([])
  const [exerciseEntries, setExerciseEntries] = useState<ExerciseEntry[]>([
    { 
      id: '1',
      exercise_id: 0, 
      weight: 0, 
      reps_per_set: 0, 
      sets: 1,
      display_weight: 0, 
      notes: '',
      isSaved: false
    }
  ])
  const [workoutNotes, setWorkoutNotes] = useState('')
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState<string | null>(null)
  const [successMessage, setSuccessMessage] = useState<string | null>(null)

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
    const newId = Date.now().toString()
    setExerciseEntries(prev => [...prev, { 
      id: newId,
      exercise_id: 0, 
      weight: 0, 
      reps_per_set: 0, 
      sets: 1,
      display_weight: 0, 
      notes: '',
      isSaved: false
    }])
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
          // If updating exercise_id, mark as not saved since it changed
          if (field === 'exercise_id') {
            updated.isSaved = false
            updated.savedId = undefined
          }
          return updated
        }
        return entry
      })
    )
  }

  const saveIndividualExercise = async (index: number) => {
    const entry = exerciseEntries[index]
    
    if ((!entry.exercise_id && !entry.exercise_name) || entry.weight <= 0 || entry.reps_per_set <= 0 || entry.sets <= 0) {
      setError('Please fill in all fields with valid values')
      return
    }

    try {
      setLoading(true)
      setError(null)
      setSuccessMessage(null)

      // Create or find exercise
      let exerciseId = entry.exercise_id
      if (entry.exercise_name && !exercises.find(e => e.id === exerciseId)) {
        // Create new exercise
        const newExercise = await exerciseApi.create({
          name: entry.exercise_name,
          category: 'strength' // Default category for new exercises
        })
        exerciseId = newExercise.id
        // Reload exercises to include the new one
        await loadExercises()
      }

      // Create workout with single exercise
      const workoutData: WorkoutCreate = {
        date,
        notes: workoutNotes || undefined,
        exercises: [{
          exercise_id: exerciseId,
          sets_data: [{
            weight: entry.weight,
            reps: entry.reps_per_set,
            sets: entry.sets
          }],
          notes: entry.notes
        }],
      }
      
      const savedWorkout = await workoutApi.create(workoutData)
      
      // Mark this exercise as saved
      setExerciseEntries(prev => 
        prev.map((e, i) => {
          if (i === index) {
            return {
              ...e,
              isSaved: true,
              savedId: savedWorkout.exercises[0]?.id
            }
          }
          return e
        })
      )
      
      setSuccessMessage(`Exercise saved to ${date}`)
      setTimeout(() => setSuccessMessage(null), 3000)
      
      onSuccess()
    } catch (err) {
      setError('Failed to save exercise')
      console.error('Error saving exercise:', err)
    } finally {
      setLoading(false)
    }
  }

  const saveAllExercises = async () => {
    const validEntries = exerciseEntries.filter(
      entry => (entry.exercise_id > 0 || entry.exercise_name) && entry.weight > 0 && entry.reps_per_set > 0 && entry.sets > 0
    )

    if (validEntries.length === 0) {
      setError('Please add at least one valid exercise')
      return
    }

    try {
      setLoading(true)
      setError(null)
      setSuccessMessage(null)

      // Process each exercise
      const exercisesToSave: WorkoutExerciseCreate[] = []
      
      for (const entry of validEntries) {
        // Skip if already saved (to avoid duplicates)
        if (entry.isSaved) {
          continue
        }

        // Create or find exercise
        let exerciseId = entry.exercise_id
        if (entry.exercise_name && !exercises.find(e => e.id === exerciseId)) {
          const newExercise = await exerciseApi.create({
            name: entry.exercise_name,
            category: 'strength' // Default category for new exercises
          })
          exerciseId = newExercise.id
        }

        exercisesToSave.push({
          exercise_id: exerciseId,
          sets_data: [{
            weight: entry.weight,
            reps: entry.reps_per_set,
            sets: entry.sets
          }],
          notes: entry.notes
        })
      }

      if (exercisesToSave.length === 0) {
        setSuccessMessage('All exercises already saved')
        setTimeout(() => setSuccessMessage(null), 3000)
        return
      }

      // Create workout with all new exercises
      const workoutData: WorkoutCreate = {
        date,
        notes: workoutNotes || undefined,
        exercises: exercisesToSave,
      }
      
      await workoutApi.create(workoutData)
      
      // Mark all exercises as saved
      setExerciseEntries(prev => 
        prev.map(entry => ({
          ...entry,
          isSaved: true
        }))
      )
      
      setSuccessMessage(`${exercisesToSave.length} exercise(s) saved to ${date}`)
      setTimeout(() => setSuccessMessage(null), 3000)
      
      onSuccess()
    } catch (err) {
      setError('Failed to save exercises')
      console.error('Error saving exercises:', err)
    } finally {
      setLoading(false)
    }
  }

  const handleExerciseNameChange = (index: number, value: string) => {
    setExerciseEntries(prev => 
      prev.map((entry, i) => {
        if (i === index) {
          // Check if this matches an existing exercise
          const existingExercise = exercises.find(e => 
            e.name.toLowerCase() === value.toLowerCase()
          )
          
          return {
            ...entry,
            exercise_name: value,
            exercise_id: existingExercise?.id || 0,
            isSaved: false, // Mark as not saved since it changed
            savedId: undefined
          }
        }
        return entry
      })
    )
  }

  return (
    <div className="space-y-4">
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

      <div className="space-y-4">
        {exerciseEntries.map((entry, index) => (
          <div key={entry.id} className="bg-gray-50 rounded-lg p-4">
            <div className="flex items-center justify-between mb-3">
              <h3 className="text-sm font-medium text-gray-700">Exercise {index + 1}</h3>
              <div className="flex items-center space-x-2">
                {entry.isSaved && (
                  <span className="text-green-600 text-sm font-medium">✓ Saved</span>
                )}
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
            </div>

            <div className="grid grid-cols-1 md:grid-cols-4 gap-3">
              <div>
                <label className="block text-sm font-medium text-gray-700 mb-1">
                  Exercise
                </label>
                <input
                  type="text"
                  value={entry.exercise_name || exercises.find(e => e.id === entry.exercise_id)?.name || ''}
                  onChange={(e) => handleExerciseNameChange(index, e.target.value)}
                  className="input"
                  placeholder="Type exercise name..."
                  required
                />
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

            <div className="mt-3 flex justify-end">
              <button
                type="button"
                onClick={() => saveIndividualExercise(index)}
                disabled={loading || entry.isSaved}
                className={`btn text-sm ${
                  entry.isSaved 
                    ? 'bg-gray-300 text-gray-500 cursor-not-allowed' 
                    : 'btn-primary'
                }`}
              >
                {entry.isSaved ? 'Saved ✓' : 'Save Exercise'}
              </button>
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
        type="button"
        onClick={saveAllExercises}
        disabled={loading}
        className="btn btn-primary w-full"
      >
        {loading ? 'Saving...' : 'Save All Unsaved Exercises'}
      </button>
    </div>
  )
}

export default BatchExerciseForm 