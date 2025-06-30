import { useState } from 'react'
import { DailyEntry } from '../../types'
import { bodyCompositionApi, workoutApi } from '../../api/client'
import BodyCompositionForm from './BodyCompositionForm'
import BodyCompositionEditForm from './BodyCompositionEditForm'
import WorkoutEditForm from './WorkoutEditForm'
import BatchExerciseForm from './BatchExerciseForm'
import { convertWeight, convertLength, formatWeight, formatLength, calculateBMI, formatBMI, formatBodyFat, BODY_WEIGHT_UNIT, EXERCISE_WEIGHT_UNIT, LENGTH_UNIT } from '../../utils/units'

interface DailyEntryViewProps {
  entry: DailyEntry
  onUpdate: () => void
}

function DailyEntryView({ entry, onUpdate }: DailyEntryViewProps) {
  const workouts = entry.workouts ?? []
  const [isEditingBodyComposition, setIsEditingBodyComposition] = useState(false)
  const [editingWorkoutId, setEditingWorkoutId] = useState<number | null>(null)

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

  const handleDeleteExercise = async (exerciseId: number) => {
    if (confirm('Are you sure you want to delete this exercise from the workout?')) {
      try {
        await workoutApi.deleteExercise(exerciseId)
        onUpdate()
      } catch (err) {
        console.error('Error deleting exercise:', err)
        alert('Failed to delete exercise')
      }
    }
  }

  return (
    <div className="space-y-6">
      {/* Body Composition Section */}
      <div className="card">
        <h2 className="text-lg font-semibold text-gray-900 mb-4">Body Composition</h2>
        
        {entry.body_composition ? (
          isEditingBodyComposition ? (
            <BodyCompositionEditForm
              measurement={entry.body_composition}
              onSuccess={() => {
                setIsEditingBodyComposition(false)
                onUpdate()
              }}
              onCancel={() => setIsEditingBodyComposition(false)}
            />
          ) : (
            <div className="bg-gray-50 rounded-lg p-4">
              <div className="flex items-center justify-between">
                <div className="grid grid-cols-2 gap-4">
                  <div>
                    <span className="text-sm font-medium text-gray-500">Weight</span>
                    <p className="text-lg font-semibold text-gray-900">
                      {formatWeight(convertWeight(entry.body_composition.weight_pounds, 'lbs', BODY_WEIGHT_UNIT), BODY_WEIGHT_UNIT)}
                    </p>
                  </div>
                  <div>
                    <span className="text-sm font-medium text-gray-500">Height</span>
                    <p className="text-lg font-semibold text-gray-900">
                      {entry.body_composition.height_inches ? 
                        formatLength(convertLength(entry.body_composition.height_inches, 'inches', LENGTH_UNIT), LENGTH_UNIT) :
                        'Not recorded'
                      }
                    </p>
                  </div>
                  <div>
                    <span className="text-sm font-medium text-gray-500">Waist Circumference</span>
                    <p className="text-lg font-semibold text-gray-900">
                      {entry.body_composition.waist_inches ? 
                        formatLength(convertLength(entry.body_composition.waist_inches, 'inches', LENGTH_UNIT), LENGTH_UNIT) :
                        'Not recorded'
                      }
                    </p>
                  </div>
                  <div>
                    <span className="text-sm font-medium text-gray-500">Neck Circumference</span>
                    <p className="text-lg font-semibold text-gray-900">
                      {entry.body_composition.neck_inches ? 
                        formatLength(convertLength(entry.body_composition.neck_inches, 'inches', LENGTH_UNIT), LENGTH_UNIT) :
                        'Not recorded'
                      }
                    </p>
                  </div>
                </div>
                <div className="flex flex-col items-end space-y-2">
                  <div className="flex space-x-2">
                    <button
                      onClick={() => setIsEditingBodyComposition(true)}
                      className="text-blue-500 hover:text-blue-700 text-sm font-medium"
                      title="Edit body composition entry"
                    >
                      Edit
                    </button>
                    <button
                      onClick={handleDeleteBodyComposition}
                      className="text-red-500 hover:text-red-700 text-sm font-medium"
                      title="Delete body composition entry"
                    >
                      Delete
                    </button>
                  </div>
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
          )
        ) : (
          <BodyCompositionForm date={entry.date} onSuccess={onUpdate} />
        )}
      </div>

      {/* Exercises Section */}
      <div className="card">
        <div className="flex items-center justify-between mb-4">
          <h2 className="text-lg font-semibold text-gray-900">Workouts</h2>
        </div>

        {workouts.length > 0 ? (
          <div className="space-y-4">
            {workouts.map((workout) => (
              <div key={workout.id} className="bg-gray-50 rounded-lg p-4">
                {editingWorkoutId === workout.id ? (
                  <WorkoutEditForm
                    workout={workout}
                    onSuccess={() => {
                      setEditingWorkoutId(null)
                      onUpdate()
                    }}
                    onCancel={() => setEditingWorkoutId(null)}
                  />
                ) : (
                  <>
                    <div className="flex items-center justify-between mb-3">
                      <h3 className="text-sm font-medium text-gray-700">Workout</h3>
                      <div className="flex space-x-2">
                        <button
                          onClick={() => setEditingWorkoutId(workout.id)}
                          className="text-blue-500 hover:text-blue-700 text-sm font-medium"
                          title="Edit workout"
                        >
                          Edit
                        </button>
                        <button
                          onClick={() => handleDeleteWorkout(workout.id)}
                          className="text-red-500 hover:text-red-700 text-sm font-medium"
                          title="Delete entire workout"
                        >
                          Delete Workout
                        </button>
                      </div>
                    </div>
                    {workout.notes && (
                      <div className="mb-3 p-3 bg-blue-50 rounded border-l-4 border-blue-400">
                        <p className="text-sm text-blue-800 italic">
                          "{workout.notes}"
                        </p>
                      </div>
                    )}
                    <div className="space-y-3">
                      {workout.exercises.map((exercise) => (
                        <div key={exercise.id} className="flex items-center justify-between bg-white rounded p-3">
                          <div className="flex-1">
                            <h3 className="font-medium text-gray-900">{exercise.exercise_name}</h3>
                            <div className="text-sm text-gray-500">
                              {exercise.sets_data.map((set, index) => (
                                <div key={index}>
                                  {set.weight === 0 ? 'Bodyweight' : formatWeight(convertWeight(set.weight, 'lbs', EXERCISE_WEIGHT_UNIT), EXERCISE_WEIGHT_UNIT)} × {set.reps} reps × {set.sets} sets
                                </div>
                              ))}
                            </div>
                            {exercise.notes && (
                              <p className="text-sm text-gray-600 mt-1 italic">
                                "{exercise.notes}"
                              </p>
                            )}
                          </div>
                          <button
                            onClick={() => handleDeleteExercise(exercise.id)}
                            className="text-red-500 hover:text-red-700 text-sm ml-4"
                            title="Delete exercise"
                          >
                            Remove
                          </button>
                        </div>
                      ))}
                    </div>
                  </>
                )}
              </div>
            ))}
          </div>
        ) : null}

        <BatchExerciseForm date={entry.date} onSuccess={onUpdate} />
      </div>
    </div>
  )
}

export default DailyEntryView 