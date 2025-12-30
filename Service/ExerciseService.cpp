#include "ExerciseService.hpp"

ExerciseService::ExerciseService(DB &database) : db(database) {}

std::vector<Exercise> ExerciseService::getAllExercises() {
    return db.queryPrepared<Exercise>(
        "SELECT id, name, description, path FROM Exercise;",
        [](sqlite3_stmt *s) {
            return Exercise{
                sqlite3_column_int(s, 0),
                (const char *)sqlite3_column_text(s, 1),
                (const char *)sqlite3_column_text(s, 2),
                (const char *)sqlite3_column_text(s, 3)
            };
        });
}

std::optional<Exercise> ExerciseService::getExerciseById(int exerciseId) {
    auto exercises = db.queryPrepared<Exercise>(
        "SELECT id, name, description, path FROM Exercise WHERE id=?;",
        [](sqlite3_stmt *s) {
            return Exercise{
                sqlite3_column_int(s, 0),
                (const char *)sqlite3_column_text(s, 1),
                (const char *)sqlite3_column_text(s, 2),
                (const char *)sqlite3_column_text(s, 3)
            };
        },
        exerciseId
    );

    if (!exercises.empty())
        return exercises[0];
    return std::nullopt;
}

std::vector<Exercise> ExerciseService::searchExercisesByName(const std::string &name) {
    return db.queryPrepared<Exercise>(
        "SELECT id, name, description, path FROM Exercise WHERE name LIKE ?;",
        [](sqlite3_stmt *s) {
            return Exercise{
                sqlite3_column_int(s, 0),
                (const char *)sqlite3_column_text(s, 1),
                (const char *)sqlite3_column_text(s, 2),
                (const char *)sqlite3_column_text(s, 3)
            };
        },
        "%" + name + "%"
    );
}
