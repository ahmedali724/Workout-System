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
            "Logout"};

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
            return SendMessageScreen();
        case 4:
            return ViewInboxScreen();
        case 5:
            return ProfileScreen();
        case 6:
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
                tui::printAt(startX, y, item.exercise_name + " - " + std::to_string(item.sets) + " sets x " + std::to_string(item.reps) + " reps", 7);
                y++;
            }
            y += 2;
        }

        std::vector<std::string> menuItems = {
            "Add Exercise",
            "Edit Exercise",
            "Remove Exercise",
            "Back"};

        int menuX = startX + 5;
        int menuY = y;

        int hintY = menuY + menuItems.size() + 2;
        tui::drawHLine(startX, hintY, contentWidth, '-');
        tui::printAt(startX + 4, hintY + 1, "UP / DOWN : Navigate    ENTER : Select", 8);

        int choice = tui::showMenu(menuX, menuY, menuItems);

        switch (choice)
        {
        case 0:
            return AddExerciseToPlanScreen(traineeId);
        case 1:
            return EditExerciseInPlanScreen(traineeId);
        case 2:
            return RemoveExerciseFromPlanScreen(traineeId);
        default:
            return CreateEditWorkoutPlanScreen();
        }
    }

    ScreenData AddExerciseToPlanScreen(int traineeId)
    {
        system("cls");

        int startX = 5, startY = 2;
        const int contentWidth = 70;

        tui::printAt(startX, startY, "=== Add Exercise to Plan ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        auto exercises = exerciseService.getAllExercises();

        std::vector<std::string> menuItems;
        for (const auto &ex : exercises)
        {
            menuItems.push_back(ex.name);
        }
        menuItems.push_back("Back");

        int menuX = startX + 5;
        int menuY = startY + 4;

        int choice = tui::showMenu(menuX, menuY, menuItems);

        if (choice == -1 || choice == menuItems.size() - 1)
        {
            return ManageWorkoutPlanForTraineeScreen(traineeId);
        }

        int exerciseId = exercises[choice].id;

        tui::printAt(startX, menuY + menuItems.size() + 2, "Enter sets:", 7);
        tui::printAt(startX, menuY + menuItems.size() + 3, "Enter reps:", 7);

        int offsetY = static_cast<int>(menuItems.size());
        std::vector<int> fieldX = {startX + 15, startX + 15};
        std::vector<int> fieldY = {menuY + offsetY + 2, menuY + offsetY + 3};
        std::vector<int> fieldLen = {10, 10};
        std::vector<char> sRange = {'0', '0'};
        std::vector<char> eRange = {'9', '9'};
        std::vector<std::string> initialData = {"3", "10"};

        std::vector<std::string> results = tui::addMultiTextField(
            fieldX, fieldY, fieldLen, 2, sRange, eRange, initialData);

        if (results.empty())
        {
            return ManageWorkoutPlanForTraineeScreen(traineeId);
        }

        try
        {
            int sets = std::stoi(results[0]);
            int reps = std::stoi(results[1]);

            if (sets <= 0 || reps <= 0)
            {
                sets = 3;
                reps = 10;
            }

            if (trainerService.addWorkoutPlanItem(traineeId, currentUser.id, exerciseId, sets, reps) > 0)
            {
                tui::printAt(startX, menuY + menuItems.size() + 5, "Exercise added to plan!", 10);
            }
            else
            {
                tui::printAt(startX, menuY + menuItems.size() + 5, "Failed to add exercise.", 12);
            }
        }
        catch (...)
        {
            tui::printAt(startX, menuY + menuItems.size() + 5, "Invalid input.", 12);
        }

        tui::printAt(startX, menuY + menuItems.size() + 6, "Press any key to continue...", 8);
        getch();

        return ManageWorkoutPlanForTraineeScreen(traineeId);
    }

    ScreenData EditExerciseInPlanScreen(int traineeId)
    {
        system("cls");

        int startX = 5, startY = 2;
        const int contentWidth = 70;

        tui::printAt(startX, startY, "=== Edit Exercise in Plan ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        auto plan = trainerService.getTraineeWorkoutPlan(traineeId, currentUser.id);

        if (plan.empty())
        {
            tui::printAt(startX, startY + 3, "No exercises in plan.", 12);
            tui::printAt(startX, startY + 5, "Press any key to return...", 8);
            getch();
            return ManageWorkoutPlanForTraineeScreen(traineeId);
        }

        std::vector<std::string> menuItems;
        for (const auto &item : plan)
        {
            menuItems.push_back(item.exercise_name + " (" + std::to_string(item.sets) +
                                " sets x " + std::to_string(item.reps) + " reps)");
        }
        menuItems.push_back("Back");

        int menuX = startX + 5;
        int menuY = startY + 4;

        int choice = tui::showMenu(menuX, menuY, menuItems);

        if (choice == -1 || choice == menuItems.size() - 1)
        {
            return ManageWorkoutPlanForTraineeScreen(traineeId);
        }

        WorkoutPlanItem selected = plan[choice];

        int offsetY2 = static_cast<int>(menuItems.size());
        tui::printAt(startX, menuY + offsetY2 + 2, "Enter new sets:", 7);
        tui::printAt(startX, menuY + offsetY2 + 3, "Enter new reps:", 7);

        std::vector<int> fieldX = {startX + 18, startX + 18};
        std::vector<int> fieldY = {menuY + offsetY2 + 2, menuY + offsetY2 + 3};
        std::vector<int> fieldLen = {10, 10};
        std::vector<char> sRange = {'0', '0'};
        std::vector<char> eRange = {'9', '9'};
        std::vector<std::string> initialData = {std::to_string(selected.sets), std::to_string(selected.reps)};

        std::vector<std::string> results = tui::addMultiTextField(
            fieldX, fieldY, fieldLen, 2, sRange, eRange, initialData);

        if (results.empty())
        {
            return ManageWorkoutPlanForTraineeScreen(traineeId);
        }

        try
        {
            int sets = std::stoi(results[0]);
            int reps = std::stoi(results[1]);

            if (sets <= 0 || reps <= 0)
            {
                sets = 3;
                reps = 10;
            }

            if (trainerService.updateWorkoutPlanItem(traineeId, currentUser.id, selected.exercise_id, sets, reps) > 0)
            {
                tui::printAt(startX, menuY + menuItems.size() + 5, "Exercise updated!", 10);
            }
            else
            {
                tui::printAt(startX, menuY + menuItems.size() + 5, "Failed to update exercise.", 12);
            }
        }
        catch (...)
        {
            tui::printAt(startX, menuY + menuItems.size() + 5, "Invalid input.", 12);
        }

        tui::printAt(startX, menuY + menuItems.size() + 6, "Press any key to continue...", 8);
        getch();

        return ManageWorkoutPlanForTraineeScreen(traineeId);
    }

    ScreenData RemoveExerciseFromPlanScreen(int traineeId)
    {
        system("cls");

        int startX = 5, startY = 2;
        const int contentWidth = 70;

        tui::printAt(startX, startY, "=== Remove Exercise from Plan ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        auto plan = trainerService.getTraineeWorkoutPlan(traineeId, currentUser.id);

        if (plan.empty())
        {
            tui::printAt(startX, startY + 3, "No exercises in plan.", 12);
            tui::printAt(startX, startY + 5, "Press any key to return...", 8);
            getch();
            return ManageWorkoutPlanForTraineeScreen(traineeId);
        }

        std::vector<std::string> menuItems;
        for (const auto &item : plan)
        {
            menuItems.push_back(item.exercise_name);
        }
        menuItems.push_back("Back");

        int menuX = startX + 5;
        int menuY = startY + 4;

        int choice = tui::showMenu(menuX, menuY, menuItems);

        if (choice == -1 || choice == menuItems.size() - 1)
        {
            return ManageWorkoutPlanForTraineeScreen(traineeId);
        }

        WorkoutPlanItem selected = plan[choice];

        tui::printAt(startX, menuY + menuItems.size() + 2, "Are you sure? (Y/N)", 12);
        int key = getch();
        if (key == 'y' || key == 'Y')
        {
            if (trainerService.deleteWorkoutPlanItem(traineeId, currentUser.id, selected.exercise_id) > 0)
            {
                tui::printAt(startX, menuY + menuItems.size() + 3, "Exercise removed!", 10);
            }
            else
            {
                tui::printAt(startX, menuY + menuItems.size() + 3, "Failed to remove exercise.", 12);
            }
            tui::printAt(startX, menuY + menuItems.size() + 4, "Press any key to continue...", 8);
            getch();
        }

        return ManageWorkoutPlanForTraineeScreen(traineeId);
    }

    ScreenData SendMessageScreen()
    {
        system("cls");

        int startX = 10, startY = 5;
        int fieldLen = 40;

        tui::printAt(startX, startY, "=== Send Message ===", 14);

        auto trainees = trainerService.getMyTrainees(currentUser.id);

        if (trainees.empty())
        {
            tui::printAt(startX, startY + 2, "No trainees assigned yet.", 12);
            tui::printAt(startX, startY + 4, "Press any key to return...", 8);
            getch();
            return ScreenData{Screen::TrainerMenu, currentUser};
        }

        std::vector<std::string> menuItems;
        for (const auto &trainee : trainees)
        {
            menuItems.push_back(trainee.name);
        }
        menuItems.push_back("Back");

        int menuX = startX + 15;
        int menuY = startY + 3;

        tui::printAt(startX, startY + 2, "Select trainee:", 7);

        int choice = tui::showMenu(menuX, menuY, menuItems);

        if (choice == -1 || choice == menuItems.size() - 1)
        {
            return ScreenData{Screen::TrainerMenu, currentUser};
        }

        int selectedTraineeId = trainees[choice].id;
        User selectedTrainee = trainees[choice];

        int offsetY3 = static_cast<int>(menuItems.size());
        tui::printAt(startX, menuY + offsetY3 + 2, "To: " + selectedTrainee.name, 11);
        tui::printAt(startX, menuY + offsetY3 + 3, "Message:", 7);

        std::vector<int> fieldX = {startX + 10};
        std::vector<int> fieldY = {menuY + offsetY3 + 3};
        std::vector<int> fieldLenVec = {fieldLen};
        auto textRange = UIHelpers::getTextRange();
        std::vector<char> sRange = {textRange[0]};
        std::vector<char> eRange = {textRange[1]};
        std::vector<std::string> initialData = {""};

        std::vector<std::string> results = tui::addMultiTextField(
            fieldX, fieldY, fieldLenVec, 1, sRange, eRange, initialData);

        if (results.empty() || results[0].empty())
        {
            return ScreenData{Screen::TrainerMenu, currentUser};
        }

        if (messageService.sendMessage(currentUser.id, selectedTraineeId, results[0]) > 0)
        {
            tui::printAt(startX, menuY + menuItems.size() + 6, "Message sent successfully!", 10);
        }
        else
        {
            tui::printAt(startX, menuY + menuItems.size() + 6, "Failed to send message.", 12);
        }
        tui::printAt(startX, menuY + menuItems.size() + 7, "Press any key to continue...", 8);
        getch();

        return ScreenData{Screen::TrainerMenu, currentUser};
    }

    ScreenData ViewInboxScreen()
    {
        system("cls");

        int startX = 5, startY = 2;
        const int contentWidth = 70;

        tui::printAt(startX, startY, "=== Inbox ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        auto messages = messageService.getInbox(currentUser.id);

        if (messages.empty())
        {
            tui::printAt(startX, startY + 3, "No messages.", 12);
            tui::printAt(startX, startY + 5, "Press any key to return...", 8);
            getch();
            return ScreenData{Screen::TrainerMenu, currentUser};
        }

        int y = startY + 3;
        for (const auto &msg : messages)
        {
            tui::printAt(startX, y, "From: " + msg.sender_name, 11);
            tui::printAt(startX, y + 1, "Time: " + msg.timestamp, 7);
            tui::printAt(startX, y + 2, "Message: " + msg.content, 7);
            y += 4;
        }

        int hintY = y + 2;
        tui::drawHLine(startX, hintY, contentWidth, '-');
        tui::printAt(startX + 4, hintY + 1, "Press any key to return...", 8);
        getch();

        return ScreenData{Screen::TrainerMenu, currentUser};
    }

    ScreenData ProfileScreen()
    {
        system("cls");

        int startX = 10, startY = 5;
        const int contentWidth = 50;

        tui::printAt(startX, startY, "=== My Profile ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        tui::printAt(startX, startY + 3, "Name: " + currentUser.name, 11);
        tui::printAt(startX, startY + 4, "Username: " + currentUser.username, 11);
        tui::printAt(startX, startY + 5, "Email: " + currentUser.email, 11);
        tui::printAt(startX, startY + 6, "Age: " + std::to_string(currentUser.age), 11);
        tui::printAt(startX, startY + 7, "Gender: " + currentUser.gender, 11);
        tui::printAt(startX, startY + 8, "Role: " + currentUser.role, 11);
        std::string avail = currentUser.availability ? "Available" : "Not Available";
        tui::printAt(startX, startY + 9, "Availability: " + avail, 11);

        std::vector<std::string> menuItems = {"Update Profile", "Change Password", "Back"};
        int menuX = startX + 15;
        int menuY = startY + 12;

        int choice = tui::showMenu(menuX, menuY, menuItems);

        switch (choice)
        {
        case 0:
            return UpdateProfileScreen();
        case 1:
            return ChangePasswordScreen();
        default:
            return ScreenData{Screen::TrainerMenu, currentUser};
        }
    }

    ScreenData UpdateProfileScreen()
    {
        return ScreenData{Screen::UpdateProfile, currentUser};
    }

    ScreenData ChangePasswordScreen()
    {
        return ScreenData{Screen::ChangePassword, currentUser};
    }
};
