# Fitness Tracker Frontend

React frontend for the Fitness Tracker application.

## Features

- **Daily Journal View**: View and edit body composition and exercises for any date
- **Date Navigation**: Simple prev/next navigation with date picker
- **Body Composition Tracking**: Add weight and waist circumference measurements
- **Exercise Tracking**: Add exercises individually or in batch
- **Charts Page**: View analytics and trends (placeholder for now)
- **Responsive Design**: Desktop-first design that works on mobile

## Tech Stack

- **React 18** with TypeScript
- **Vite** for fast development and building
- **Tailwind CSS** for styling
- **React Router** for navigation
- **date-fns** for date manipulation

## Development

### Prerequisites

- Node.js 18 or higher
- Backend server running on `http://localhost:8000`

### Setup

1. **Install dependencies**:
   ```bash
   npm install
   ```

2. **Start the development server**:
   ```bash
   npm run dev
   ```

The frontend will be available at `http://localhost:3000`

### Build

```bash
npm run build
```

### Linting

```bash
npm run lint
```

## Project Structure

```
src/
├── components/
│   ├── Layout/           # Layout components
│   │   ├── Header.tsx    # Navigation header
│   │   └── Layout.tsx    # Main layout wrapper
│   └── Journal/          # Journal-specific components
│       ├── DateNavigation.tsx      # Date navigation
│       ├── DailyEntryView.tsx      # Daily entry display
│       ├── BodyCompositionForm.tsx # Body comp form
│       ├── ExerciseForm.tsx        # Single exercise form
│       └── BatchExerciseForm.tsx   # Batch exercise form
├── pages/
│   ├── Journal.tsx       # Main journal page
│   └── Charts.tsx        # Analytics page
├── api/
│   └── client.ts         # API client
├── types/
│   └── index.ts          # TypeScript types
└── ...
```

## API Integration

The frontend communicates with the Python backend via REST API calls. The API client (`src/api/client.ts`) provides methods for:

- Body composition CRUD operations
- Exercise and workout management
- Journal data retrieval
- Analytics and charts

## Development Notes

- The backend must be running for the frontend to work
- API calls are proxied through Vite to avoid CORS issues
- All forms include validation and error handling
- The UI is designed to be desktop-first but responsive 