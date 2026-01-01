#include "TrainerService.hpp"
#include <string>
#include "Mappers.hpp"

std::vector<User> TrainerService::getAllTrainers() {
    return db.queryPrepared<User>(
        "SELECT u.id, u.username, u.password, u.email, u.name, u.age, u.gender, u.role, u.availability "
        "FROM User u "
        "INNER JOIN Trainer t ON u.id = t.user_id;",
        Mappers::mapUser);
}

std::vector<User> TrainerService::searchTrainersByName(const std::string &name) {
    return db.queryPrepared<User>(
        "SELECT u.id, u.username, u.password, u.email, u.name, u.age, u.gender, u.role, u.availability "
        "FROM User u "
        "INNER JOIN Trainer t ON u.id = t.user_id "
        "WHERE u.name LIKE ?;",
        Mappers::mapUser,
        "%" + name + "%");
}

std::vector<User> TrainerService::getMyTrainees(int trainerId) {
    return db.queryPrepared<User>(
        "SELECT u.id, u.username, u.password, u.email, u.name, u.age, u.gender, u.role, u.availability "
        "FROM User u "
        "INNER JOIN Trainee t ON u.id = t.user_id "
        "INNER JOIN TraineeTrainer tt ON t.user_id = tt.trainee_id "
        "INNER JOIN Trainer tr ON tt.trainer_id = tr.user_id "
        "WHERE tr.user_id = ?;",
        Mappers::mapUser,
        trainerId);
}

std::optional<User> TrainerService::getTraineeProfile(int traineeId) {
    auto users = db.queryPrepared<User>(
        "SELECT id, username, password, email, name, age, gender, role, availability "
        "FROM User WHERE id=?;",
        Mappers::mapUser,
        traineeId);
    
    if (!users.empty()) {
        return users[0];
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
        Mappers::mapWorkoutPlanItem,
        traineeId, trainerId);
}

int TrainerService::getTotalClients(int trainerId) {
    return db.scalarPrepared<int>(
        "SELECT COUNT(*) FROM TraineeTrainer WHERE trainer_id=?;",
        trainerId);
}