#include "tuiUtils.hpp"
#include "ScreenData.hpp"
#include <iostream>
#include <conio.h>
#include <vector>
#include <string>

using namespace std;
class AuthUI
{
public:
    AuthService &authService;
    AuthUI(AuthService &_authService) : authService(_authService)
    {
    }

    ScreenData<NoData> WelcomeScreen()
    {
        system("cls");
        std::cout << R"(
__        __   _                            _ 
\ \      / /__| | ___ ___  _ __ ___   ___  | |
 \ \ /\ / / _ \ |/ __/ _ \| '_ ` _ \ / _ \ | |
  \ V  V /  __/ | (_| (_) | | | | | |  __/ |_|
   \_/\_/ \___|_|\___\___/|_| |_| |_|\___| (_)
)" << std::endl;

        std::cout << "Press Any Key to Continue...";
        getch();
        return ScreenData<NoData>{Screen::Login, NoData{}};
    }

    ScreenData<NoData> LoginScreen()
    {
        system("cls");
        int startX = 10, startY = 5;
        int fieldLen = 20;

        // Title
        tui::printAt(startX, startY, "=== FitnessGO! ===", 14);

        // Labels
        tui::printAt(startX, startY + 2, "Username:", 7);
        tui::printAt(startX, startY + 4, "Password:", 7);

        // Error message area
        int errX = startX;
        int errY = startY + 6;
        string errorMessage = "";

        // Instructions / controller at the bottom
        int ctrlX = startX;
        int ctrlY = startY + 8;
        tui::printAt(ctrlX, ctrlY, "Use UP/DOWN to navigate, ENTER to submit", 8);
        tui::drawHLine(ctrlX, ctrlY + 1, 40, '-');

        // Multi-line field setup
        vector<int> fieldX = {startX + 10, startX + 10};
        vector<int> fieldY = {startY + 2, startY + 4};
        vector<int> fieldLenVec = {fieldLen, fieldLen};
        vector<char> sRange = {'!', '!'};
        vector<char> eRange = {'~', '~'};
        vector<string> initialData = {};

        while (true)
        {
            // Add fields and handle input
            vector<string> results = tui::addMultiTextField(fieldX, fieldY, fieldLenVec, 2, sRange, eRange, initialData);

            initialData = results;

            if (authService.login(results[0], results[1]) != nullopt)
            {
                return ScreenData<NoData>{Screen::Welcome, NoData{}};
            }
            else
            {
                errorMessage = "Check username name or password please !";
                tui::drawHLine(errX, errY, fieldLen + 1, ' ');
                tui::printAt(errX, errY, errorMessage, 12);
            }
        }
    }
};
