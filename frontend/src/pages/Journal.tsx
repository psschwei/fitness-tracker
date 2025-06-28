import { useState, useEffect } from 'react'
import { format, subDays, addDays } from 'date-fns'
import { DailyEntry } from '../types'
import { journalApi } from '../api/client'
import DateNavigation from '../components/Journal/DateNavigation'
import DailyEntryView from '../components/Journal/DailyEntryView'

function Journal() {
  const [currentDate, setCurrentDate] = useState(new Date())
  const [dailyEntry, setDailyEntry] = useState<DailyEntry | null>(null)
  const [loading, setLoading] = useState(true)
  const [error, setError] = useState<string | null>(null)

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
      setError('Failed to load daily entry')
      console.error('Error loading daily entry:', err)
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
      </div>

      <DateNavigation
        currentDate={currentDate}
        onPreviousDay={goToPreviousDay}
        onNextDay={goToNextDay}
        onDateSelect={goToDate}
      />

      {dailyEntry && (
        <DailyEntryView
          entry={dailyEntry}
          onUpdate={handleEntryUpdate}
        />
      )}
    </div>
  )
}

export default Journal 