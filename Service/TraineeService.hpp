#pragma once
#include "Database.hpp"
#include "User.hpp"
#include "WorkoutPlan.hpp"
#include "CustomList.hpp"
#include <vector>
#include <optional>

class TraineeService {
    DB &db;
    
public:
    TraineeService(DB &database) : db(database) {}
    
    std::optional<User> getAssignedTrainer(int traineeId);
    int leaveTrainer(int traineeId, int trainerId);
    int joinTrainer(int traineeId, int trainerId);
    std::vector<WorkoutPlanItem> getCurrentWorkoutPlan(int traineeId);
    int logCompletedWorkout(int traineeId, int trainerId, int exerciseId);
    std::vector<CustomList> getCustomListExercises(int userId, const std::string &listName);
    int addExerciseToCustomList(int userId, int exerciseId, const std::string &listName);
    int removeExerciseFromCustomList(int userId, int exerciseId, const std::string &listName);
    int deleteCustomList(int userId, const std::string &listName);
    bool exportWorkoutPlanToFile(int traineeId, const std::string &filename);
};


