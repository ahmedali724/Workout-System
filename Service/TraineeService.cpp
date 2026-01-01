#include "TraineeService.hpp"
#include <fstream>
#include "Mappers.hpp"

std::optional<User> TraineeService::getAssignedTrainer(int traineeId) {
    auto users = db.queryPrepared<User>(
        "SELECT u.id, u.username, u.password, u.email, u.name, u.age, u.gender, u.role, u.availability "
        "FROM User u "
        "INNER JOIN Trainer t ON u.id = t.user_id "
        "INNER JOIN TraineeTrainer tt ON t.user_id = tt.trainer_id "
        "INNER JOIN Trainee tr ON tt.trainee_id = tr.user_id "
        "WHERE tr.user_id = ?;",
        Mappers::mapUser,
        traineeId);
    
    if (!users.empty()) {
        return users[0];
    }
    return std::nullopt;
}

int TraineeService::leaveTrainer(int traineeId, int trainerId) {
    std::string sql = "DELETE FROM TraineeTrainer WHERE trainee_id=? AND trainer_id=?;";
    std::string sql2 = "DELETE FROM TraineeAssignedPlan WHERE trainee_id=? AND trainer_id=?;";
    std::string sql3 = "DELETE FROM Message WHERE sender_id=? AND receiver_id=?;";
    db.executePrepared(sql2,traineeId, trainerId);
    db.executePrepared(sql3,traineeId, trainerId);
    db.executePrepared(sql3,trainerId,traineeId);
    return db.executePrepared(sql, traineeId, trainerId);
}

int TraineeService::joinTrainer(int traineeId, int trainerId){
    std::string sql = "INSERT INTO TraineeTrainer Values(?, ?);";
    return db.executePrepared(sql, traineeId, trainerId);
}

std::vector<WorkoutPlanItem> TraineeService::getCurrentWorkoutPlan(int traineeId) {
    return db.queryPrepared<WorkoutPlanItem>(
        "SELECT tap.trainee_id, tap.trainer_id, tap.exercise_id, tap.sets, tap.reps, tap.completed, e.name "
        "FROM TraineeAssignedPlan tap "
        "INNER JOIN Exercise e ON tap.exercise_id = e.id "
        "WHERE tap.trainee_id=?;",
        Mappers::mapWorkoutPlanItem,
        traineeId);
}

int TraineeService::logCompletedWorkout(int traineeId, int trainerId, int exerciseId) {
    std::string sql = "UPDATE TraineeAssignedPlan SET completed=1 "
                      "WHERE trainee_id=? AND trainer_id=? AND exercise_id=?;";
    return db.executePrepared(sql, traineeId, trainerId, exerciseId);
}

std::vector<CustomList> TraineeService::getCustomListExercises(int userId, const std::string &listName) {
    return db.queryPrepared<CustomList>(
        "SELECT uw.user_id, uw.exercise_id, ?, e.name "
        "FROM UserWorkout uw "
        "INNER JOIN Exercise e ON uw.exercise_id = e.id "
        "WHERE uw.user_id=?;",
        [&listName](sqlite3_stmt *s) {
            return Mappers::mapCustomList(s, listName);
        },
        listName, userId);
}

int TraineeService::addExerciseToCustomList(int userId, int exerciseId, const std::string &listName) {
    std::string sql = "INSERT OR IGNORE INTO UserWorkout (user_id, exercise_id) VALUES (?, ?);";
    return db.executePrepared(sql, userId, exerciseId);
}

int TraineeService::removeExerciseFromCustomList(int userId, int exerciseId, const std::string &listName) {
    std::string sql = "DELETE FROM UserWorkout WHERE user_id=? AND exercise_id=?;";
    return db.executePrepared(sql, userId, exerciseId);
}

int TraineeService::deleteCustomList(int userId, const std::string &listName) {
    std::string sql = "DELETE FROM UserWorkout WHERE user_id=?;";
    return db.executePrepared(sql, userId);
}

bool TraineeService::exportWorkoutPlanToFile(int traineeId, const std::string &filename) {
    auto plan = getCurrentWorkoutPlan(traineeId);
    
    std::ofstream file(filename);
    if (!file.is_open())
        return false;
    
    file << "=== Workout Plan ===\n\n";
    
    for (const auto &item : plan) {
        file << "Exercise: " << item.exercise_name << "\n";
        file << "Sets: " << item.sets << "\n";
        file << "Reps: " << item.reps << "\n";
        file << "Status: " << (item.completed ? "Completed" : "Pending") << "\n";
        file << "---\n";
    }
    
    file.close();
    return true;
}
