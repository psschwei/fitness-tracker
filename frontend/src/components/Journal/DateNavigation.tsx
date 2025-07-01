import { format } from 'date-fns'

interface DateNavigationProps {
  currentDate: Date
  onPreviousDay: () => void
  onNextDay: () => void
  onDateSelect: (date: Date) => void
}

function DateNavigation({
  currentDate,
  onPreviousDay,
  onNextDay,
  onDateSelect,
}: DateNavigationProps) {
  const handleDateChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    // Create a date at the start of the day in local timezone
    const [year, month, day] = event.target.value.split('-').map(Number)
    const newDate = new Date(year, month - 1, day) // month is 0-indexed
    onDateSelect(newDate)
  }

  return (
    <div className="flex items-center justify-center space-x-4 bg-white rounded-lg shadow-sm border border-gray-200 p-4">
      <button
        onClick={onPreviousDay}
        className="btn btn-secondary"
        aria-label="Previous day"
      >
        ← Previous
      </button>

      <div className="flex items-center space-x-3">
        <span className="text-lg font-medium text-gray-900">
          {format(currentDate, 'EEEE, MMMM d, yyyy')}
        </span>
        
        <input
          type="date"
          value={format(currentDate, 'yyyy-MM-dd')}
          onChange={handleDateChange}
          className="input w-auto"
        />
      </div>

      <button
        onClick={onNextDay}
        className="btn btn-secondary"
        aria-label="Next day"
      >
        Next →
      </button>
    </div>
  )
}

export default DateNavigation 