#include "tuiUtils.hpp"
#include "ScreenData.hpp"
#include "ExerciseService.hpp"
#include "TrainerService.hpp"
#include "MessageService.hpp"
#include "User.hpp"
#include "WorkoutPlan.hpp"
#include "UIHelpers.hpp"
#include <vector>
#include <string>
#include <conio.h>

class TrainerUI
{
public:
    ExerciseService &exerciseService;
    TrainerService &trainerService;
    MessageService &messageService;
    User currentUser;

    TrainerUI(ExerciseService &_exerciseService, TrainerService &_trainerService,
              MessageService &_messageService, User _user) 
        : exerciseService(_exerciseService), trainerService(_trainerService),
          messageService(_messageService), currentUser(_user) {}

    
    ScreenData TrainerMenuScreen()
    {
        system("cls");

        const int startX = 10;
        const int startY = 2;
        const int contentWidth = 50;

        UIHelpers::showDashboardHeader(startX, startY, contentWidth, "Trainer Dashboard", currentUser);
        std::string availStatus = currentUser.availability ? "Available" : "Not Available";
        tui::printAt(startX, startY + 5, "Status: " + availStatus, 7);
        int totalClients = trainerService.getTotalClients(currentUser.id);
        tui::printAt(startX, startY + 6, "Total Clients: " + std::to_string(totalClients), 7);

        std::vector<std::string> menuItems = {
            "View My Trainees",
            "Create/Edit Workout Plan",
            "View Exercise Library",
            "Send Message to Trainee",
            "Inbox",
            "View/Update Profile",
            "Logout"
        };

        int menuX = startX + 12;
        int menuY = startY + 9;
        int choice = UIHelpers::showMenuScreen(startX, startY, contentWidth, menuX, menuY, menuItems);

        
        switch (choice)
        {
        case 0: 
            return ViewTraineesScreen();
        case 1: 
            return CreateEditWorkoutPlanScreen();
        case 2: 
            return ScreenData{Screen::ExerciseList, currentUser};
        case 3: 
        default:
            return ScreenData{Screen::Welcome, NoData{}};
        }
    }

    
    ScreenData ViewTraineesScreen()
    {
        system("cls");

        int startX = 5, startY = 2;
        const int contentWidth = 70;

        
        tui::printAt(startX, startY, "=== My Trainees ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        auto trainees = trainerService.getMyTrainees(currentUser.id);

        if (trainees.empty())
        {
            tui::printAt(startX, startY + 3, "No trainees assigned yet.", 12);
            tui::printAt(startX, startY + 5, "Press any key to return...", 8);
            getch();
            return ScreenData{Screen::TrainerMenu, currentUser};
        }

        
        int tableY = startY + 3;
        tui::printAt(startX, tableY, "Name", 14);
        tui::drawHLine(startX, tableY + 1, contentWidth, '-');

        std::vector<std::string> menuItems;
        for (const auto &trainee : trainees)
        {
            menuItems.push_back(trainee.name);
        }
        menuItems.push_back("Back");

        int menuX = startX + 5;
        int menuY = tableY + 3;

        int hintY = menuY + menuItems.size() + 2;
        tui::drawHLine(startX, hintY, contentWidth, '-');
        tui::printAt(startX + 4, hintY + 1, "UP / DOWN : Navigate    ENTER : View Profile    ESC : Back", 8);

        int choice = tui::showMenu(menuX, menuY, menuItems);

        if (choice == -1 || choice == menuItems.size() - 1)
        {
            return ScreenData{Screen::TrainerMenu, currentUser};
        }

        
        return ViewTraineeProfileScreen(trainees[choice].id);
    }

    
    ScreenData ViewTraineeProfileScreen(int traineeId)
    {
        system("cls");

        int startX = 10, startY = 5;
        const int contentWidth = 50;

        auto traineeOpt = trainerService.getTraineeProfile(traineeId);
        if (!traineeOpt.has_value())
        {
            tui::printAt(startX, startY, "Trainee not found!", 12);
            tui::printAt(startX, startY + 2, "Press any key to return...", 8);
            getch();
            return ScreenData{Screen::TrainerMenu, currentUser};
        }

        User trainee = traineeOpt.value();

        
        tui::printAt(startX, startY, "=== Trainee Profile ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        
        tui::printAt(startX, startY + 3, "Name: " + trainee.name, 11);
        tui::printAt(startX, startY + 4, "Email: " + trainee.email, 11);
        tui::printAt(startX, startY + 5, "Age: " + std::to_string(trainee.age), 11);
        tui::printAt(startX, startY + 6, "Gender: " + trainee.gender, 11);

        
        int hintY = startY + 9;
        tui::drawHLine(startX, hintY, contentWidth, '-');
        tui::printAt(startX + 4, hintY + 1, "Press any key to return...", 8);

        getch();
        return ViewTraineesScreen();
    }

    
    ScreenData CreateEditWorkoutPlanScreen()
    {
        system("cls");

        int startX = 5, startY = 2;
        const int contentWidth = 70;

        
        tui::printAt(startX, startY, "=== Create/Edit Workout Plan ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        
        auto trainees = trainerService.getMyTrainees(currentUser.id);

        if (trainees.empty())
        {
            tui::printAt(startX, startY + 3, "No trainees assigned yet.", 12);
            tui::printAt(startX, startY + 5, "Press any key to return...", 8);
            getch();
            return ScreenData{Screen::TrainerMenu, currentUser};
        }

        
        std::vector<std::string> menuItems;
        for (const auto &trainee : trainees)
        {
            menuItems.push_back(trainee.name);
        }
        menuItems.push_back("Back");

        int menuX = startX + 5;
        int menuY = startY + 4;

        tui::printAt(startX, startY + 3, "Select trainee:", 7);

        int hintY = menuY + menuItems.size() + 2;
        tui::drawHLine(startX, hintY, contentWidth, '-');
        tui::printAt(startX + 4, hintY + 1, "UP / DOWN : Navigate    ENTER : Select", 8);

        int choice = tui::showMenu(menuX, menuY, menuItems);

        if (choice == -1 || choice == menuItems.size() - 1)
        {
            return ScreenData{Screen::TrainerMenu, currentUser};
        }

        int selectedTraineeId = trainees[choice].id;
        return ManageWorkoutPlanForTraineeScreen(selectedTraineeId);
    }

    
    ScreenData ManageWorkoutPlanForTraineeScreen(int traineeId)
    {
        system("cls");

        int startX = 5, startY = 2;
        const int contentWidth = 70;

        
        tui::printAt(startX, startY, "=== Manage Workout Plan ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        
        auto currentPlan = trainerService.getTraineeWorkoutPlan(traineeId, currentUser.id);

        
        int y = startY + 3;
        if (!currentPlan.empty())
        {
            tui::printAt(startX, y, "Current Workout Plan:", 14);
            y += 2;
            for (const auto &item : currentPlan)
            {
                tui::printAt(startX, y, item.exercise_name + " - " + 
                           std::to_string(item.sets) + " sets x " + std::to_string(item.reps) + " reps", 7);
                y++;
            }
            y += 2;
        }

        
        std::vector<std::string> menuItems = {
            "Add Exercise",
            "Edit Exercise",
            "Remove Exercise",
            "Back"
        };

        int menuX = startX + 5;
        int menuY = y;

        int hintY = menuY + menuItems.size() + 2;
        tui::drawHLine(startX, hintY, contentWidth, '-');
        tui::printAt(startX + 4, hintY + 1, "UP / DOWN : Navigate    ENTER : Select", 8);

        int choice = tui::showMenu(menuX, menuY, menuItems);

        // Manage Exercise Plan Screen
    }
};




