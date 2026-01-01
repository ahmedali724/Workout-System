#include "Mappers.hpp"

namespace Mappers {
    User mapUser(sqlite3_stmt *s) {
        return createUserFromData(
            sqlite3_column_int(s, 0),
            (const char *)sqlite3_column_text(s, 1),
            (const char *)sqlite3_column_text(s, 2),
            (const char *)sqlite3_column_text(s, 3),
            (const char *)sqlite3_column_text(s, 4),
            sqlite3_column_int(s, 5),
            (const char *)sqlite3_column_text(s, 6),
            (const char *)sqlite3_column_text(s, 7),
            sqlite3_column_int(s, 8) != 0
        );
    }

    WorkoutPlanItem mapWorkoutPlanItem(sqlite3_stmt *s) {
        return WorkoutPlanItem{
            sqlite3_column_int(s, 0),
            sqlite3_column_int(s, 1),
            sqlite3_column_int(s, 2),
            sqlite3_column_int(s, 3),
            sqlite3_column_int(s, 4),
            sqlite3_column_int(s, 5) != 0,
            (const char *)sqlite3_column_text(s, 6)
        };
    }

    CustomList mapCustomList(sqlite3_stmt *s, const std::string& listName) {
        return CustomList{
            sqlite3_column_int(s, 0),
            sqlite3_column_int(s, 1),
            listName,
            (const char *)sqlite3_column_text(s, 3)
        };
    }

    Message mapMessage(sqlite3_stmt *s) {
        return Message{
            sqlite3_column_int(s, 0),
            sqlite3_column_int(s, 1),
            sqlite3_column_int(s, 2),
            (const char *)sqlite3_column_text(s, 3),
            (const char *)sqlite3_column_text(s, 4),
            (const char *)sqlite3_column_text(s, 5),
            (const char *)sqlite3_column_text(s, 6)
        };
    }

    Exercise mapExercise(sqlite3_stmt *s) {
        return Exercise{
            sqlite3_column_int(s, 0),
            (const char *)sqlite3_column_text(s, 1),
            (const char *)sqlite3_column_text(s, 2),
            (const char *)sqlite3_column_text(s, 3)
        };
    }
}

