#pragma once
#include <string>

class User {
public:
    int id;
    std::string username;
    std::string password;
    std::string email;
    std::string name;
    int age;
    std::string gender; // "Male" or "Female"
    std::string role;   // "Trainee", "Trainer", or "Admin"
    bool availability;

    // Constructors
    User(int _id, const std::string& _username, const std::string& _password,
         const std::string& _email, const std::string& _name,
         int _age, const std::string& _gender, const std::string& _role,bool _availability)
        : id(_id), username(_username), password(_password),
          email(_email), name(_name), age(_age),
          gender(_gender), role(_role), availability(_availability) {}
};
