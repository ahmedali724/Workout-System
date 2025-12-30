#include "TrainerService.hpp"
#include <string>

std::vector<User> TrainerService::getAllTrainers() {
    struct UserData {
        int id;
        std::string username;
        std::string password;
        std::string email;
        std::string name;
        int age;
        std::string gender;
        std::string role;
        bool availability;
    };

    auto users = db.queryPrepared<UserData>(
        "SELECT u.id, u.username, u.password, u.email, u.name, u.age, u.gender, u.role, u.availability "
        "FROM User u "
        "INNER JOIN Trainer t ON u.id = t.user_id;",
        [](sqlite3_stmt *s) {
            return UserData{
                sqlite3_column_int(s, 0),
                (const char *)sqlite3_column_text(s, 1),
                (const char *)sqlite3_column_text(s, 2),
                (const char *)sqlite3_column_text(s, 3),
                (const char *)sqlite3_column_text(s, 4),
                sqlite3_column_int(s, 5),
                (const char *)sqlite3_column_text(s, 6),
                (const char *)sqlite3_column_text(s, 7),
                sqlite3_column_int(s, 8) != 0
            };
        });

    std::vector<User> result;
    for (auto& u : users) {
        result.push_back(createUserFromData(u.id, u.username, u.password, u.email, u.name, 
                                            u.age, u.gender, u.role, u.availability));
    }
    return result;
}

std::vector<User> TrainerService::searchTrainersByName(const std::string &name) {
    struct UserData {
        int id;
        std::string username;
        std::string password;
        std::string email;
        std::string name;
        int age;
        std::string gender;
        std::string role;
        bool availability;
    };

    auto users = db.queryPrepared<UserData>(
        "SELECT u.id, u.username, u.password, u.email, u.name, u.age, u.gender, u.role, u.availability "
        "FROM User u "
        "INNER JOIN Trainer t ON u.id = t.user_id "
        "WHERE u.name LIKE ?;",
        [](sqlite3_stmt *s) {
            return UserData{
                sqlite3_column_int(s, 0),
                (const char *)sqlite3_column_text(s, 1),
                (const char *)sqlite3_column_text(s, 2),
                (const char *)sqlite3_column_text(s, 3),
                (const char *)sqlite3_column_text(s, 4),
                sqlite3_column_int(s, 5),
                (const char *)sqlite3_column_text(s, 6),
                (const char *)sqlite3_column_text(s, 7),
                sqlite3_column_int(s, 8) != 0
            };
        },
        "%" + name + "%");

    std::vector<User> result;
    for (auto& u : users) {
        result.push_back(createUserFromData(u.id, u.username, u.password, u.email, u.name, 
                                            u.age, u.gender, u.role, u.availability));
    }
    return result;
}

std::vector<User> TrainerService::getMyTrainees(int trainerId) {
    struct UserData {
        int id;
        std::string username;
        std::string password;
        std::string email;
        std::string name;
        int age;
        std::string gender;
        std::string role;
        bool availability;
    };

    auto users = db.queryPrepared<UserData>(
        "SELECT u.id, u.username, u.password, u.email, u.name, u.age, u.gender, u.role, u.availability "
        "FROM User u "
        "INNER JOIN Trainee t ON u.id = t.user_id "
        "INNER JOIN TraineeTrainer tt ON t.user_id = tt.trainee_id "
        "INNER JOIN Trainer tr ON tt.trainer_id = tr.user_id "
        "WHERE tr.user_id = ?;",
        [](sqlite3_stmt *s) {
            return UserData{
                sqlite3_column_int(s, 0),
                (const char *)sqlite3_column_text(s, 1),
                (const char *)sqlite3_column_text(s, 2),
                (const char *)sqlite3_column_text(s, 3),
                (const char *)sqlite3_column_text(s, 4),
                sqlite3_column_int(s, 5),
                (const char *)sqlite3_column_text(s, 6),
                (const char *)sqlite3_column_text(s, 7),
                sqlite3_column_int(s, 8) != 0
            };
        },
        trainerId);

    std::vector<User> result;
    for (auto& u : users) {
        result.push_back(createUserFromData(u.id, u.username, u.password, u.email, u.name, 
                                            u.age, u.gender, u.role, u.availability));
    }
    return result;
}

std::optional<User> TrainerService::getTraineeProfile(int traineeId) {
    struct UserData {
        int id;
        std::string username;
        std::string password;
        std::string email;
        std::string name;
        int age;
        std::string gender;
        std::string role;
        bool availability;
    };

    auto users = db.queryPrepared<UserData>(
        "SELECT id, username, password, email, name, age, gender, role, availability "
        "FROM User WHERE id=?;",
        [](sqlite3_stmt *s) {
            return UserData{
                sqlite3_column_int(s, 0),
                (const char *)sqlite3_column_text(s, 1),
                (const char *)sqlite3_column_text(s, 2),
                (const char *)sqlite3_column_text(s, 3),
                (const char *)sqlite3_column_text(s, 4),
                sqlite3_column_int(s, 5),
                (const char *)sqlite3_column_text(s, 6),
                (const char *)sqlite3_column_text(s, 7),
                sqlite3_column_int(s, 8) != 0
            };
        },
        traineeId);
    
    if (!users.empty()) {
        auto& u = users[0];
        return createUserFromData(u.id, u.username, u.password, u.email, u.name, 
                                  u.age, u.gender, u.role, u.availability);
    }
    return std::nullopt;
}

int TrainerService::addWorkoutPlanItem(int traineeId, int trainerId, int exerciseId, int sets, int reps) {
    std::string sql = "INSERT OR REPLACE INTO TraineeAssignedPlan (trainee_id, trainer_id, exercise_id, sets, reps, completed) "
                      "VALUES (?, ?, ?, ?, ?, 0);";
    return db.executePrepared(sql, traineeId, trainerId, exerciseId, sets, reps);
}

int TrainerService::updateWorkoutPlanItem(int traineeId, int trainerId, int exerciseId, int sets, int reps) {
    std::string sql = "UPDATE TraineeAssignedPlan SET sets=?, reps=? "
                      "WHERE trainee_id=? AND trainer_id=? AND exercise_id=?;";
    return db.executePrepared(sql, sets, reps, traineeId, trainerId, exerciseId);
}

int TrainerService::deleteWorkoutPlanItem(int traineeId, int trainerId, int exerciseId) {
    std::string sql = "DELETE FROM TraineeAssignedPlan "
                      "WHERE trainee_id=? AND trainer_id=? AND exercise_id=?;";
    return db.executePrepared(sql, traineeId, trainerId, exerciseId);
}

std::vector<WorkoutPlanItem> TrainerService::getTraineeWorkoutPlan(int traineeId, int trainerId) {
    return db.queryPrepared<WorkoutPlanItem>(
        "SELECT tap.trainee_id, tap.trainer_id, tap.exercise_id, tap.sets, tap.reps, tap.completed, e.name "
        "FROM TraineeAssignedPlan tap "
        "INNER JOIN Exercise e ON tap.exercise_id = e.id "
        "WHERE tap.trainee_id=? AND tap.trainer_id=?;",
        [](sqlite3_stmt *s) {
            return WorkoutPlanItem{
                sqlite3_column_int(s, 0),
                sqlite3_column_int(s, 1),
                sqlite3_column_int(s, 2),
                sqlite3_column_int(s, 3),
                sqlite3_column_int(s, 4),
                sqlite3_column_int(s, 5) != 0,
                (const char *)sqlite3_column_text(s, 6)
            };
        },
        traineeId, trainerId);
}

int TrainerService::getTotalClients(int trainerId) {
    return db.scalarPrepared<int>(
        "SELECT COUNT(*) FROM TraineeTrainer WHERE trainer_id=?;",
        trainerId);
}