#pragma once

enum class Screen
{
    Welcome,
    Login,
    Register,
    MainMenu,
    Exit

};

template<typename T>
struct ScreenData
{
    Screen nextScreen;
    T data;
};

struct NoData {};