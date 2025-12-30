#include "AuthService.hpp"

int AuthService::registerUser(User &newUser)
{
    std::string sql = "INSERT INTO User (username, password, email, name, age, gender, role, availability) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    int result = db.executePrepared(sql,
                                    newUser.username,
                                    newUser.password,
                                    newUser.email,
                                    newUser.name,
                                    newUser.age,
                                    newUser.gender,
                                    newUser.role,
                                    (int)newUser.availability);

    if (result > 0 && (newUser.role == "Trainee" || newUser.role == "Trainer")) {
        int userId = db.scalarPrepared<int>("SELECT last_insert_rowid();");
        if (userId > 0) {
            if (newUser.role == "Trainee") {
                std::string traineeSql = "INSERT INTO Trainee (user_id) VALUES (?);";
                db.executePrepared(traineeSql, userId);
            } else if (newUser.role == "Trainer") {
                std::string trainerSql = "INSERT INTO Trainer (user_id) VALUES (?);";
                db.executePrepared(trainerSql, userId);
            }
        }
    }

    return result;
}

std::optional<User> AuthService::login(const std::string &username, const std::string &password)
{
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
        "FROM User WHERE username=? AND password=?;",
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
        username, password);

    if (!users.empty()) {
        auto& u = users[0];
        return createUserFromData(u.id, u.username, u.password, u.email, u.name, 
                                  u.age, u.gender, u.role, u.availability);
    }

    return std::nullopt;
}

std::optional<User> AuthService::getProfile(int userId)
{
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
        userId);

    if (!users.empty()) {
        auto& u = users[0];
        return createUserFromData(u.id, u.username, u.password, u.email, u.name, 
                                  u.age, u.gender, u.role, u.availability);
    }

    return std::nullopt;
}

int AuthService::updateProfile(User &user)
{
    std::string sql = "UPDATE User SET username=?, password=?, email=?, name=?, age=?, gender=?, role=?, availability=? "
                      "WHERE id=?;";

    return db.executePrepared(sql,
                              user.username,
                              user.password,
                              user.email,
                              user.name,
                              user.age,
                              user.gender,
                              user.role,
                              (int)user.availability,
                              user.id);
}

int AuthService::changePassword(int userId, const std::string& oldPassword, const std::string& newPassword)
{
    auto userOpt = getProfile(userId);
    if (!userOpt.has_value() || userOpt.value().password != oldPassword)
        return -1;
    
    std::string sql = "UPDATE User SET password=? WHERE id=?;";
    return db.executePrepared(sql, newPassword, userId);
}
