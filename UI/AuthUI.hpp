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

        // Layout base
        const int startX = 10;
        const int startY = 2;
        const int contentWidth = 50;

        // ASCII Logo
        tui::printAt(startX, startY, R"(__        __   _                            _ )");
        tui::printAt(startX, startY + 1, R"(\ \      / /__| | ___ ___  _ __ ___   ___  | |)");
        tui::printAt(startX, startY + 2, R"( \ \ /\ / / _ \ |/ __/ _ \| '_ ` _ \ / _ \ | |)");
        tui::printAt(startX, startY + 3, R"(  \ V  V /  __/ | (_| (_) | | | | | |  __/ |_|)");
        tui::printAt(startX, startY + 4, R"(   \_/\_/ \___|_|\___\___/|_| |_| |_|\___| (_))");

        // Separator line
        tui::drawHLine(startX, startY + 6, contentWidth, '=');

        // Subtitle
        tui::printAt(startX + 12, startY + 7, "Welcome to FitnessGO!", 14);


        // Menu items
        vector<string> menuItems = {
            "Login",
            "Register",
            "Exit"};

        // Menu position (centered-ish)
        int menuX = startX + 18;
        int menuY = startY + 9;

        int choice = tui::showMenu(menuX, menuY, menuItems);

        // Controller hints
        int hintY = menuY + menuItems.size() + 2;
        tui::drawHLine(startX, hintY, contentWidth, '-');
        tui::printAt(startX + 4, hintY + 1, "UP / DOWN : Navigate    ENTER : Select", 8);

        // Navigation
        switch (choice)
        {
        case 0:
            return ScreenData<NoData>{Screen::Login, NoData{}};

        case 1:
            return ScreenData<NoData>{Screen::Register, NoData{}};

        default:
            return ScreenData<NoData>{Screen::Exit, NoData{}};
        }
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
            if (results.empty())
            {
                return ScreenData<NoData>{Screen::Welcome, NoData{}};
            }
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

    ScreenData<NoData> RegisterScreen()
    {
        system("cls");

        int startX = 10, startY = 4;
        int fieldLen = 22;

        // Title
        tui::printAt(startX, startY, "=== Register New Account ===", 14);

        // Labels
        tui::printAt(startX, startY + 2, "Username:", 7);
        tui::printAt(startX, startY + 4, "Password:", 7);
        tui::printAt(startX, startY + 6, "Email:", 7);
        tui::printAt(startX, startY + 8, "Name:", 7);
        tui::printAt(startX, startY + 10, "Age:", 7);
        tui::printAt(startX, startY + 12, "Gender (Male/Female):", 7);
        tui::printAt(startX, startY + 14, "Role (Trainee/Trainer):", 7);

        // Error message area
        int errX = startX;
        int errY = startY + 17;
        string errorMessage = "";

        // Controller instructions
        tui::printAt(startX, errY + 2, "UP/DOWN: Navigate   ENTER: Submit", 8);
        tui::drawHLine(startX, errY + 3, 50, '-');

        // Multi-field configuration
        vector<int> fieldX = {
            startX + 26,
            startX + 26,
            startX + 26,
            startX + 26,
            startX + 26,
            startX + 26,
            startX + 26};

        vector<int> fieldY = {
            startY + 2,
            startY + 4,
            startY + 6,
            startY + 8,
            startY + 10,
            startY + 12,
            startY + 14};

        vector<int> fieldLenVec(7, fieldLen);
        vector<char> sRange(7, '!');
        vector<char> eRange(7, '~');

        vector<string> initialData = {};

        while (true)
        {
            vector<string> results =
                tui::addMultiTextField(
                    fieldX,
                    fieldY,
                    fieldLenVec,
                    7,
                    sRange,
                    eRange,
                    initialData);

            if (results.empty())
            {
                return ScreenData<NoData>{Screen::Welcome, NoData{}};
            }

            initialData = results;

            try
            {
                int age = stoi(results[4]);

                User newUser(
                    0,          // ID ignored (auto-generated)
                    results[0], // username
                    results[1], // password
                    results[2], // email
                    results[3], // name
                    age,
                    results[5], // gender
                    results[6]  // role
                );

                if (authService.registerUser(newUser) > 0)
                {
                    return ScreenData<NoData>{Screen::Login, NoData{}};
                }
                else
                {
                    errorMessage = "Registration failed. check inputs please!";
                }
            }
            catch (...)
            {
                errorMessage = "Age must be a valid number!";
            }

            // Clear & show error
            tui::drawHLine(errX, errY, 60, ' ');
            tui::printAt(errX, errY, errorMessage, 12);
        }
    }
};
