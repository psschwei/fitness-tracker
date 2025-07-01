import { useState, useEffect } from 'react'
import { format, subDays, addDays } from 'date-fns'
import { DailyEntry, DailyActivity } from '../types'
import { journalApi, bodyCompositionApi, workoutApi } from '../api/client'
import DateNavigation from '../components/Journal/DateNavigation'
import DailyEntryView from '../components/Journal/DailyEntryView'
import { DailyActivityForm } from '../components/Journal/DailyActivityForm'

function Journal() {
  const [currentDate, setCurrentDate] = useState(new Date())
  const [dailyEntry, setDailyEntry] = useState<DailyEntry | null>(null)
  const [loading, setLoading] = useState(true)
  const [error, setError] = useState<string | null>(null)
  const [dailyActivity, setDailyActivity] = useState<DailyActivity | null>(null)

  const dateString = format(currentDate, 'yyyy-MM-dd')

  useEffect(() => {
    loadDailyEntry()
  }, [currentDate])

  const loadDailyEntry = async () => {
    try {
      setLoading(true)
      setError(null)
      const entry = await journalApi.getDailyEntry(dateString)
      setDailyEntry(entry)
    } catch (err) {
      console.error('Error loading daily entry:', err)
      // Create an empty entry if the API fails
      const emptyEntry: DailyEntry = {
        date: dateString,
        body_composition: undefined,
        workouts: []
      }
      setDailyEntry(emptyEntry)
      setError('Failed to load daily entry data, but you can still add new entries')
    } finally {
      setLoading(false)
    }
  }

  const goToPreviousDay = () => {
    setCurrentDate(subDays(currentDate, 1))
  }

  const goToNextDay = () => {
    setCurrentDate(addDays(currentDate, 1))
  }

  const goToDate = (date: Date) => {
    setCurrentDate(date)
  }

  const handleEntryUpdate = () => {
    loadDailyEntry()
  }

  const handleDeleteDay = async () => {
    if (!dailyEntry) return

    const hasData = dailyEntry.body_composition || dailyEntry.workouts.length > 0
    if (!hasData) {
      alert('No data to delete for this day')
      return
    }

    const confirmMessage = `Are you sure you want to delete ALL data for ${format(currentDate, 'MMMM d, yyyy')}?\n\nThis will delete:\n` +
      (dailyEntry.body_composition ? '• Body composition entry\n' : '') +
      (dailyEntry.workouts.length > 0 ? `• ${dailyEntry.workouts.length} workout(s) with ${dailyEntry.workouts.reduce((total, w) => total + w.exercises.length, 0)} exercise(s)\n` : '') +
      '\nThis action cannot be undone.'

    if (confirm(confirmMessage)) {
      try {
        const deletePromises = []

        // Delete body composition if it exists
        if (dailyEntry.body_composition) {
          deletePromises.push(bodyCompositionApi.delete(dailyEntry.body_composition.id))
        }

        // Delete all workouts for this day
        dailyEntry.workouts.forEach(workout => {
          deletePromises.push(workoutApi.delete(workout.id))
        })

        await Promise.all(deletePromises)
        loadDailyEntry() // Reload to show empty state
      } catch (err) {
        console.error('Error deleting day:', err)
        alert('Failed to delete day data')
      }
    }
  }

  const handleActivityUpdated = (activity: DailyActivity) => {
    setDailyActivity(activity)
  }

  if (loading) {
    return (
      <div className="flex justify-center items-center h-64">
        <div className="text-gray-500">Loading...</div>
      </div>
    )
  }

  if (error) {
    return (
      <div className="flex justify-center items-center h-64">
        <div className="text-red-500">{error}</div>
      </div>
    )
  }

  return (
    <div className="space-y-6">
      <div className="flex items-center justify-between">
        <h1 className="text-2xl font-bold text-gray-900">Daily Journal</h1>
        {dailyEntry && (dailyEntry.body_composition || dailyEntry.workouts.length > 0) && (
          <button
            onClick={handleDeleteDay}
            className="btn bg-red-600 hover:bg-red-700 text-white text-sm"
            title="Delete all data for this day"
          >
            Delete Day
          </button>
        )}
      </div>

      <DateNavigation
        currentDate={currentDate}
        onPreviousDay={goToPreviousDay}
        onNextDay={goToNextDay}
        onDateSelect={goToDate}
      />

      <div className="space-y-6">
        {dailyEntry ? (
          <DailyEntryView
            entry={dailyEntry}
            onUpdate={handleEntryUpdate}
          />
        ) : (
          <div className="text-center py-8">
            <div className="text-gray-500">Loading entry...</div>
          </div>
        )}
        
        <DailyActivityForm 
          date={dateString} 
          onActivityUpdated={handleActivityUpdated}
        />
      </div>
    </div>
  )
}

export default Journal 