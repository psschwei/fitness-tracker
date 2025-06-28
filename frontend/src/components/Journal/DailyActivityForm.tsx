import React, { useState, useEffect } from 'react';
import { DailyActivity, DailyActivityCreate } from '../../types';
import { createOrUpdateDailyActivity, getDailyActivityByDate } from '../../api/client';

interface DailyActivityFormProps {
  date: string;
  onActivityUpdated?: (activity: DailyActivity) => void;
}

export const DailyActivityForm: React.FC<DailyActivityFormProps> = ({ 
  date, 
  onActivityUpdated 
}) => {
  const [steps, setSteps] = useState<number | undefined>(undefined);
  const [walkYesNo, setWalkYesNo] = useState<boolean | undefined>(undefined);
  const [notes, setNotes] = useState<string>('');
  const [loading, setLoading] = useState(false);
  const [savingSteps, setSavingSteps] = useState(false);
  const [savingWalk, setSavingWalk] = useState(false);
  const [savingAll, setSavingAll] = useState(false);
  const [currentActivity, setCurrentActivity] = useState<DailyActivity | null>(null);
  const [hasUnsavedChanges, setHasUnsavedChanges] = useState(false);

  useEffect(() => {
    loadActivity();
  }, [date]);

  const loadActivity = async () => {
    try {
      const activity = await getDailyActivityByDate(date);
      if (activity) {
        setCurrentActivity(activity);
        setSteps(activity.steps || undefined);
        setWalkYesNo(activity.walk_yes_no !== undefined ? activity.walk_yes_no : undefined);
        setNotes(activity.notes || '');
      } else {
        setCurrentActivity(null);
        setSteps(undefined);
        setWalkYesNo(undefined);
        setNotes('');
      }
      setHasUnsavedChanges(false);
    } catch (error) {
      console.error('Error loading daily activity:', error);
    }
  };

  useEffect(() => {
    const hasChanges = 
      (steps !== undefined && steps !== (currentActivity?.steps || undefined)) ||
      (walkYesNo !== undefined && walkYesNo !== (currentActivity?.walk_yes_no ?? undefined)) ||
      (notes !== (currentActivity?.notes || ''));
    setHasUnsavedChanges(hasChanges);
  }, [steps, walkYesNo, notes, currentActivity]);

  const saveSteps = async () => {
    if (steps === undefined) return;
    
    setSavingSteps(true);
    try {
      const activityData: DailyActivityCreate = {
        date,
        steps,
        walk_yes_no: walkYesNo,
        notes: notes || undefined
      };
      
      const updatedActivity = await createOrUpdateDailyActivity(activityData);
      setCurrentActivity(updatedActivity);
      onActivityUpdated?.(updatedActivity);
    } catch (error) {
      console.error('Error saving steps:', error);
    } finally {
      setSavingSteps(false);
    }
  };

  const saveWalk = async () => {
    if (walkYesNo === undefined) return;
    
    setSavingWalk(true);
    try {
      const activityData: DailyActivityCreate = {
        date,
        steps,
        walk_yes_no: walkYesNo,
        notes: notes || undefined
      };
      
      const updatedActivity = await createOrUpdateDailyActivity(activityData);
      setCurrentActivity(updatedActivity);
      onActivityUpdated?.(updatedActivity);
    } catch (error) {
      console.error('Error saving walk status:', error);
    } finally {
      setSavingWalk(false);
    }
  };

  const saveAllChanges = async () => {
    setSavingAll(true);
    try {
      const activityData: DailyActivityCreate = {
        date,
        steps,
        walk_yes_no: walkYesNo,
        notes: notes || undefined
      };
      
      const updatedActivity = await createOrUpdateDailyActivity(activityData);
      setCurrentActivity(updatedActivity);
      onActivityUpdated?.(updatedActivity);
    } catch (error) {
      console.error('Error saving all changes:', error);
    } finally {
      setSavingAll(false);
    }
  };

  return (
    <div className="bg-white rounded-lg shadow p-6 mb-6">
      <h3 className="text-lg font-semibold mb-4">Daily Activity</h3>
      
      <div className="space-y-4">
        {/* Steps Section */}
        <div className="flex items-center space-x-4">
          <label className="flex-1">
            <span className="block text-sm font-medium text-gray-700 mb-1">
              Steps Today
            </span>
            <input
              type="number"
              min="0"
              value={steps || ''}
              onChange={(e) => setSteps(e.target.value ? parseInt(e.target.value) : undefined)}
              className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
              placeholder="Enter steps"
            />
          </label>
          <button
            onClick={saveSteps}
            disabled={savingSteps || steps === undefined}
            className="px-4 py-2 bg-blue-600 text-white rounded-md hover:bg-blue-700 disabled:opacity-50 disabled:cursor-not-allowed"
          >
            {savingSteps ? 'Saving...' : 'Save Steps'}
          </button>
        </div>

        {/* Walk Section */}
        <div className="flex items-center space-x-4">
          <label className="flex-1">
            <span className="block text-sm font-medium text-gray-700 mb-1">
              Took a Walk Today?
            </span>
            <select
              value={walkYesNo === undefined ? '' : walkYesNo.toString()}
              onChange={(e) => setWalkYesNo(e.target.value === '' ? undefined : e.target.value === 'true')}
              className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
            >
              <option value="">Select...</option>
              <option value="true">Yes</option>
              <option value="false">No</option>
            </select>
          </label>
          <button
            onClick={saveWalk}
            disabled={savingWalk || walkYesNo === undefined}
            className="px-4 py-2 bg-green-600 text-white rounded-md hover:bg-green-700 disabled:opacity-50 disabled:cursor-not-allowed"
          >
            {savingWalk ? 'Saving...' : 'Save Walk'}
          </button>
        </div>

        {/* Notes Section */}
        <div>
          <label>
            <span className="block text-sm font-medium text-gray-700 mb-1">
              Notes
            </span>
            <textarea
              value={notes}
              onChange={(e) => setNotes(e.target.value)}
              className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
              rows={2}
              placeholder="Optional notes about your activity..."
            />
          </label>
        </div>

        {/* Save All Changes Button */}
        <div className="pt-2">
          <button
            onClick={saveAllChanges}
            disabled={savingAll || !hasUnsavedChanges}
            className={`w-full px-4 py-2 rounded-md text-white font-medium ${
              hasUnsavedChanges 
                ? 'bg-purple-600 hover:bg-purple-700' 
                : 'bg-gray-300 text-gray-500 cursor-not-allowed'
            }`}
          >
            {savingAll ? 'Saving All Changes...' : 'Save All Unsaved Changes'}
          </button>
        </div>

        {/* Current Status */}
        {currentActivity && (
          <div className="mt-4 p-3 bg-gray-50 rounded-md">
            <p className="text-sm text-gray-600">
              <strong>Current:</strong> 
              {currentActivity.steps && ` ${currentActivity.steps} steps`}
              {currentActivity.walk_yes_no !== undefined && (
                <span className={currentActivity.walk_yes_no ? ' text-green-600' : ' text-red-600'}>
                  {currentActivity.walk_yes_no ? ' ✓ Walk taken' : ' ✗ No walk'}
                </span>
              )}
              {currentActivity.notes && (
                <span className=" text-gray-600">
                  {` - "${currentActivity.notes}"`}
                </span>
              )}
            </p>
          </div>
        )}
      </div>
    </div>
  );
}; 