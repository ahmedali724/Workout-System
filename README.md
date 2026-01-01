# FitnessGo - Workout System

A comprehensive terminal-based fitness management system built with C++ that allows trainers, trainees, and admins to manage workout plans, exercises, and fitness programs.

## Overview

FitnessGo is a multi-user fitness management platform designed to streamline workout planning and tracking. The application features a text-based user interface (TUI) with role-based access control for different user types: Admins, Trainers, and Trainees.

## Features

### User Management
- **Role-Based Access Control**: Three distinct user roles with specific permissions
  - **Admin**: Full system management and user administration
  - **Trainer**: Create and manage workout plans for trainees
  - **Trainee**: View assigned workout plans and track progress
- **Authentication**: Secure login system with username and password verification
- **User Profiles**: Manage user information including age, gender, email, and contact details

### Exercise Management
- **Exercise Library**: Comprehensive database of exercises with descriptions
- **ASCII Animations**: Visual demonstrations of exercises with ASCII frame animations
- **Exercise Organization**: Categorized exercises including:
  - Push-ups
  - Squats
  - Planks
  - Jumping Jacks
  - And more...

### Workout Planning
- **Plan Creation**: Trainers can create customized workout plans
- **Exercise Assignment**: Assign exercises to specific workout sessions
- **Plan Management**: Modify and track workout plans

### Communication
- **Messaging System**: Internal messaging between trainers and trainees
- **Notifications**: Keep users updated on workout assignments and changes

## Project Structure

```
Workout-System/
├── Model/                 # Data models
│   ├── User.hpp
│   ├── Exercise.hpp
│   ├── WorkoutPlan.hpp
│   ├── Message.hpp
│   └── CustomList.hpp
├── Service/              # Business logic layer
│   ├── AuthService.cpp/hpp
│   ├── ExerciseService.cpp/hpp
│   ├── TrainerService.cpp/hpp
│   ├── TraineeService.cpp/hpp
│   ├── MessageService.cpp/hpp
│   ├── AdminService.cpp/hpp
│   └── Mappers.cpp/hpp
├── UI/                   # User interface layer
│   ├── AuthUI.hpp
│   ├── ExerciseUI.hpp
│   ├── TrainerUI.hpp
│   ├── TraineeUI.hpp
│   ├── AdminUI.hpp
│   └── UIHelpers.cpp/hpp
├── Utils/               # Utility functions and database
│   ├── tuiUtils.cpp/hpp
│   ├── Database.hpp
│   └── sqlite3.c/h
├── ASCIIFrames/        # Exercise animation frames
├── CMakeLists.txt      # Build configuration
├── main.cpp            # Application entry point
├── DB.sql              # Database schema
└── Seed.sql            # Initial data
```

## Technology Stack

- **Language**: C++17
- **Build System**: CMake
- **Database**: SQLite3
- **UI Framework**: Custom Terminal UI (TUI)
- **Platform**: Windows (with multimedia support)

## Building the Project

### Prerequisites
- CMake 3.10 or higher
- C++17 compatible compiler
- Windows OS (for multimedia features)

### Build Instructions

1. Clone the repository:
```bash
git clone <repository-url>
cd Workout-System
```

2. Create a build directory and compile:
```bash
mkdir build
cd build
cmake ..
make
```

3. Run the application:
```bash
./FitnessGo
```

## Database

The application uses SQLite3 for persistent data storage. The database includes:
- User accounts and profiles
- Exercise library
- Workout plans and assignments
- Messages between users
- User activity logs

### Database Setup

The database schema is defined in [DB.sql](DB.sql) and initial seed data is in [Seed.sql](Seed.sql).

## Usage

### Starting the Application
1. Run `FitnessGo.exe`
2. The intro screen will display with audio
3. Log in with your credentials

### For Admins
- Manage user accounts
- Monitor system activity
- Configure system settings

### For Trainers
- Create and manage workout plans
- Assign exercises to trainees
- Send messages to trainees
- Track trainee progress

### For Trainees
- View assigned workout plans
- Browse exercise library with animations
- Receive messages from trainers
- Track completed exercises

## Features Highlights

### ASCII Exercise Animations
The application includes ASCII frame animations for exercises to help users understand proper form. Animations are stored in the `ASCIIFrames/` directory with individual frames for each exercise.

### Terminal User Interface
- Clean, intuitive menu-driven interface
- Real-time user feedback
- Colored text output for better readability
- Responsive controls

### Security
- Password-protected accounts
- Role-based authorization
- Secure authentication service

## File Structure Details

- **Model**: Contains data structure definitions for core entities
- **Service**: Implements business logic and database operations
- **UI**: Manages user interface and input/output
- **Utils**: Helper functions and database connectivity

## Configuration

The application loads configuration from:
- SQLite database (`fitnessGo.db`)
- Environment variables (if applicable)
- ASCII frame files for exercise animations

## Future Enhancements

- Progress tracking analytics
- Workout history and statistics
- Mobile app integration
- Advanced scheduling features
- Social features and community challenges
- Nutrition tracking
