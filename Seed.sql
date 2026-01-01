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

-- Exercises
INSERT INTO "main"."Exercise" ("id", "name", "description", "path") VALUES (1, 'Push-Up', 'A basic upper body exercise focusing on chest and triceps.', 'exercises/pushup.mp4');
INSERT INTO "main"."Exercise" ("id", "name", "description", "path") VALUES (2, 'Squat', 'Lower body exercise targeting quadriceps, hamstrings, and glutes.', 'exercises/squat.mp4');
INSERT INTO "main"."Exercise" ("id", "name", "description", "path") VALUES (3, 'Plank', 'Core-strengthening exercise maintaining a push-up position.', 'exercises/plank.mp4');
INSERT INTO "main"."Exercise" ("id", "name", "description", "path") VALUES (4, 'Lunge', 'Lower body exercise for legs and glutes, performed by stepping forward.', 'exercises/lunge.mp4');
INSERT INTO "main"."Exercise" ("id", "name", "description", "path") VALUES (5, 'Burpee', 'Full body exercise combining squat, push-up, and jump.', 'exercises/burpee.mp4');
INSERT INTO "main"."Exercise" ("id", "name", "description", "path") VALUES (6, 'Crunch', 'Abdominal exercise focusing on core strength.', 'exercises/crunch.mp4');
INSERT INTO "main"."Exercise" ("id", "name", "description", "path") VALUES (7, 'Mountain Climber', 'Cardio-core exercise simulating climbing movement.', 'exercises/mountain_climber.mp4');
INSERT INTO "main"."Exercise" ("id", "name", "description", "path") VALUES (8, 'Jumping Jack', 'Full body cardio exercise involving jumping and arm movements.', 'exercises/jumping_jack.mp4');
INSERT INTO "main"."Exercise" ("id", "name", "description", "path") VALUES (9, 'Bicep Curl', 'Arm exercise targeting the biceps using dumbbells.', 'exercises/bicep_curl.mp4');
INSERT INTO "main"."Exercise" ("id", "name", "description", "path") VALUES (10, 'Tricep Dip', 'Upper body exercise targeting triceps using a bench or chair.', 'exercises/tricep_dip.mp4');



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



