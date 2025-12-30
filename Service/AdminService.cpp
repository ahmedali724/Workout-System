#include "AdminService.hpp"
#include <string>

std::vector<User> AdminService::getAllUsers() {
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
        "FROM User ORDER BY role, name;",
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
        });

    std::vector<User> result;
    for (auto& u : users) {
        result.push_back(createUserFromData(u.id, u.username, u.password, u.email, u.name, 
                                            u.age, u.gender, u.role, u.availability));
    }
    return result;
}

std::vector<User> AdminService::searchUsersByName(const std::string &name) {
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
        "FROM User WHERE name LIKE ? ORDER BY role, name;",
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
        "%" + name + "%");

    std::vector<User> result;
    for (auto& u : users) {
        result.push_back(createUserFromData(u.id, u.username, u.password, u.email, u.name, 
                                            u.age, u.gender, u.role, u.availability));
    }
    return result;
}

std::optional<User> AdminService::getUserById(int userId) {
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

int AdminService::updateUser(User &user) {
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

int AdminService::deleteUser(int userId) {
    std::string sql = "DELETE FROM User WHERE id=?;";
    return db.executePrepared(sql, userId);
}

int AdminService::createAdmin(User &newAdmin) {
    std::string sql = "INSERT INTO User (username, password, email, name, age, gender, role, availability) "
                      "VALUES (?, ?, ?, ?, ?, ?, 'Admin', ?);";
    
    return db.executePrepared(sql,
                              newAdmin.username,
                              newAdmin.password,
                              newAdmin.email,
                              newAdmin.name,
                              newAdmin.age,
                              newAdmin.gender,
                              (int)newAdmin.availability);
}

int AdminService::getTotalUsersByRole(const std::string &role) {
    return db.scalarPrepared<int>(
        "SELECT COUNT(*) FROM User WHERE role=?;",
        role);
}

int AdminService::getTotalTraineeTrainerPairs() {
    return db.scalarPrepared<int>(
        "SELECT COUNT(*) FROM TraineeTrainer;");
}

AdminService::ReportData AdminService::getReportData() {
    ReportData data;
    data.totalTrainees = getTotalUsersByRole("Trainee");
    data.totalTrainers = getTotalUsersByRole("Trainer");
    data.totalAdmins = getTotalUsersByRole("Admin");
    data.totalPairs = getTotalTraineeTrainerPairs();
    return data;
}