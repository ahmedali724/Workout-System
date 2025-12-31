#pragma once
#include "Database.hpp"
#include "User.hpp"
#include "WorkoutPlan.hpp"
#include <vector>
#include <optional>

class TrainerService {
    DB &db;
    
public:
    TrainerService(DB &database) : db(database) {}
    
    std::vector<User> getAllTrainers();
    std::vector<User> searchTrainersByName(const std::string &name);
    std::vector<User> getMyTrainees(int trainerId);
    std::optional<User> getTraineeProfile(int traineeId);
    int addWorkoutPlanItem(int traineeId, int trainerId, int exerciseId, int sets, int reps);
    int updateWorkoutPlanItem(int traineeId, int trainerId, int exerciseId, int sets, int reps);
    int deleteWorkoutPlanItem(int traineeId, int trainerId, int exerciseId);
    std::vector<WorkoutPlanItem> getTraineeWorkoutPlan(int traineeId, int trainerId);
    int getTotalClients(int trainerId);
};


