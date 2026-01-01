#include "tuiUtils.hpp"
#include "ScreenData.hpp"
#include "AuthService.hpp"
#include "User.hpp"
#include "UIHelpers.hpp"
#include <conio.h>
#include <vector>
#include <string>
#include <optional>
using namespace std;
class AuthUI
{
public:
    AuthService &authService;
    AuthUI(AuthService &_authService) : authService(_authService)
    {
    }

    ScreenData WelcomeScreen()
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
            return ScreenData{Screen::Login, NoData{}};

        case 1:
            return ScreenData{Screen::Register, NoData{}};

        default:
            return ScreenData{Screen::Exit, NoData{}};
        }
    }

    ScreenData LoginScreen()
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
        auto textRange = UIHelpers::getTextRange();
        vector<char> sRange = {textRange[0], textRange[0]};
        vector<char> eRange = {textRange[1], textRange[1]};
        vector<string> initialData = {};

        while (true)
        {
            // Add fields and handle input
            vector<string> results = tui::addMultiTextField(fieldX, fieldY, fieldLenVec, 2, sRange, eRange, initialData);
            if (results.empty())
            {
                return ScreenData{Screen::Welcome, NoData{}};
            }
            initialData = results;

            auto userOpt = authService.login(results[0], results[1]);
            if (userOpt != nullopt)
            {
                return UIHelpers::routeToUserMenu(userOpt.value());
            }
            else
            {
                errorMessage = "Check username name or password please !";
                UIHelpers::showError(errX, errY, errorMessage);
            }
        }
    }

    ScreenData RegisterScreen()
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
        tui::printAt(startX, startY + 16, "Availability (1/0):", 7);

        // Error message area
        int errX = startX;
        int errY = startY + 17;
        string errorMessage = "";

        UIHelpers::showInstructions(startX, errY + 2);

        // Multi-field configuration
        vector<int> fieldX = {
            startX + 26,
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
            startY + 14,
            startY + 16};

        vector<int> fieldLenVec(8, fieldLen);
        auto textRange = UIHelpers::getTextRange();
        auto ageRange = UIHelpers::getAgeRange();
        auto availRange = UIHelpers::getAvailabilityRange();
        vector<char> sRange = {textRange[0], textRange[0], textRange[0], textRange[0], ageRange[0], textRange[0], textRange[0], availRange[0]};
        vector<char> eRange = {textRange[1], textRange[1], textRange[1], textRange[1], ageRange[1], textRange[1], textRange[1], availRange[1]};

        vector<string> initialData = {};

        while (true)
        {
            vector<string> results =
                tui::addMultiTextField(
                    fieldX,
                    fieldY,
                    fieldLenVec,
                    8,
                    sRange,
                    eRange,
                    initialData);

            if (results.empty())
            {
                return ScreenData{Screen::Welcome, NoData{}};
            }

            initialData = results;

            int age;
            bool availability;
            if (!UIHelpers::validateAge(results[4], age))
            {
                errorMessage = "Age must be 12 or more !";
            }
            else if (!UIHelpers::validateAvailability(results[7], availability))
            {
                errorMessage = "Availability must be 0 or 1!";
            }
            else
            {
                std::string role = results[6];
                int result = 0;
                if (role == "Trainee")
                {
                    Trainee trainee(0, results[0], results[1], results[2], results[3], age, results[5], availability);
                    result = authService.registerUser(trainee);
                }
                else if (role == "Trainer")
                {
                    Trainer trainer(0, results[0], results[1], results[2], results[3], age, results[5], availability);
                    result = authService.registerUser(trainer);
                }
                else
                {
                    User user(0, results[0], results[1], results[2], results[3], age, results[5], role, availability);
                    result = authService.registerUser(user);
                }

                if (result > 0)
                {
                    return ScreenData{Screen::Login, NoData{}};
                }
                else
                {
                    errorMessage = "Registration failed. check inputs please!";
                }
            }

            UIHelpers::showError(errX, errY, errorMessage);
        }
    }

    // --- Change Password Screen ---
    ScreenData ChangePasswordScreen(User &currentUser)
    {
        system("cls");

        int startX = 10, startY = 5;
        int fieldLen = 25;

        // Title
        tui::printAt(startX, startY, "=== Change Password ===", 14);

        // Labels
        tui::printAt(startX, startY + 2, "Current Password:", 7);
        tui::printAt(startX, startY + 4, "New Password:", 7);
        tui::printAt(startX, startY + 6, "Confirm New Password:", 7);

        // Error message area
        int errX = startX;
        int errY = startY + 8;
        string errorMessage = "";

        UIHelpers::showInstructions(startX, errY + 2);

        // Multi-field configuration
        vector<int> fieldX = {startX + 20, startX + 20, startX + 20};
        vector<int> fieldY = {startY + 2, startY + 4, startY + 6};
        vector<int> fieldLenVec(3, fieldLen);
        auto textRange = UIHelpers::getTextRange();
        vector<char> sRange(3, textRange[0]);
        vector<char> eRange(3, textRange[1]);
        vector<string> initialData = {};

        while (true)
        {
            vector<string> results = tui::addMultiTextField(
                fieldX, fieldY, fieldLenVec, 3, sRange, eRange, initialData);

            if (results.empty())
            {
                return ScreenData{Screen::Welcome, NoData{}};
            }

            initialData = results;

            if (results[0].empty() || results[1].empty() || results[2].empty())
            {
                errorMessage = "All fields are required!";
            }
            else if (results[1] != results[2])
            {
                errorMessage = "New passwords do not match!";
            }
            else if (authService.changePassword(currentUser.id, results[0], results[1]) > 0)
            {
                tui::printAt(startX, errY, "Password changed successfully!", 10);
                tui::printAt(startX, errY + 1, "Press any key to continue...", 8);
                getch();
                return ScreenData{Screen::Welcome, NoData{}};
            }
            else
            {
                errorMessage = "Invalid current password or update failed!";
            }

            UIHelpers::showError(errX, errY, errorMessage);
        }
    }

    // --- Update Profile Screen ---
    ScreenData UpdateProfileScreen(User &currentUser)
    {
        system("cls");

        int startX = 10, startY = 4;
        int fieldLen = 22;

        // Title
        tui::printAt(startX, startY, "=== Update Profile ===", 14);

        // Labels
        tui::printAt(startX, startY + 2, "Username:", 7);
        tui::printAt(startX, startY + 4, "Email:", 7);
        tui::printAt(startX, startY + 6, "Name:", 7);
        tui::printAt(startX, startY + 8, "Age:", 7);
        tui::printAt(startX, startY + 10, "Gender (Male/Female):", 7);
        tui::printAt(startX, startY + 12, "Availability (1/0):", 7);

        // Error message area
        int errX = startX;
        int errY = startY + 14;
        string errorMessage = "";

        UIHelpers::showInstructions(startX, errY + 2);

        // Pre-fill with current data
        vector<string> initialData = {
            currentUser.username,
            currentUser.email,
            currentUser.name,
            to_string(currentUser.age),
            currentUser.gender,
            to_string(currentUser.availability ? 1 : 0)};

        // Multi-field configuration
        vector<int> fieldX = {
            startX + 26, startX + 26, startX + 26,
            startX + 26, startX + 26, startX + 26};
        vector<int> fieldY = {
            startY + 2, startY + 4, startY + 6,
            startY + 8, startY + 10, startY + 12};
        vector<int> fieldLenVec(6, fieldLen);
        auto textRange = UIHelpers::getTextRange();
        auto ageRange = UIHelpers::getAgeRange();
        auto availRange = UIHelpers::getAvailabilityRange();
        vector<char> sRange = {textRange[0], textRange[0], textRange[0], ageRange[0], textRange[0], availRange[0]};
        vector<char> eRange = {textRange[1], textRange[1], textRange[1], ageRange[1], textRange[1], availRange[1]};

        while (true)
        {
            vector<string> results = tui::addMultiTextField(
                fieldX, fieldY, fieldLenVec, 6, sRange, eRange, initialData);

            if (results.empty())
            {
                return ScreenData{Screen::Welcome, currentUser};
            }

            initialData = results;

            int age;
            bool availability;
            if (!UIHelpers::validateAge(results[3], age))
            {
                errorMessage = "Age must be 12 or more !";
            }
            else if (!UIHelpers::validateAvailability(results[5], availability))
            {
                errorMessage = "Availability must be 0 or 1!";
            }
            else
            {
                User updatedUser(
                    currentUser.id,
                    results[0],
                    currentUser.password,
                    results[1],
                    results[2],
                    age,
                    results[4],
                    currentUser.role,
                    availability);

                if (authService.updateProfile(updatedUser) > 0)
                {
                    tui::printAt(startX, errY, "Profile updated successfully!", 10);
                    tui::printAt(startX, errY + 1, "Press any key to continue...", 8);
                    getch();
                    return ScreenData{Screen::Welcome, updatedUser};
                }
                else
                {
                    errorMessage = "Update failed. Check inputs please!";
                }
            }

            UIHelpers::showError(errX, errY, errorMessage);
        }
    }
};
