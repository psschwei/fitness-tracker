// Body Composition Types
export interface BodyComposition {
  id: number
  date: string
  weight_pounds: number
  height_inches: number | null
  waist_inches: number | null
  neck_inches: number | null
  bmi: number | null
  body_fat_percentage: number | null
  is_male: boolean
  created_at: string
  updated_at: string
  notes?: string | null
}

export interface BodyCompositionCreate {
  date: string
  weight_pounds: number
  height_inches: number | null
  waist_inches: number | null
  neck_inches: number | null
  bmi?: number | null
  body_fat_percentage?: number | null
  is_male: boolean
  notes?: string | null
}

export interface BodyCompositionUpdate {
  weight_pounds?: number
  height_inches?: number | null
  waist_inches?: number | null
  neck_inches?: number | null
  bmi?: number | null
  body_fat_percentage?: number | null
  is_male?: boolean
  notes?: string | null
}

// Exercise Types
export interface Exercise {
  id: number
  name: string
  created_at: string
  updated_at: string
}

export interface ExerciseCreate {
  name: string
}

// Workout Types
export interface Workout {
  id: number
  date: string
  created_at: string
  updated_at: string
  exercises: WorkoutExercise[]
}

export interface WorkoutCreate {
  date: string
  exercises: WorkoutExerciseCreate[]
}

export interface WorkoutExercise {
  id: number
  workout_id: number
  exercise_id: number
  exercise_name: string
  weight: number
  reps_per_set: number
  created_at: string
  updated_at: string
}

export interface WorkoutExerciseCreate {
  exercise_id: number
  weight: number
  reps_per_set: number
}

// Journal Types
export interface DailyEntry {
  date: string
  body_composition?: BodyComposition
  workouts: Workout[]
}

// Analytics Types
export interface WeightTrend {
  date: string
  weight: number
}

export interface BodyCompositionTrend {
  date: string
  weight: number
  waist_circumference: number
}

export interface ExerciseProgress {
  exercise_name: string
  date: string
  max_weight: number
  total_reps: number
}

export interface DashboardSummary {
  total_measurements: number
  total_workouts: number
  current_weight?: number
  weight_change_7d?: number
  weight_change_30d?: number
} 