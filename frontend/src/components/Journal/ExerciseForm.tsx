import { useState, useEffect } from 'react'
import { Exercise, WorkoutCreate, WorkoutExerciseCreate } from '../../types'
import { exerciseApi, workoutApi } from '../../api/client'
import { useUnits } from '../../contexts/UnitContext'
import { convertWeight, getWeightUnitSymbol } from '../../utils/units'

interface ExerciseFormProps {
  date: string
  onSuccess: () => void
}

function ExerciseForm({ date, onSuccess }: ExerciseFormProps) {
  const { units } = useUnits()
  const [exercises, setExercises] = useState<Exercise[]>([])
  const [exerciseInput, setExerciseInput] = useState('')
  const [showDropdown, setShowDropdown] = useState(false)
  const [selectedExerciseId, setSelectedExerciseId] = useState<number | null>(null)
  const [formData, setFormData] = useState<WorkoutExerciseCreate>({
    exercise_id: 0,
    weight: 0,
    reps_per_set: 0,
  })
  const [displayWeight, setDisplayWeight] = useState(0)
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState<string | null>(null)

  useEffect(() => {
    loadExercises()
  }, [])

  // Convert display weight to backend units when units change
  useEffect(() => {
    setFormData(prev => ({
      ...prev,
      weight: convertWeight(displayWeight, units.exerciseWeight, 'lbs')
    }))
  }, [units.exerciseWeight, displayWeight])

  const loadExercises = async () => {
    try {
      const exerciseList = await exerciseApi.getAll()
      setExercises(exerciseList)
    } catch (err) {
      console.error('Error loading exercises:', err)
    }
  }

  // Filter exercises based on input
  const filteredExercises = exercises.filter(exercise =>
    exercise.name.toLowerCase().includes(exerciseInput.toLowerCase())
  )

  // Check if input matches an existing exercise exactly
  const exactMatch = exercises.find(exercise =>
    exercise.name.toLowerCase() === exerciseInput.toLowerCase()
  )

  const handleExerciseSelect = (exercise: Exercise | null) => {
    if (exercise) {
      setExerciseInput(exercise.name)
      setSelectedExerciseId(exercise.id)
      setFormData(prev => ({ ...prev, exercise_id: exercise.id }))
    } else {
      // Creating new exercise
      setSelectedExerciseId(null)
      setFormData(prev => ({ ...prev, exercise_id: 0 }))
    }
    setShowDropdown(false)
  }

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault()
    
    if (!exerciseInput.trim() || formData.weight <= 0 || formData.reps_per_set <= 0) {
      setError('Please fill in all fields with valid values')
      return
    }

    try {
      setLoading(true)
      setError(null)
      
      let exerciseId = selectedExerciseId
      
      // If no exercise is selected, create a new one
      if (!exerciseId) {
        const newExercise = await exerciseApi.create({
          name: exerciseInput.trim()
        })
        exerciseId = newExercise.id
      }
      
      const workoutData: WorkoutCreate = {
        date,
        exercises: [{
          ...formData,
          exercise_id: exerciseId
        }],
      }
      
      await workoutApi.create(workoutData)
      
      // Reset form
      setExerciseInput('')
      setSelectedExerciseId(null)
      setDisplayWeight(0)
      setFormData({
        exercise_id: 0,
        weight: 0,
        reps_per_set: 0,
      })
      
      // Reload exercises to include the new one
      await loadExercises()
      
      onSuccess()
    } catch (err) {
      setError('Failed to save exercise')
      console.error('Error saving exercise:', err)
    } finally {
      setLoading(false)
    }
  }

  const handleInputChange = (field: keyof WorkoutExerciseCreate, value: number) => {
    if (field === 'weight') {
      setDisplayWeight(value)
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

      <div className="relative">
        <label htmlFor="exercise" className="block text-sm font-medium text-gray-700 mb-1">
          Exercise
        </label>
        <input
          type="text"
          id="exercise"
          value={exerciseInput}
          onChange={(e) => {
            setExerciseInput(e.target.value)
            setShowDropdown(true)
            setSelectedExerciseId(null)
          }}
          onFocus={() => setShowDropdown(true)}
          onBlur={() => {
            // Delay hiding dropdown to allow for clicks
            setTimeout(() => setShowDropdown(false), 200)
          }}
          className="input w-full"
          placeholder="Type exercise name..."
          required
        />
        
        {showDropdown && exerciseInput && (
          <div className="absolute z-10 w-full mt-1 bg-white border border-gray-300 rounded-md shadow-lg max-h-60 overflow-auto">
            {filteredExercises.length > 0 && (
              <>
                {filteredExercises.map((exercise) => (
                  <button
                    key={exercise.id}
                    type="button"
                    className="w-full text-left px-4 py-2 hover:bg-gray-100 focus:bg-gray-100 focus:outline-none"
                    onClick={() => handleExerciseSelect(exercise)}
                  >
                    {exercise.name}
                  </button>
                ))}
                <div className="border-t border-gray-200"></div>
              </>
            )}
            
            {!exactMatch && exerciseInput.trim() && (
              <button
                type="button"
                className="w-full text-left px-4 py-2 hover:bg-gray-100 focus:bg-gray-100 focus:outline-none text-blue-600 font-medium"
                onClick={() => handleExerciseSelect(null)}
              >
                Create "{exerciseInput.trim()}"
              </button>
            )}
          </div>
        )}
      </div>

      <div className="grid grid-cols-2 gap-4">
        <div>
          <label htmlFor="weight" className="block text-sm font-medium text-gray-700 mb-1">
            Weight ({getWeightUnitSymbol(units.exerciseWeight)})
          </label>
          <input
            type="number"
            id="weight"
            step="0.5"
            min="0"
            value={displayWeight || ''}
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