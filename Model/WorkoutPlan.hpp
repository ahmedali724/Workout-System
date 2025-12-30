#pragma once
#include <string>

struct WorkoutPlanItem {
    int trainee_id;
    int trainer_id;
    int exercise_id;
    int sets;
    int reps;
    bool completed;
    std::string exercise_name;
    
    WorkoutPlanItem(int _trainee_id, int _trainer_id, int _exercise_id, 
                   int _sets, int _reps, bool _completed, const std::string& _exercise_name)
        : trainee_id(_trainee_id), trainer_id(_trainer_id), exercise_id(_exercise_id),
          sets(_sets), reps(_reps), completed(_completed), exercise_name(_exercise_name) {}
};


