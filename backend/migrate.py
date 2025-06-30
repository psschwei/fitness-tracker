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

def run_migrations():
    """Run all database migrations."""
    print("Running database migrations...")
    migrate_workout_status()
    print("✅ All migrations completed!")

if __name__ == "__main__":
    run_migrations() 