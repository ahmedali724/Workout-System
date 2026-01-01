#include "AdminService.hpp"
#include <string>
#include "Mappers.hpp"

std::vector<User> AdminService::getAllUsers() {
    return db.queryPrepared<User>(
        "SELECT id, username, password, email, name, age, gender, role, availability "
        "FROM User ORDER BY role, name;",
        Mappers::mapUser);
}

std::vector<User> AdminService::searchUsersByName(const std::string &name) {
    return db.queryPrepared<User>(
        "SELECT id, username, password, email, name, age, gender, role, availability "
        "FROM User WHERE name LIKE ? ORDER BY role, name;",
        Mappers::mapUser,
        "%" + name + "%");
}

std::optional<User> AdminService::getUserById(int userId) {
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