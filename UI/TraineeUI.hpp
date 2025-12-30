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

  // --- View Workout Plan Screen ---
  ScreenData ViewWorkoutPlanScreen()
  {
    system("cls");

    int startX = 5, startY = 2;
    const int contentWidth = 70;

    // Title
    tui::printAt(startX, startY, "=== My Workout Plan ===", 14);
    tui::drawHLine(startX, startY + 1, contentWidth, '=');

    auto plan = traineeService.getCurrentWorkoutPlan(currentUser.id);

    if (plan.empty())
    {
      tui::printAt(startX, startY + 3, "No workout plan assigned yet.", 12);
      tui::printAt(startX, startY + 5, "Press any key to return...", 8);
      getch();
      return ScreenData{Screen::TraineeMenu, currentUser};
    }

    // Display workout plan
    int y = startY + 3;
    for (const auto &item : plan)
    {
      tui::printAt(startX, y, "Exercise: " + item.exercise_name, 11);
      tui::printAt(startX, y + 1, "Sets: " + std::to_string(item.sets) + "  Reps: " + std::to_string(item.reps), 7);
      // std::string status = item.completed ? "Completed" : "Pending";
      // tui::printAt(startX, y + 2, "Status: " + status, item.completed ? 10 : 14);
      y += 4;
    }

    int hintY = y + 2;
    tui::drawHLine(startX, hintY, contentWidth, '-');
    tui::printAt(startX + 4, hintY + 1, "Press any key to return...", 8);
    getch();

    return ScreenData{Screen::TraineeMenu, currentUser};
  }

  // --- Manage Custom List Screen ---
  ScreenData ManageCustomListScreen()
  {
    system("cls");

    int startX = 5, startY = 2;
    const int contentWidth = 70;

    // Title
    tui::printAt(startX, startY, "=== Manage Custom List ===", 14);
    tui::drawHLine(startX, startY + 1, contentWidth, '=');

    // Get list name (simplified - using default list name)
    std::string listName = "My Exercises";
    auto exercises = traineeService.getCustomListExercises(currentUser.id, listName);

    std::vector<std::string> menuItems;
    for (const auto &ex : exercises)
    {
      menuItems.push_back(ex.exercise_name);
    }
    menuItems.push_back("Add Exercise");
    menuItems.push_back("Delete List");
    menuItems.push_back("Back");

    int menuX = startX + 5;
    int menuY = startY + 4;

    int hintY = menuY + menuItems.size() + 2;
    tui::drawHLine(startX, hintY, contentWidth, '-');
    tui::printAt(startX + 4, hintY + 1, "UP / DOWN : Navigate    ENTER : Select", 8);

    int choice = tui::showMenu(menuX, menuY, menuItems);

    if (choice == -1 || choice == menuItems.size() - 1)
    {
      return ScreenData{Screen::TraineeMenu, currentUser};
    }

    if (choice == menuItems.size() - 3) // Add Exercise
    {
      return AddExerciseToListScreen(listName);
    }
    else if (choice == menuItems.size() - 2) // Delete List
    {
      if (traineeService.deleteCustomList(currentUser.id, listName) > 0)
      {
        tui::printAt(startX, hintY + 3, "List deleted successfully!", 10);
      }
      else
      {
        tui::printAt(startX, hintY + 3, "Failed to delete list.", 12);
      }
      tui::printAt(startX, hintY + 4, "Press any key to continue...", 8);
      getch();
      return ScreenData{Screen::TraineeMenu, currentUser};
    }
    else if (choice < exercises.size()) // Remove exercise
    {
      if (traineeService.removeExerciseFromCustomList(currentUser.id, exercises[choice].exercise_id, listName) > 0)
      {
        tui::printAt(startX, hintY + 3, "Exercise removed from list!", 10);
      }
      else
      {
        tui::printAt(startX, hintY + 3, "Failed to remove exercise.", 12);
      }
      tui::printAt(startX, hintY + 4, "Press any key to continue...", 8);
      getch();
      return ManageCustomListScreen();
    }

    return ScreenData{Screen::TraineeMenu, currentUser};
  }

  // --- Add Exercise to List Screen ---
  ScreenData AddExerciseToListScreen(const std::string &listName)
  {
    system("cls");

    int startX = 5, startY = 2;
    const int contentWidth = 70;

    // Title
    tui::printAt(startX, startY, "=== Add Exercise to List ===", 14);
    tui::drawHLine(startX, startY + 1, contentWidth, '=');

    // Get all exercises
    auto allExercises = exerciseService.getAllExercises();

    std::vector<std::string> menuItems;
    for (const auto &ex : allExercises)
    {
      menuItems.push_back(ex.name);
    }
    menuItems.push_back("Back");

    int menuX = startX + 5;
    int menuY = startY + 4;

    int choice = tui::showMenu(menuX, menuY, menuItems);

    if (choice == -1 || choice == menuItems.size() - 1)
    {
      return ManageCustomListScreen();
    }

    if (traineeService.addExerciseToCustomList(currentUser.id, allExercises[choice].id, listName) > 0)
    {
      tui::printAt(startX, menuY + menuItems.size() + 2, "Exercise added to list!", 10);
    }
    else
    {
      tui::printAt(startX, menuY + menuItems.size() + 2, "Failed to add exercise.", 12);
    }
    tui::printAt(startX, menuY + menuItems.size() + 3, "Press any key to continue...", 8);
    getch();

    return ManageCustomListScreen();
  }

  // --- Create Custom List Screen ---
  ScreenData CreateCustomListScreen()
  {
    system("cls");

    int startX = 10, startY = 5;
    int fieldLen = 25;

    // Title
    tui::printAt(startX, startY, "=== Create Custom List ===", 14);

    // Note: Since UserWorkout doesn't support list names, this is simplified
    tui::printAt(startX, startY + 2, "Note: Custom lists are managed through your exercise collection.", 7);
    tui::printAt(startX, startY + 4, "Press any key to return...", 8);
    getch();

    return ScreenData{Screen::TraineeMenu, currentUser};
  }

  // --- Messages Screen ---
  ScreenData MessagesScreen()
  {
    system("cls");

    int startX = 5, startY = 2;
    const int contentWidth = 70;

    // Title
    tui::printAt(startX, startY, "=== Messages ===", 14);
    tui::drawHLine(startX, startY + 1, contentWidth, '=');

    std::vector<std::string> menuItems = {
        "View Inbox",
        "Send Message to Trainer",
        "Back"};

    int menuX = startX + 15;
    int menuY = startY + 4;

    int choice = tui::showMenu(menuX, menuY, menuItems);

    switch (choice)
    {
    case 0: // View Inbox
      return ViewInboxScreen();
    case 1: // Send Message
      return SendMessageScreen();
    default:
      return ScreenData{Screen::TraineeMenu, currentUser};
    }
  }

  // --- View Inbox Screen ---
  ScreenData ViewInboxScreen()
  {
    system("cls");

    int startX = 5, startY = 2;
    const int contentWidth = 70;

    // Title
    tui::printAt(startX, startY, "=== Inbox ===", 14);
    tui::drawHLine(startX, startY + 1, contentWidth, '=');

    auto messages = messageService.getInbox(currentUser.id);

    if (messages.empty())
    {
      tui::printAt(startX, startY + 3, "No messages.", 12);
      tui::printAt(startX, startY + 5, "Press any key to return...", 8);
      getch();
      return MessagesScreen();
    }

    // Display messages
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

    return MessagesScreen();
  }

  // --- Send Message Screen ---
  ScreenData SendMessageScreen()
  {
    system("cls");

    int startX = 10, startY = 5;
    int fieldLen = 40;

    // Title
    tui::printAt(startX, startY, "=== Send Message ===", 14);

    // Get assigned trainer
    auto trainerOpt = traineeService.getAssignedTrainer(currentUser.id);
    if (!trainerOpt.has_value())
    {
      tui::printAt(startX, startY + 2, "You don't have an assigned trainer.", 12);
      tui::printAt(startX, startY + 4, "Press any key to return...", 8);
      getch();
      return MessagesScreen();
    }

    User trainer = trainerOpt.value();
    tui::printAt(startX, startY + 2, "To: " + trainer.name, 11);
    tui::printAt(startX, startY + 4, "Message:", 7);

    // Get message content
    std::vector<int> fieldX = {startX + 10};
    std::vector<int> fieldY = {startY + 4};
    std::vector<int> fieldLenVec = {fieldLen};
    std::vector<char> sRange = {' '};
    std::vector<char> eRange = {'~'};
    std::vector<std::string> initialData = {""};

    std::vector<std::string> results = tui::addMultiTextField(
        fieldX, fieldY, fieldLenVec, 1, sRange, eRange, initialData);

    if (results.empty() || results[0].empty())
    {
      return MessagesScreen();
    }

    if (messageService.sendMessage(currentUser.id, trainer.id, results[0]) > 0)
    {
      tui::printAt(startX, startY + 7, "Message sent successfully!", 10);
    }
    else
    {
      tui::printAt(startX, startY + 7, "Failed to send message.", 12);
    }
    tui::printAt(startX, startY + 8, "Press any key to continue...", 8);
    getch();

    return MessagesScreen();
  }
}