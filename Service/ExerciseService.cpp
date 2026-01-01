#include "ExerciseService.hpp"
#include "Mappers.hpp"

ExerciseService::ExerciseService(DB &database) : db(database) {}

std::vector<Exercise> ExerciseService::getAllExercises() {
    return db.queryPrepared<Exercise>(
        "SELECT id, name, description, path FROM Exercise;",
        Mappers::mapExercise);
}

std::optional<Exercise> ExerciseService::getExerciseById(int exerciseId) {
    auto exercises = db.queryPrepared<Exercise>(
        "SELECT id, name, description, path FROM Exercise WHERE id=?;",
        Mappers::mapExercise,
        exerciseId
    );

    if (!exercises.empty())
        return exercises[0];
    return std::nullopt;
}

std::vector<Exercise> ExerciseService::searchExercisesByName(const std::string &name) {
    return db.queryPrepared<Exercise>(
        "SELECT id, name, description, path FROM Exercise WHERE name LIKE ?;",
        Mappers::mapExercise,
        "%" + name + "%"
    );
}
