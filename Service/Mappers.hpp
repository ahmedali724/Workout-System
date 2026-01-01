#pragma once
#include "sqlite3.h"
#include "User.hpp"
#include "WorkoutPlan.hpp"
#include "CustomList.hpp"
#include "Message.hpp"
#include "Exercise.hpp"

namespace Mappers {
    User mapUser(sqlite3_stmt *s);
    WorkoutPlanItem mapWorkoutPlanItem(sqlite3_stmt *s);
    CustomList mapCustomList(sqlite3_stmt *s, const std::string& listName);
    Message mapMessage(sqlite3_stmt *s);
    Exercise mapExercise(sqlite3_stmt *s);
}

