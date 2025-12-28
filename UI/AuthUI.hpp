#include "tuiUtils.hpp"
#include "ScreenData.hpp"
#include <iostream>
#include<conio.h>
class AuthUI
{
public:
    ScreenData<NoData> WelcomeScreen()
    {
        std::cout << R"(
__        __   _                            _ 
\ \      / /__| | ___ ___  _ __ ___   ___  | |
 \ \ /\ / / _ \ |/ __/ _ \| '_ ` _ \ / _ \ | |
  \ V  V /  __/ | (_| (_) | | | | | |  __/ |_|
   \_/\_/ \___|_|\___\___/|_| |_| |_|\___| (_)
)" << std::endl;

        std::cout<<"Press Any Key to Continue...";
        getch();
        return ScreenData<NoData>{Screen::Exit, NoData{}};
    }
};
