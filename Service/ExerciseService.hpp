#pragma once
#include "Database.hpp"
#include <optional>
#include <vector>
#include <string>
#include "Exercise.hpp"

class ExerciseService {
    DB &db;

public:
    ExerciseService(DB &database);

    std::vector<Exercise> getAllExercises();
    std::optional<Exercise> getExerciseById(int exerciseId);
    std::vector<Exercise> searchExercisesByName(const std::string &name);

};
