#pragma once
#include <string>

class Exercise {
public:
    int id;
    std::string name;
    std::string description;
    std::string path;

    Exercise(int _id, 
             const std::string& _name, 
             const std::string& _description,
             const std::string& _path) 
        : id(_id), 
          name(_name), 
          description(_description),
          path(_path) 
    {}
};