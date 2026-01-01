#include <windows.h>
#include <mmsystem.h>

#include "Database.hpp"
#include <conio.h>
#include <AuthService.hpp>
#include <ExerciseService.hpp>
#include "TrainerService.hpp"
#include "TraineeService.hpp"
#include "MessageService.hpp"
#include "AdminService.hpp"
#include <User.hpp>
#include "ScreenData.hpp"
#include "AuthUI.hpp"
#include "ExerciseUI.hpp"
#include "TraineeUI.hpp"
#include "TrainerUI.hpp"
#include "AdminUI.hpp"
#include <any>

using namespace std;
using namespace tui;

int main()
{
    DB db("fitnessGo.db");
    AuthService authService(db);
    ExerciseService exerciseService(db);
    TrainerService trainerService(db);
    TraineeService traineeService(db);
    MessageService messageService(db);
    AdminService adminService(db);

    AuthUI authUI(authService);
    ExerciseUI exerciseUI(exerciseService);

    system("cls");

    const int startX = 10;
    const int startY = 2;

    PlaySound(TEXT("intro.wav"), NULL, SND_FILENAME | SND_ASYNC);

    Sleep(200);
    tui::printAt(startX, startY, R"( _____ _ _                        ____         _ )", 14);
    Sleep(1100);
    tui::printAt(startX, startY + 1, R"(|  ___(_) |_ _ __   ___  ___ ___ / ___| ___   | |)", 14);
    Sleep(900);
    tui::printAt(startX, startY + 2, R"(| |_  | | __| '_ \ / _ \/ __/ __| |  _ / _ \  | |)", 14);
    Sleep(900);
    tui::printAt(startX, startY + 3, R"(|  _| | | |_| | | |  __/\__ \__ \ |_| | (_) | |_|)", 14);
    Sleep(900);
    tui::printAt(startX, startY + 4, R"(|_|   |_|\__|_| |_|\___||___/___/\____|\___/  (_))", 14);

    Sleep(800);

    ScreenData current{Screen::Welcome, NoData{}};
    User currentUser(0, "", "", "", "", 0, "", "", false);

    while (current.nextScreen != Screen::Exit)
    {
        switch (current.nextScreen)
        {
        case Screen::Welcome:
            current = authUI.WelcomeScreen();
            break;
        case Screen::Login:
            current = authUI.LoginScreen();
            if (current.nextScreen == Screen::TraineeMenu ||
                current.nextScreen == Screen::TrainerMenu ||
                current.nextScreen == Screen::AdminMenu)
            {
                try
                {
                    currentUser = any_cast<User>(current.data);
                }
                catch (...)
                {
                }
            }
            break;
        case Screen::Register:
            current = authUI.RegisterScreen();
            break;
        case Screen::TraineeMenu:
        {
            TraineeUI traineeUI(exerciseService, trainerService, traineeService, messageService, currentUser);
            current = traineeUI.TraineeMenuScreen();
            try
            {
                currentUser = any_cast<User>(current.data);
            }
            catch (...)
            {
            }
        }
        break;
        case Screen::TrainerMenu:
        {
            TrainerUI trainerUI(exerciseService, trainerService, messageService, currentUser);
            current = trainerUI.TrainerMenuScreen();
            try
            {
                currentUser = any_cast<User>(current.data);
            }
            catch (...)
            {
            }
        }
        break;
        case Screen::AdminMenu:
        {
            AdminUI adminUI(adminService, currentUser);
            current = adminUI.AdminMenuScreen();
            try
            {
                currentUser = any_cast<User>(current.data);
            }
            catch (...)
            {
            }
        }
        break;
        case Screen::ChangePassword:
        {
            try
            {
                User user = any_cast<User>(current.data);
                current = authUI.ChangePasswordScreen(user);
                if (current.nextScreen == Screen::Welcome)
                {
                    current = ScreenData{Screen::Login, NoData{}};
                }
                else
                {
                    if (user.role == "Trainee")
                        current = ScreenData{Screen::TraineeMenu, user};
                    else if (user.role == "Trainer")
                        current = ScreenData{Screen::TrainerMenu, user};
                    else if (user.role == "Admin")
                        current = ScreenData{Screen::AdminMenu, user};
                }
            }
            catch (...)
            {
                current = ScreenData{Screen::Welcome, NoData{}};
            }
        }
        break;
        case Screen::UpdateProfile:
        {
            try
            {
                User user = any_cast<User>(current.data);
                current = authUI.UpdateProfileScreen(user);
                if (current.nextScreen == Screen::Welcome)
                {
                    try
                    {
                        currentUser = any_cast<User>(current.data);
                        if (currentUser.role == "Trainee")
                            current = ScreenData{Screen::TraineeMenu, currentUser};
                        else if (currentUser.role == "Trainer")
                            current = ScreenData{Screen::TrainerMenu, currentUser};
                        else if (currentUser.role == "Admin")
                            current = ScreenData{Screen::AdminMenu, currentUser};
                    }
                    catch (...)
                    {
                        current = ScreenData{Screen::Welcome, NoData{}};
                    }
                }
            }
            catch (...)
            {
                current = ScreenData{Screen::Welcome, NoData{}};
            }
        }
        break;
        case Screen::ExerciseDetail:
        {
            int exerciseId;
            try
            {
                exerciseId = any_cast<int>(current.data);
            }
            catch (...)
            {
                if (currentUser.role == "Trainee")
                    current = ScreenData{Screen::TraineeMenu, currentUser};
                else if (currentUser.role == "Trainer")
                    current = ScreenData{Screen::TrainerMenu, currentUser};
                else if (currentUser.role == "Admin")
                    current = ScreenData{Screen::AdminMenu, currentUser};
                else
                    current = ScreenData{Screen::Welcome, NoData{}};
                break;
            }
            current = exerciseUI.ExerciseDetailScreen(exerciseId, currentUser);
        }
        break;
        case Screen::ExerciseList:
        {
            User userForUI = currentUser;
            try
            {
                userForUI = any_cast<User>(current.data);
                currentUser = userForUI;
            }
            catch (...)
            {
            }
            current = exerciseUI.ExerciseListScreen(userForUI);
        }
        break;
        }
    }

    return 0;
}
