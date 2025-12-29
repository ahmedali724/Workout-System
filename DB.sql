PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS "User" (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL,
    email TEXT UNIQUE NOT NULL,
    name TEXT NOT NULL,
    age INTEGER NOT NULL,
    gender TEXT NOT NULL CHECK(gender IN ('Male', 'Female')),
    role TEXT NOT NULL CHECK(role IN ('Trainee', 'Trainer', 'Admin')),
    availability INTEGER DEFAULT 1 CHECK(availability IN (0, 1))
);

CREATE TABLE IF NOT EXISTS Exercise (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    description TEXT DEFAULT '',
    path TEXT DEFAULT ''
);

CREATE TABLE IF NOT EXISTS Message (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    sender_id INTEGER NOT NULL,
    receiver_id INTEGER NOT NULL,
    content TEXT NOT NULL,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(sender_id) REFERENCES "User"(id) ON DELETE CASCADE,
    FOREIGN KEY(receiver_id) REFERENCES "User"(id) ON DELETE CASCADE
);


CREATE TABLE IF NOT EXISTS UserWorkout (

    user_id INTEGER NOT NULL,
    exercise_id INTEGER NOT NULL,
    FOREIGN KEY(user_id) REFERENCES "User"(id) ON DELETE CASCADE,
    FOREIGN KEY(exercise_id) REFERENCES Exercise(id) ON DELETE CASCADE,
    PRIMARY KEY(user_id, exercise_id)

);


CREATE TABLE IF NOT EXISTS TraineeAssignedPlan (
    trainee_id INTEGER NOT NULL,
    trainer_id INTEGER NOT NULL,
    exercise_id INTEGER NOT NULL,
    sets INTEGER DEFAULT 3,
    reps INTEGER DEFAULT 10,
    completed INTEGER DEFAULT 0 CHECK(completed IN (0, 1)),
    FOREIGN KEY(trainee_id) REFERENCES "User"(id) ON DELETE CASCADE,
    FOREIGN KEY(trainer_id) REFERENCES "User"(id) ON DELETE CASCADE,
    FOREIGN KEY(exercise_id) REFERENCES Exercise(id) ON DELETE CASCADE,
    PRIMARY KEY(trainee_id, trainer_id,exercise_id)
);


CREATE TABLE IF NOT EXISTS TraineeTrainer (

    trainee_id INTEGER NOT NULL,
    trainer_id INTEGER NOT NULL,

    FOREIGN KEY(trainee_id) REFERENCES "User"(id) ON DELETE CASCADE,
    FOREIGN KEY(trainer_id) REFERENCES "User"(id) ON DELETE CASCADE,
    PRIMARY KEY(trainee_id, trainer_id)

);




