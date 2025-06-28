import { DailyEntry } from '../../types'
import { bodyCompositionApi, workoutApi } from '../../api/client'
import BodyCompositionForm from './BodyCompositionForm'
import ExerciseForm from './ExerciseForm'
import BatchExerciseForm from './BatchExerciseForm'
import { useUnits } from '../../contexts/UnitContext'
import { convertWeight, convertLength, formatWeight, formatLength, calculateBMI, formatBMI, formatBodyFat } from '../../utils/units'

interface DailyEntryViewProps {
  entry: DailyEntry
  onUpdate: () => void
}

function DailyEntryView({ entry, onUpdate }: DailyEntryViewProps) {
  const { units } = useUnits()
  const workouts = entry.workouts ?? []

  const handleDeleteBodyComposition = async () => {
    if (!entry.body_composition) return
    
    if (confirm('Are you sure you want to delete this body composition entry?')) {
      try {
        await bodyCompositionApi.delete(entry.body_composition.id)
        onUpdate()
      } catch (err) {
        console.error('Error deleting body composition:', err)
        alert('Failed to delete body composition entry')
      }
    }
  }

  const handleDeleteWorkout = async (workoutId: number) => {
    if (confirm('Are you sure you want to delete this entire workout?')) {
      try {
        await workoutApi.delete(workoutId)
        onUpdate()
      } catch (err) {
        console.error('Error deleting workout:', err)
        alert('Failed to delete workout')
      }
    }
  }

  return (
    <div className="space-y-6">
      {/* Body Composition Section */}
      <div className="card">
        <h2 className="text-lg font-semibold text-gray-900 mb-4">Body Composition</h2>
        
        {entry.body_composition ? (
          <div className="bg-gray-50 rounded-lg p-4">
            <div className="flex items-center justify-between">
              <div className="grid grid-cols-2 gap-4">
                <div>
                  <span className="text-sm font-medium text-gray-500">Weight</span>
                  <p className="text-lg font-semibold text-gray-900">
                    {formatWeight(convertWeight(entry.body_composition.weight_pounds, 'lbs', units.bodyWeight), units.bodyWeight)}
                  </p>
                </div>
                <div>
                  <span className="text-sm font-medium text-gray-500">Height</span>
                  <p className="text-lg font-semibold text-gray-900">
                    {entry.body_composition.height_inches ? 
                      formatLength(convertLength(entry.body_composition.height_inches, 'inches', units.length), units.length) :
                      'Not recorded'
                    }
                  </p>
                </div>
                <div>
                  <span className="text-sm font-medium text-gray-500">Waist Circumference</span>
                  <p className="text-lg font-semibold text-gray-900">
                    {entry.body_composition.waist_inches ? 
                      formatLength(convertLength(entry.body_composition.waist_inches, 'inches', units.length), units.length) :
                      'Not recorded'
                    }
                  </p>
                </div>
                <div>
                  <span className="text-sm font-medium text-gray-500">Neck Circumference</span>
                  <p className="text-lg font-semibold text-gray-900">
                    {entry.body_composition.neck_inches ? 
                      formatLength(convertLength(entry.body_composition.neck_inches, 'inches', units.length), units.length) :
                      'Not recorded'
                    }
                  </p>
                </div>
              </div>
              <div className="flex flex-col items-end space-y-2">
                <button
                  onClick={handleDeleteBodyComposition}
                  className="text-red-500 hover:text-red-700 text-sm font-medium"
                  title="Delete body composition entry"
                >
                  Delete
                </button>
                {entry.body_composition.height_inches && entry.body_composition.weight_pounds > 0 && (
                  <div className="text-right">
                    <span className="text-sm font-medium text-gray-500">BMI</span>
                    <p className="text-lg font-semibold text-blue-600">
                      {entry.body_composition.bmi ? formatBMI(entry.body_composition.bmi) : 'Calculating...'}
                    </p>
                  </div>
                )}
                {entry.body_composition.waist_inches && entry.body_composition.neck_inches && entry.body_composition.height_inches && (
                  <div className="text-right">
                    <span className="text-sm font-medium text-gray-500">Body Fat</span>
                    <p className="text-lg font-semibold text-green-600">
                      {entry.body_composition.body_fat_percentage ? 
                        formatBodyFat(entry.body_composition.body_fat_percentage, entry.body_composition.is_male) : 
                        'Calculating...'
                      }
                    </p>
                  </div>
                )}
              </div>
            </div>
          </div>
        ) : (
          <BodyCompositionForm date={entry.date} onSuccess={onUpdate} />
        )}
      </div>

      {/* Exercises Section */}
      <div className="card">
        <div className="flex items-center justify-between mb-4">
          <h2 className="text-lg font-semibold text-gray-900">Exercises</h2>
          <div className="flex space-x-2">
            <button className="btn btn-primary text-sm">
              Add Exercise
            </button>
            <button className="btn btn-secondary text-sm">
              Batch Add
            </button>
          </div>
        </div>

        {workouts.length > 0 ? (
          <div className="space-y-4">
            {workouts.map((workout) => (
              <div key={workout.id} className="bg-gray-50 rounded-lg p-4">
                <div className="flex items-center justify-between mb-3">
                  <h3 className="text-sm font-medium text-gray-700">Workout</h3>
                  <button
                    onClick={() => handleDeleteWorkout(workout.id)}
                    className="text-red-500 hover:text-red-700 text-sm font-medium"
                    title="Delete entire workout"
                  >
                    Delete Workout
                  </button>
                </div>
                <div className="space-y-3">
                  {workout.exercises.map((exercise) => (
                    <div key={exercise.id} className="flex items-center justify-between bg-white rounded p-3">
                      <div>
                        <h3 className="font-medium text-gray-900">{exercise.exercise_name}</h3>
                        <p className="text-sm text-gray-500">
                          {formatWeight(convertWeight(exercise.weight, 'lbs', units.exerciseWeight), units.exerciseWeight)} × {exercise.reps_per_set} reps
                        </p>
                      </div>
                      <button className="text-red-500 hover:text-red-700 text-sm">
                        Remove
                      </button>
                    </div>
                  ))}
                </div>
              </div>
            ))}
          </div>
        ) : (
          <div className="text-center py-8">
            <p className="text-gray-500 mb-4">No exercises recorded for this day</p>
            <div className="flex justify-center space-x-2">
              <ExerciseForm date={entry.date} onSuccess={onUpdate} />
              <BatchExerciseForm date={entry.date} onSuccess={onUpdate} />
            </div>
          </div>
        )}
      </div>
    </div>
  )
}

export default DailyEntryView 