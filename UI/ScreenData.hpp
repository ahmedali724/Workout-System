#pragma once
#include <any>
#include "User.hpp"

enum class Screen
{
    Welcome,
    Login,
    Register,
    TraineeMenu,
    TrainerMenu,
    ExerciseList,
    ExerciseDetail,
    ChangePassword,
    UpdateProfile,
    AdminMenu,
    Exit

};

struct ScreenData
{
    Screen nextScreen;
    std::any data;
};

struct NoData {};