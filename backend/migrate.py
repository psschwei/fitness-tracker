"""Database migration script for fitness tracker."""

import sqlite3
from pathlib import Path
from backend.database import DATABASE_URL, engine

def migrate_workout_status():
    """Add status column to workouts table if it doesn't exist."""
    # Extract database path from SQLAlchemy URL
    db_path = DATABASE_URL.replace('sqlite:///', '')
    
    if not Path(db_path).exists():
        print("Database doesn't exist yet. It will be created with the new schema.")
        return
    
    try:
        # Connect to the database
        conn = sqlite3.connect(db_path)
        cursor = conn.cursor()
        
        # Check if status column already exists
        cursor.execute("PRAGMA table_info(workouts)")
        columns = [column[1] for column in cursor.fetchall()]
        
        if 'status' not in columns:
            print("Adding status column to workouts table...")
            cursor.execute("ALTER TABLE workouts ADD COLUMN status TEXT DEFAULT 'in_progress'")
            conn.commit()
            print("✅ Status column added successfully!")
        else:
            print("✅ Status column already exists.")
        
        conn.close()
        
    except Exception as e:
        print(f"❌ Migration failed: {e}")
        raise

def migrate_workout_weights_to_kg():
    """Convert existing workout exercise weights from lbs to kg."""
    # Extract database path from SQLAlchemy URL
    db_path = DATABASE_URL.replace('sqlite:///', '')
    
    if not Path(db_path).exists():
        print("Database doesn't exist yet. It will be created with the new schema.")
        return
    
    try:
        # Connect to the database
        conn = sqlite3.connect(db_path)
        cursor = conn.cursor()
        
        # Check if migration has already been run
        cursor.execute("PRAGMA table_info(workout_exercises)")
        columns = [column[1] for column in cursor.fetchall()]
        
        # Check if we have a migration flag
        cursor.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='migrations'")
        migrations_table_exists = cursor.fetchone() is not None
        
        if not migrations_table_exists:
            cursor.execute("CREATE TABLE migrations (name TEXT PRIMARY KEY, applied_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)")
        
        # Check if this migration has been applied
        cursor.execute("SELECT name FROM migrations WHERE name = 'workout_weights_to_kg'")
        migration_applied = cursor.fetchone() is not None
        
        if migration_applied:
            print("✅ Workout weights migration already applied.")
            conn.close()
            return
        
        print("Converting workout exercise weights from lbs to kg...")
        
        # Get all workout exercises
        cursor.execute("SELECT id, sets_data FROM workout_exercises")
        exercises = cursor.fetchall()
        
        converted_count = 0
        for exercise_id, sets_data_json in exercises:
            if sets_data_json:
                import json
                try:
                    sets_data = json.loads(sets_data_json)
                    updated = False
                    
                    # Convert weights in each set
                    for set_data in sets_data:
                        if 'weight' in set_data and set_data['weight'] > 0:
                            # Convert from lbs to kg (lbs * 0.45359237)
                            old_weight = set_data['weight']
                            new_weight = round(old_weight * 0.45359237, 2)
                            set_data['weight'] = new_weight
                            updated = True
                    
                    if updated:
                        # Update the database
                        cursor.execute(
                            "UPDATE workout_exercises SET sets_data = ? WHERE id = ?",
                            (json.dumps(sets_data), exercise_id)
                        )
                        converted_count += 1
                        
                except json.JSONDecodeError:
                    print(f"Warning: Could not parse JSON for exercise {exercise_id}")
                    continue
        
        # Mark migration as applied
        cursor.execute("INSERT INTO migrations (name) VALUES (?)", ('workout_weights_to_kg',))
        
        conn.commit()
        print(f"✅ Converted {converted_count} workout exercises from lbs to kg!")
        
        conn.close()
        
    except Exception as e:
        print(f"❌ Migration failed: {e}")
        raise

def run_migrations():
    """Run all database migrations."""
    print("Running database migrations...")
    migrate_workout_status()
    migrate_workout_weights_to_kg()
    print("✅ All migrations completed!")

if __name__ == "__main__":
    run_migrations() 