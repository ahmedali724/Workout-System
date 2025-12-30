#include "tuiUtils.hpp"
#include "ScreenData.hpp"
#include "ExerciseService.hpp"
#include "TrainerService.hpp"
#include "TraineeService.hpp"
#include "MessageService.hpp"
#include "User.hpp"
#include "WorkoutPlan.hpp"
#include "CustomList.hpp"
#include "UIHelpers.hpp"
#include <vector>
#include <string>
#include <conio.h>

class TraineeUI
{
public:
  ExerciseService &exerciseService;
  TrainerService &trainerService;
  TraineeService &traineeService;
  MessageService &messageService;
  User currentUser;

  TraineeUI(ExerciseService &_exerciseService, TrainerService &_trainerService,
            TraineeService &_traineeService, MessageService &_messageService, User _user)
      : exerciseService(_exerciseService), trainerService(_trainerService),
        traineeService(_traineeService), messageService(_messageService), currentUser(_user) {}

  // --- Trainee Main Menu Screen ---
  ScreenData TraineeMenuScreen()
  {
    system("cls");

    const int startX = 10;
    const int startY = 2;
    const int contentWidth = 50;

    UIHelpers::showDashboardHeader(startX, startY, contentWidth, "Trainee Dashboard", currentUser);

    std::vector<std::string> menuItems = {
        "View Trainers",
        "View Assigned Trainer",
        "View Workout Plan",
        "Browse Exercise Library",
        "My Custom Exercise Lists",
        "Messages",
        "Export Workout Plan",
        "View/Update Profile",
        "Logout"};

    int menuX = startX + 12;
    int menuY = startY + 7;
    int choice = UIHelpers::showMenuScreen(startX, startY, contentWidth, menuX, menuY, menuItems);

    // Navigation
    switch (choice)
    {
    case 0: // View Trainers
      return ViewTrainersScreen();
    case 1: // View Assigned Trainer
      return ViewAssignedTrainerScreen();
    case 2: // View Workout Plan
      return ViewWorkoutPlanScreen();
    case 3: // Browse Exercise Library
      return ScreenData{Screen::ExerciseList, currentUser};
    case 4: // My Custom Exercise Lists
      return ManageCustomListScreen();
    case 5: // Messages
      return MessagesScreen();
    case 6: // Export Workout Plan
      return ExportWorkoutPlanScreen();
    case 7: // View/Update Profile
      return ProfileScreen();
    case 8: // Logout
    default:
      return ScreenData{Screen::Welcome, NoData{}};
    }
  }

  // --- View Trainers Screen ---
  ScreenData ViewTrainersScreen()
  {
    system("cls");

    int startX = 5, startY = 2;
    const int contentWidth = 70;

    // Title
    tui::printAt(startX, startY, "=== Available Trainers ===", 14);
    tui::drawHLine(startX, startY + 1, contentWidth, '=');

    // Search field
    tui::printAt(startX, startY + 3, "Search by name (leave empty for all): ", 7);
    std::vector<int> fieldX = {startX + 40};
    std::vector<int> fieldY = {startY + 3};
    std::vector<int> fieldLen = {30};
    auto textRange = UIHelpers::getTextRange();
    std::vector<char> sRange = {textRange[0]};
    std::vector<char> eRange = {textRange[1]};
    std::vector<std::string> initialData = {""};

    std::vector<std::string> searchResults = tui::addMultiTextField(
        fieldX, fieldY, fieldLen, 1, sRange, eRange, initialData);

    if (searchResults.empty())
    {
      return ScreenData{Screen::TraineeMenu, currentUser};
    }

    std::string keyword = searchResults[0];

    // Get trainers
    std::vector<User> trainers;
    if (keyword.empty())
    {
      trainers = trainerService.getAllTrainers();
    }
    else
    {
      trainers = trainerService.searchTrainersByName(keyword);
    }

    if (trainers.empty())
    {
      tui::printAt(startX, startY + 5, "No trainers found.", 12);
      tui::printAt(startX, startY + 7, "Press any key to return...", 8);
      getch();
      return ScreenData{Screen::TraineeMenu, currentUser};
    }

    // Display trainers in ASCII table
    int tableY = startY + 5;
    tui::printAt(startX, tableY, "Name", 14);
    tui::printAt(startX + 30, tableY, "Availability", 14);
    tui::drawHLine(startX, tableY + 1, contentWidth, '-');

    std::vector<std::string> menuItems;
    for (const auto &trainer : trainers)
    {
      std::string avail = trainer.availability ? "Available" : "Not Available";
      std::string item = trainer.name + " - " + avail;
      menuItems.push_back(item);
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
      return ScreenData{Screen::TraineeMenu, currentUser};
    }

    // View trainer profile
    return ViewTrainerProfileScreen(trainers[choice].id);
  }

  // --- View Trainer Profile Screen ---
  ScreenData ViewTrainerProfileScreen(int trainerId)
  {
    system("cls");

    int startX = 10, startY = 5;
    const int contentWidth = 50;

    auto trainerOpt = trainerService.getTraineeProfile(trainerId);
    if (!trainerOpt.has_value())
    {
      tui::printAt(startX, startY, "Trainer not found!", 12);
      tui::printAt(startX, startY + 2, "Press any key to return...", 8);
      getch();
      return ScreenData{Screen::TraineeMenu, currentUser};
    }

    User trainer = trainerOpt.value();

    // Title
    tui::printAt(startX, startY, "=== Trainer Profile ===", 14);
    tui::drawHLine(startX, startY + 1, contentWidth, '=');

    // Display trainer info
    tui::printAt(startX, startY + 3, "Name: " + trainer.name, 11);
    tui::printAt(startX, startY + 4, "Email: " + trainer.email, 11);
    tui::printAt(startX, startY + 5, "Age: " + std::to_string(trainer.age), 11);
    tui::printAt(startX, startY + 6, "Gender: " + trainer.gender, 11);
    std::string avail = trainer.availability ? "Available" : "Not Available";
    tui::printAt(startX, startY + 7, "Availability: " + avail, 11);

    // Controller hints
    int hintY = startY + 10;
    tui::drawHLine(startX, hintY, contentWidth, '-');
    tui::printAt(startX + 4, hintY + 1, "Press J To Join | Any To Back", 8);

    int key = getch();

    if (key == 'j')
    {
      traineeService.joinTrainer(currentUser.id, trainerId);
    }

    return ScreenData{Screen::TraineeMenu, currentUser};
  }

  // --- View Assigned Trainer Screen ---
  ScreenData ViewAssignedTrainerScreen()
  {
    system("cls");

    int startX = 10, startY = 5;
    const int contentWidth = 50;

    auto trainerOpt = traineeService.getAssignedTrainer(currentUser.id);

    // Title
    tui::printAt(startX, startY, "=== My Assigned Trainer ===", 14);
    tui::drawHLine(startX, startY + 1, contentWidth, '=');

    if (!trainerOpt.has_value())
    {
      tui::printAt(startX, startY + 3, "You don't have an assigned trainer.", 12);
      tui::printAt(startX, startY + 5, "Press any key to return...", 8);
      getch();
      return ScreenData{Screen::TraineeMenu, currentUser};
    }

    User trainer = trainerOpt.value();

    // Display trainer info
    tui::printAt(startX, startY + 3, "Name: " + trainer.name, 11);
    tui::printAt(startX, startY + 4, "Email: " + trainer.email, 11);
    tui::printAt(startX, startY + 5, "Age: " + std::to_string(trainer.age), 11);
    std::string avail = trainer.availability ? "Available" : "Not Available";
    tui::printAt(startX, startY + 6, "Availability: " + avail, 11);

    // Menu options
    std::vector<std::string> menuItems = {"Leave Trainer", "Back"};
    int menuX = startX + 15;
    int menuY = startY + 9;

    int choice = tui::showMenu(menuX, menuY, menuItems);

    if (choice == 0)
    {
      if (traineeService.leaveTrainer(currentUser.id, trainer.id) > 0)
      {
        tui::printAt(startX, startY + 12, "Successfully left trainer.", 10);
      }
      else
      {
        tui::printAt(startX, startY + 12, "Failed to leave trainer.", 12);
      }
      tui::printAt(startX, startY + 13, "Press any key to continue...", 8);
      getch();
    }

    return ScreenData{Screen::TraineeMenu, currentUser};
  }
}