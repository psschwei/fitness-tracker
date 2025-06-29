import {
  BodyComposition,
  BodyCompositionCreate,
  BodyCompositionUpdate,
  Exercise,
  ExerciseCreate,
  Workout,
  WorkoutCreate,
  DailyEntry,
  WeightTrend,
  BodyCompositionTrend,
  ExerciseProgress,
  DashboardSummary,
  DailyActivity,
  DailyActivityCreate,
  DailyActivityUpdate,
} from '../types'

const API_BASE = '/api'

class ApiError extends Error {
  constructor(public status: number, message: string) {
    super(message)
    this.name = 'ApiError'
  }
}

async function apiRequest<T>(
  endpoint: string,
  options: RequestInit = {}
): Promise<T> {
  const response = await fetch(`${API_BASE}${endpoint}`, {
    headers: {
      'Content-Type': 'application/json',
      ...options.headers,
    },
    ...options,
  })

  if (!response.ok) {
    throw new ApiError(response.status, `API request failed: ${response.statusText}`)
  }

  return response.json()
}

// Body Composition API
export const bodyCompositionApi = {
  create: (data: BodyCompositionCreate): Promise<BodyComposition> =>
    apiRequest('/body-composition/', {
      method: 'POST',
      body: JSON.stringify(data),
    }),

  getAll: (): Promise<BodyComposition[]> =>
    apiRequest('/body-composition/'),

  getLatest: (): Promise<BodyComposition> =>
    apiRequest('/body-composition/latest'),

  getByDate: (date: string): Promise<BodyComposition> =>
    apiRequest(`/body-composition/date/${date}`),

  update: (id: number, data: BodyCompositionUpdate): Promise<BodyComposition> =>
    apiRequest(`/body-composition/${id}`, {
      method: 'PUT',
      body: JSON.stringify(data),
    }),

  delete: (id: number): Promise<void> =>
    apiRequest(`/body-composition/${id}`, {
      method: 'DELETE',
    }),

  getTrends: (days: number): Promise<BodyCompositionTrend[]> =>
    apiRequest(`/body-composition/trends/${days}`),

  getStatistics: (): Promise<DashboardSummary> =>
    apiRequest('/body-composition/statistics/overview'),
}

// Exercise API
export const exerciseApi = {
  create: (data: ExerciseCreate): Promise<Exercise> =>
    apiRequest('/exercise/exercises', {
      method: 'POST',
      body: JSON.stringify(data),
    }),

  getAll: (): Promise<Exercise[]> =>
    apiRequest('/exercise/exercises'),

  update: (id: number, data: ExerciseCreate): Promise<Exercise> =>
    apiRequest(`/exercise/exercises/${id}`, {
      method: 'PUT',
      body: JSON.stringify(data),
    }),

  delete: (id: number): Promise<void> =>
    apiRequest(`/exercise/exercises/${id}`, {
      method: 'DELETE',
    }),

  getProgress: (id: number, days: number = 30): Promise<ExerciseProgress[]> =>
    apiRequest(`/exercise/exercises/${id}/progress?days=${days}`),
}

// Workout API
export const workoutApi = {
  create: (data: WorkoutCreate): Promise<Workout> =>
    apiRequest('/exercise/workouts', {
      method: 'POST',
      body: JSON.stringify(data),
    }),

  getAll: (): Promise<Workout[]> =>
    apiRequest('/exercise/workouts'),

  getByDate: (date: string): Promise<Workout[]> =>
    apiRequest(`/exercise/workouts/date/${date}`),

  update: (id: number, data: WorkoutCreate): Promise<Workout> =>
    apiRequest(`/exercise/workouts/${id}`, {
      method: 'PUT',
      body: JSON.stringify(data),
    }),

  delete: (id: number): Promise<void> =>
    apiRequest(`/exercise/workouts/${id}`, {
      method: 'DELETE',
    }),

  deleteExercise: (exerciseId: number): Promise<void> =>
    apiRequest(`/exercise/workout-exercises/${exerciseId}`, {
      method: 'DELETE',
    }),
}

// Journal API
export const journalApi = {
  getDailyEntry: (date: string): Promise<DailyEntry> =>
    apiRequest(`/journal/daily/${date}`),

  getDateRange: (start_date: string, end_date: string): Promise<DailyEntry[]> =>
    apiRequest(`/journal/range?start_date=${start_date}&end_date=${end_date}`),
}

// Analytics API
export const analyticsApi = {
  getWeightTrend: (days: number = 30): Promise<WeightTrend[]> =>
    apiRequest(`/analytics/charts/weight-trend?days=${days}`),

  getBodyCompositionChart: (days: number = 30): Promise<BodyCompositionTrend[]> =>
    apiRequest(`/analytics/charts/body-composition?days=${days}`),

  getExerciseProgressChart: (exercise_id: number, days: number = 30): Promise<ExerciseProgress[]> =>
    apiRequest(`/analytics/charts/exercise-progress/${exercise_id}?days=${days}`),

  getDashboardSummary: (): Promise<DashboardSummary> =>
    apiRequest('/analytics/summary/dashboard'),
}

// Daily Activity API
export const getDailyActivities = async (): Promise<DailyActivity[]> => {
  const response = await fetch(`${API_BASE}/exercise/daily-activities`);
  if (!response.ok) {
    throw new Error('Failed to fetch daily activities');
  }
  return response.json();
};

export const getDailyActivityByDate = async (date: string): Promise<DailyActivity | null> => {
  try {
    const response = await fetch(`${API_BASE}/exercise/daily-activities/date/${date}`);
    if (response.status === 404) {
      return null;
    }
    if (!response.ok) {
      throw new Error('Failed to fetch daily activity');
    }
    return response.json();
  } catch (error) {
    console.error('Error fetching daily activity:', error);
    return null;
  }
};

export const createOrUpdateDailyActivity = async (activity: DailyActivityCreate): Promise<DailyActivity> => {
  const response = await fetch(`${API_BASE}/exercise/daily-activities`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify(activity),
  });
  if (!response.ok) {
    throw new Error('Failed to create/update daily activity');
  }
  return response.json();
};

export const updateDailyActivity = async (id: number, activity: DailyActivityUpdate): Promise<DailyActivity> => {
  const response = await fetch(`${API_BASE}/exercise/daily-activities/${id}`, {
    method: 'PUT',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify(activity),
  });
  if (!response.ok) {
    throw new Error('Failed to update daily activity');
  }
  return response.json();
};

export const deleteDailyActivity = async (id: number): Promise<void> => {
  const response = await fetch(`${API_BASE}/exercise/daily-activities/${id}`, {
    method: 'DELETE',
  });
  if (!response.ok) {
    throw new Error('Failed to delete daily activity');
  }
}; 