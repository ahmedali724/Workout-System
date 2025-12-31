INSERT INTO "User" (username, password, email, name, age, gender, role)
VALUES
-- Trainees
('trainee1', 'pass123', 't1@mail.com', 'Ahmed Ali', 22, 'Male', 'Trainee'),
('trainee2', 'pass123', 't2@mail.com', 'Sara Mohamed', 21, 'Female', 'Trainee'),

-- Trainers
('trainer1', 'pass123', 'tr1@mail.com', 'Coach Omar', 30, 'Male', 'Trainer'),
('trainer2', 'pass123', 'tr2@mail.com', 'Coach Lina', 28, 'Female', 'Trainer'),

-- Admin
('admin1', 'adminpass', 'admin@mail.com', 'System Admin', 35, 'Male', 'Admin');



-- Trainees
INSERT INTO Trainee (user_id)
SELECT id FROM "User" WHERE role = 'Trainee';

-- Trainers
INSERT INTO Trainer (user_id)
SELECT id FROM "User" WHERE role = 'Trainer';


INSERT INTO TraineeTrainer (trainee_id, trainer_id)
VALUES
(
    (SELECT id FROM "User" WHERE username = 'trainee1'),
    (SELECT id FROM "User" WHERE username = 'trainer1')
),
(
    (SELECT id FROM "User" WHERE username = 'trainee2'),
    (SELECT id FROM "User" WHERE username = 'trainer2')
);


INSERT INTO TraineeAssignedPlan
(trainee_id, trainer_id, exercise_id, sets, reps, completed)
VALUES
(
    (SELECT id FROM "User" WHERE username = 'trainee1'),
    (SELECT id FROM "User" WHERE username = 'trainer1'),
    1, 3, 12, 0
),
(
    (SELECT id FROM "User" WHERE username = 'trainee1'),
    (SELECT id FROM "User" WHERE username = 'trainer1'),
    2, 4, 10, 1
),
(
    (SELECT id FROM "User" WHERE username = 'trainee2'),
    (SELECT id FROM "User" WHERE username = 'trainer2'),
    3, 3, 15, 0
);


INSERT INTO UserWorkout (user_id, exercise_id)
VALUES
(
    (SELECT id FROM "User" WHERE username = 'trainee1'),
    2
),
(
    (SELECT id FROM "User" WHERE username = 'trainee2'),
    3
);


INSERT INTO Message (sender_id, receiver_id, content)
VALUES
(
    (SELECT id FROM "User" WHERE username = 'trainer1'),
    (SELECT id FROM "User" WHERE username = 'trainee1'),
    'Great progress! Increase reps next session.'
),
(
    (SELECT id FROM "User" WHERE username = 'trainee1'),
    (SELECT id FROM "User" WHERE username = 'trainer1'),
    'Thanks coach!'
);


