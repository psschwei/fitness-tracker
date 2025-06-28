#!/bin/bash

# Fitness Tracker Application Launcher
# This script starts both the backend and frontend services and opens the browser

set -e  # Exit on any error

echo "🚀 Starting Fitness Tracker Application..."

# Function to cleanup background processes on exit
cleanup() {
    echo "🛑 Shutting down services..."
    if [ ! -z "$BACKEND_PID" ]; then
        kill $BACKEND_PID 2>/dev/null || true
    fi
    if [ ! -z "$FRONTEND_PID" ]; then
        kill $FRONTEND_PID 2>/dev/null || true
    fi
    exit 0
}

# Set up signal handlers for cleanup
trap cleanup SIGINT SIGTERM

# Check if we're in the right directory
if [ ! -f "pyproject.toml" ] || [ ! -d "frontend" ]; then
    echo "❌ Error: Please run this script from the fitness-tracker root directory"
    exit 1
fi

# Check if uv is available
if ! command -v uv &> /dev/null; then
    echo "❌ Error: uv is not installed. Please install uv first."
    exit 1
fi

# Check if Node.js is available
if ! command -v node &> /dev/null; then
    echo "❌ Error: Node.js is not installed. Please install Node.js first."
    exit 1
fi

# Check if npm is available
if ! command -v npm &> /dev/null; then
    echo "❌ Error: npm is not installed. Please install npm first."
    exit 1
fi

echo "📦 Installing dependencies..."

# Install Python dependencies
echo "   Installing Python dependencies..."
uv sync --quiet

# Install frontend dependencies
echo "   Installing frontend dependencies..."
cd frontend
npm install --silent
cd ..

echo "🔧 Starting services..."

# Start backend in background
echo "   Starting backend server..."
uv run python -m backend.main &
BACKEND_PID=$!

# Wait a moment for backend to start
sleep 3

# Check if backend is running
if ! curl -s http://localhost:8000/api/health > /dev/null 2>&1; then
    echo "   Waiting for backend to be ready..."
    sleep 2
fi

# Start frontend in background
echo "   Starting frontend server..."
cd frontend
npm run dev &
FRONTEND_PID=$!
cd ..

# Wait for frontend to be ready
echo "   Waiting for frontend to be ready..."
sleep 5

# Check if frontend is running
for i in {1..10}; do
    if curl -s http://localhost:3000 > /dev/null 2>&1; then
        break
    fi
    sleep 1
done

echo "✅ Services started successfully!"
echo ""
echo "🌐 Opening application in browser..."
echo "   Frontend: http://localhost:3000"
echo "   Backend API: http://localhost:8000"
echo "   API Docs: http://localhost:8000/docs"
echo ""
echo "Press Ctrl+C to stop all services"

# Open browser (try different commands for different OS)
if command -v xdg-open &> /dev/null; then
    # Linux
    xdg-open http://localhost:3000 &
elif command -v open &> /dev/null; then
    # macOS
    open http://localhost:3000 &
elif command -v start &> /dev/null; then
    # Windows
    start http://localhost:3000 &
else
    echo "   Please open http://localhost:3000 in your browser"
fi

# Wait for user to stop the application
wait 