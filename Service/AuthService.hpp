#pragma once
#include <string>
#include "Database.hpp"
#include "User.hpp"
#include <optional>

class AuthService
{
private:
    DB& db; 

public:
    AuthService(DB& database) : db(database) {}

    int registerUser(User& newUser);
    std::optional<User> login(const std::string& username, const std::string& password);
    std::optional<User> getProfile(int userId);
    int updateProfile(User &user);
    int changePassword(int userId, const std::string& oldPassword, const std::string& newPassword);
};
