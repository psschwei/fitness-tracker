import { useState, useEffect } from 'react'
import { WeightTrend, BodyCompositionTrend } from '../types'
import { analyticsApi } from '../api/client'
import { useUnits } from '../contexts/UnitContext'
import { convertWeight, convertLength, formatWeight, formatLength } from '../utils/units'

function Charts() {
  const { units } = useUnits()
  const [weightTrend, setWeightTrend] = useState<WeightTrend[]>([])
  const [bodyCompositionTrend, setBodyCompositionTrend] = useState<BodyCompositionTrend[]>([])
  const [loading, setLoading] = useState(true)
  const [error, setError] = useState<string | null>(null)

  useEffect(() => {
    loadCharts()
  }, [])

  const loadCharts = async () => {
    try {
      setLoading(true)
      setError(null)
      
      const [weightData, bodyCompData] = await Promise.all([
        analyticsApi.getWeightTrend(30),
        analyticsApi.getBodyCompositionChart(30)
      ])
      
      setWeightTrend(weightData)
      setBodyCompositionTrend(bodyCompData)
    } catch (err) {
      setError('Failed to load charts')
      console.error('Error loading charts:', err)
    } finally {
      setLoading(false)
    }
  }

  if (loading) {
    return (
      <div className="flex justify-center items-center h-64">
        <div className="text-gray-500">Loading charts...</div>
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
        <h1 className="text-2xl font-bold text-gray-900">Analytics & Charts</h1>
      </div>

      <div className="grid grid-cols-1 lg:grid-cols-2 gap-6">
        {/* Weight Trend Chart */}
        <div className="card">
          <h2 className="text-lg font-semibold text-gray-900 mb-4">Weight Trend (30 days)</h2>
          {weightTrend.length > 0 ? (
            <div className="h-64 bg-gray-50 rounded flex items-center justify-center">
              <p className="text-gray-500">Chart placeholder - {weightTrend.length} data points</p>
            </div>
          ) : (
            <div className="h-64 bg-gray-50 rounded flex items-center justify-center">
              <p className="text-gray-500">No weight data available</p>
            </div>
          )}
        </div>

        {/* Body Composition Chart */}
        <div className="card">
          <h2 className="text-lg font-semibold text-gray-900 mb-4">Body Composition (30 days)</h2>
          {bodyCompositionTrend.length > 0 ? (
            <div className="h-64 bg-gray-50 rounded flex items-center justify-center">
              <p className="text-gray-500">Chart placeholder - {bodyCompositionTrend.length} data points</p>
            </div>
          ) : (
            <div className="h-64 bg-gray-50 rounded flex items-center justify-center">
              <p className="text-gray-500">No body composition data available</p>
            </div>
          )}
        </div>
      </div>

      {/* Data Tables */}
      <div className="grid grid-cols-1 lg:grid-cols-2 gap-6">
        <div className="card">
          <h2 className="text-lg font-semibold text-gray-900 mb-4">Recent Weight Data</h2>
          <div className="overflow-x-auto">
            <table className="min-w-full divide-y divide-gray-200">
              <thead className="bg-gray-50">
                <tr>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                    Date
                  </th>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                    Weight ({units.bodyWeight})
                  </th>
                </tr>
              </thead>
              <tbody className="bg-white divide-y divide-gray-200">
                {weightTrend.slice(-10).reverse().map((entry) => (
                  <tr key={entry.date}>
                    <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-900">
                      {entry.date}
                    </td>
                    <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-900">
                      {formatWeight(convertWeight(entry.weight, 'lbs', units.bodyWeight), units.bodyWeight)}
                    </td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        </div>

        <div className="card">
          <h2 className="text-lg font-semibold text-gray-900 mb-4">Recent Body Composition</h2>
          <div className="overflow-x-auto">
            <table className="min-w-full divide-y divide-gray-200">
              <thead className="bg-gray-50">
                <tr>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                    Date
                  </th>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                    Weight
                  </th>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                    Waist
                  </th>
                </tr>
              </thead>
              <tbody className="bg-white divide-y divide-gray-200">
                {bodyCompositionTrend.slice(-10).reverse().map((entry) => (
                  <tr key={entry.date}>
                    <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-900">
                      {entry.date}
                    </td>
                    <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-900">
                      {formatWeight(convertWeight(entry.weight, 'lbs', units.bodyWeight), units.bodyWeight)}
                    </td>
                    <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-900">
                      {formatLength(convertLength(entry.waist_circumference, 'inches', units.length), units.length)}
                    </td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        </div>
      </div>
    </div>
  )
}

export default Charts 