#include "tuiUtils.hpp"
#include "ScreenData.hpp"
#include "ExerciseService.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>

class ExerciseUI
{
public:
    ExerciseService &exerciseService;

    ExerciseUI(ExerciseService &_exerciseService) : exerciseService(_exerciseService) {}

    // --- Exercise List Screen ---
    ScreenData ExerciseListScreen()
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
        std::vector<char> sRange = {'!'};
        std::vector<char> eRange = {'~'};
        std::vector<std::string> initialData = {""};

        tui::printAt(startX, startY + 3, "Search (leave empty to list all): ", 7);

        std::vector<std::string> results = tui::addMultiTextField(
            fieldX, fieldY, fieldLen, 1, sRange, eRange, initialData);

        if (results.empty())
        {
            return ScreenData{Screen::Welcome, -1};
        }

        std::string keyword = results[0];

        // Get exercises based on input
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
            return ScreenData{Screen::Welcome, -1};
        }

        // Build menu items
        std::vector<std::string> menuItems;
        for (auto &ex : exercises)
        {
            menuItems.push_back(ex.name);
        }

        // Menu position
        int menuX = startX + 5;
        int menuY = startY + 5;

        // Show menu
        int choice = tui::showMenu(menuX, menuY, menuItems);

        // Controller hints below menu
        int hintY = menuY + menuItems.size() + 2;
        tui::drawHLine(startX, hintY, contentWidth, '-');
        tui::printAt(startX + 4, hintY + 1, "UP / DOWN : Navigate    ENTER : Select", 8);


        // Return screen and selected exercise index
        return ScreenData{Screen::ExerciseDetail, exercises[choice].id};
    }

// --- Exercise Detail Screen ---
ScreenData ExerciseDetailScreen(int exerciseId)
{
    system("cls");

    int startX = 5, startY = 2;
    const int contentWidth = 60;

    // Fetch exercise by ID
    auto exerciseOpt = exerciseService.getExerciseById(exerciseId);
    if (!exerciseOpt.has_value())
    {
        tui::printAt(startX, startY, "Exercise not found!", 12);
        tui::printAt(startX, startY + 2, "Press any key to return...", 8);
        getch();
        return ScreenData{Screen::ExerciseList, NoData{}};
    }

    const Exercise &exercise = exerciseOpt.value();

    // Title
    tui::printAt(startX, startY, "=== Exercise Details ===", 14);
    tui::drawHLine(startX, startY + 1, contentWidth, '=');

    // Content
    tui::printAt(startX, startY + 3, "Name: " + exercise.name, 11);
    tui::printAt(startX, startY + 5, "Description: " + exercise.description, 11);
    tui::printAt(startX, startY + 7, "Path: " + exercise.path, 11);

    // Controller hints (AuthUI style)
    int hintY = startY + 10;
    tui::drawHLine(startX, hintY, contentWidth, '-');
    tui::printAt(startX + 4, hintY + 1, "ESC : Return", 8);

    // Wait for ESC
    while (true)
    {
        int key = getch();
        if (key == 27) // ESC
            break;
    }
    // Return screen only
    return ScreenData{Screen::ExerciseList, NoData{}};
}

};
