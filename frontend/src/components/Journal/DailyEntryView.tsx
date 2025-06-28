import { DailyEntry } from '../../types'
import BodyCompositionForm from './BodyCompositionForm'
import ExerciseForm from './ExerciseForm'
import BatchExerciseForm from './BatchExerciseForm'

interface DailyEntryViewProps {
  entry: DailyEntry
  onUpdate: () => void
}

function DailyEntryView({ entry, onUpdate }: DailyEntryViewProps) {
  const workouts = entry.workouts ?? []
  return (
    <div className="space-y-6">
      {/* Body Composition Section */}
      <div className="card">
        <h2 className="text-lg font-semibold text-gray-900 mb-4">Body Composition</h2>
        
        {entry.body_composition ? (
          <div className="bg-gray-50 rounded-lg p-4">
            <div className="grid grid-cols-2 gap-4">
              <div>
                <span className="text-sm font-medium text-gray-500">Weight</span>
                <p className="text-lg font-semibold text-gray-900">
                  {entry.body_composition.weight} lbs
                </p>
              </div>
              <div>
                <span className="text-sm font-medium text-gray-500">Waist Circumference</span>
                <p className="text-lg font-semibold text-gray-900">
                  {entry.body_composition.waist_circumference} inches
                </p>
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
                <div className="space-y-3">
                  {workout.exercises.map((exercise) => (
                    <div key={exercise.id} className="flex items-center justify-between bg-white rounded p-3">
                      <div>
                        <h3 className="font-medium text-gray-900">{exercise.exercise_name}</h3>
                        <p className="text-sm text-gray-500">
                          {exercise.weight} lbs × {exercise.reps_per_set} reps
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