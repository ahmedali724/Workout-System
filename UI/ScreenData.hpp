#pragma once
#include <any>

enum class Screen
{
    Welcome,
    Login,
    Register,
    MainMenu,
    ExerciseList,
    ExerciseDetail,
    Exit

};

struct ScreenData
{
    Screen nextScreen;
    std::any data;
};

struct NoData {};