#pragma once
#include "Database.hpp"
#include "User.hpp"
#include <vector>
#include <optional>

class AdminService {
    DB &db;
    
public:
    AdminService(DB &database) : db(database) {}
    
    std::vector<User> getAllUsers();
    std::vector<User> searchUsersByName(const std::string &name);
    std::optional<User> getUserById(int userId);
    int updateUser(User &user);
    int deleteUser(int userId);
    int createAdmin(User &newAdmin);
    int getTotalUsersByRole(const std::string &role);
    int getTotalTraineeTrainerPairs();
    
    struct ReportData {
        int totalTrainees;
        int totalTrainers;
        int totalAdmins;
        int totalPairs;
    };
    
    ReportData getReportData();
};


