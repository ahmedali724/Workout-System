#pragma once
#include <string>

struct CustomList {
    int user_id;
    int exercise_id;
    std::string list_name;
    std::string exercise_name;
    
    CustomList(int _user_id, int _exercise_id, 
              const std::string& _list_name, const std::string& _exercise_name = "")
        : user_id(_user_id), exercise_id(_exercise_id),
          list_name(_list_name), exercise_name(_exercise_name) {}
};


