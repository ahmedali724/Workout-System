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
}