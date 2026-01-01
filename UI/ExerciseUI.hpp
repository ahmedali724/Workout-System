#include "tuiUtils.hpp"
#include "ScreenData.hpp"
#include "ExerciseService.hpp"
#include "User.hpp"
#include "UIHelpers.hpp"
#include <vector>
#include <string>
#include <conio.h>

class ExerciseUI
{
public:
    ExerciseService &exerciseService;

    ExerciseUI(ExerciseService &_exerciseService) : exerciseService(_exerciseService) {}

    // --- Exercise List Screen ---
    ScreenData ExerciseListScreen(const User &user = User(0, "", "", "", "", 0, "", "", false))
    {
        system("cls");

        int startX = 5, startY = 2;
        const int contentWidth = 60;

        // Title
        tui::printAt(startX, startY, "=== Exercise Search ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        // Multi-text field for search
        std::vector<int> fieldX = {startX + 40};
        std::vector<int> fieldY = {startY + 3};
        std::vector<int> fieldLen = {30};
        auto textRange = UIHelpers::getTextRange();
        std::vector<char> sRange = {textRange[0]};
        std::vector<char> eRange = {textRange[1]};
        std::vector<std::string> initialData = {""};

        tui::printAt(startX, startY + 3, "Search (leave empty to list all): ", 7);

        std::vector<std::string> results = tui::addMultiTextField(
            fieldX, fieldY, fieldLen, 1, sRange, eRange, initialData);

        if (results.empty())
        {
            return user.role != "" ? UIHelpers::routeToUserMenu(user) : ScreenData{Screen::Welcome, NoData{}};
        }

        std::string keyword = results[0];
        std::vector<Exercise> exercises;
        if (keyword.empty())
        {
            exercises = exerciseService.getAllExercises();
        }
        else
        {
            exercises = exerciseService.searchExercisesByName(keyword);
        }

        if (exercises.empty())
        {
            tui::printAt(startX, startY + 5, "No exercises found.", 12);
            tui::printAt(startX, startY + 7, "Press any key to return...", 8);
            getch();
            return user.role != "" ? UIHelpers::routeToUserMenu(user) : ScreenData{Screen::Welcome, NoData{}};
        }

        std::vector<std::string> menuItems;
        for (auto &ex : exercises)
        {
            menuItems.push_back(ex.name);
        }
        menuItems.push_back("Back");

        int menuX = startX + 5;
        int menuY = startY + 5;
        int hintY = menuY + menuItems.size() + 2;
        tui::drawHLine(startX, hintY, contentWidth, '-');
        tui::printAt(startX + 4, hintY + 1, "UP / DOWN : Navigate    ENTER : Select    ESC : Back", 8);

        int choice = tui::showMenu(menuX, menuY, menuItems);

        if (choice == -1 || choice == menuItems.size() - 1)
        {
            return user.role != "" ? UIHelpers::routeToUserMenu(user) : ScreenData{Screen::Welcome, NoData{}};
        }

        return ScreenData{Screen::ExerciseDetail, exercises[choice].id};
    }

    // --- Exercise Detail Screen ---
    ScreenData ExerciseDetailScreen(int exerciseId, const User &user = User(0, "", "", "", "", 0, "", "", false))
    {
        system("cls");

        int startX = 5, startY = 2;
        const int contentWidth = 60;

        auto exerciseOpt = exerciseService.getExerciseById(exerciseId);
        if (!exerciseOpt.has_value())
        {
            tui::printAt(startX, startY, "Exercise not found!", 12);
            tui::printAt(startX, startY + 2, "Press any key to return...", 8);
            getch();
            if (user.role != "")
            {
                if (user.role == "Trainee")
                    return ScreenData{Screen::TraineeMenu, user};
                else if (user.role == "Trainer")
                    return ScreenData{Screen::TrainerMenu, user};
                else if (user.role == "Admin")
                    return ScreenData{Screen::AdminMenu, user};
            }
            return ScreenData{Screen::ExerciseList, NoData{}};
        }

        const Exercise &exercise = exerciseOpt.value();

        tui::printAt(startX, startY, "=== Exercise Details ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        tui::printAt(startX, startY + 3, "Name: " + exercise.name, 11);
        tui::printAt(startX, startY + 5, "Description: " + exercise.description, 11);
        tui::printAt(startX, startY + 7, "Path: " + exercise.path, 11);

        int hintY = startY + 10;
        tui::drawHLine(startX, hintY, contentWidth, '-');
        tui::printAt(startX + 4, hintY + 1, "Enter : View Animation    ESC : Return", 8);

        while (true)
        {
            int key = getch();
            if (key == 27)
                break;
            else if (key == 13)
            {
                system(("start \"\" animation.exe \"ASCIIFrames/" + exercise.name + "\"").c_str());
            }
        }

        return user.role != "" ? UIHelpers::routeToUserMenu(user) : ScreenData{Screen::ExerciseList, NoData{}};
    }
};
