#include "AuthService.hpp"
#include "Mappers.hpp"

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
    auto users = db.queryPrepared<User>(
        "SELECT id, username, password, email, name, age, gender, role, availability "
        "FROM User WHERE username=? AND password=?;",
        Mappers::mapUser,
        username, password);

    if (!users.empty()) {
        return users[0];
    }

    return std::nullopt;
}

std::optional<User> AuthService::getProfile(int userId)
{
    auto users = db.queryPrepared<User>(
        "SELECT id, username, password, email, name, age, gender, role, availability "
        "FROM User WHERE id=?;",
        Mappers::mapUser,
        userId);

    if (!users.empty()) {
        return users[0];
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
