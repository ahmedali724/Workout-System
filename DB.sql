CREATE TABLE IF NOT EXISTS Users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL,
    email TEXT UNIQUE NOT NULL,
    name TEXT NOT NULL,
    age INTEGER NOT NULL,
    gender TEXT NOT NULL CHECK(gender IN('Male','Female')),
    role TEXT NOT NULL CHECK(role IN ('Trainee', 'Trainer', 'Admin'))
);

CREATE TABLE IF NOT EXISTS Exercise (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    description TEXT DEFAULT '',
    path TEXT DEFAULT ''
);
