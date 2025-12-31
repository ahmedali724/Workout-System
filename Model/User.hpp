#pragma once
#include <string>
#include <memory>

class User {
public:
    int id;
    std::string username;
    std::string password;
    std::string email;
    std::string name;
    int age;
    std::string gender;
    std::string role;
    bool availability;

    User(int _id, const std::string& _username, const std::string& _password,
         const std::string& _email, const std::string& _name,
         int _age, const std::string& _gender, const std::string& _role, bool _availability)
        : id(_id), username(_username), password(_password),
          email(_email), name(_name), age(_age),
          gender(_gender), role(_role), availability(_availability) {}

    virtual ~User() = default;
};

class Trainee : public User {
public:
    Trainee(int _id, const std::string& _username, const std::string& _password,
            const std::string& _email, const std::string& _name,
            int _age, const std::string& _gender, bool _availability = true)
        : User(_id, _username, _password, _email, _name, _age, _gender, "Trainee", _availability) {}
};

class Trainer : public User {
public:
    Trainer(int _id, const std::string& _username, const std::string& _password,
            const std::string& _email, const std::string& _name,
            int _age, const std::string& _gender, bool _availability = true)
        : User(_id, _username, _password, _email, _name, _age, _gender, "Trainer", _availability) {}
};

class Admin : public User {
public:
    Admin(int _id, const std::string& _username, const std::string& _password,
          const std::string& _email, const std::string& _name,
          int _age, const std::string& _gender, bool _availability = true)
        : User(_id, _username, _password, _email, _name, _age, _gender, "Admin", _availability) {}
};

inline User createUserFromData(int id, const std::string& username, const std::string& password,
                                const std::string& email, const std::string& name,
                                int age, const std::string& gender, 
                                const std::string& role, bool availability) {
    if (role == "Trainee") {
        return Trainee(id, username, password, email, name, age, gender, availability);
    } else if (role == "Trainer") {
        return Trainer(id, username, password, email, name, age, gender, availability);
    } else if (role == "Admin") {
        return Admin(id, username, password, email, name, age, gender, availability);
    }
    return User(id, username, password, email, name, age, gender, role, availability);
}
