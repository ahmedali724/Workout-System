#include "tuiUtils.hpp"
#include "ScreenData.hpp"
#include "AdminService.hpp"
#include "User.hpp"
#include "UIHelpers.hpp"
#include <vector>
#include <string>
#include <conio.h>

class AdminUI
{
public:
    AdminService &adminService;
    User currentUser;

    AdminUI(AdminService &_adminService, User _user)
        : adminService(_adminService), currentUser(_user) {}

    // --- Admin Main Menu Screen ---
    ScreenData AdminMenuScreen()
    {
        system("cls");

        const int startX = 10;
        const int startY = 2;
        const int contentWidth = 50;

        UIHelpers::showDashboardHeader(startX, startY, contentWidth, "Admin Dashboard", currentUser);

        std::vector<std::string> menuItems = {
            "View All Users",
            "Search Users",
            "Create Admin Account",
            "Generate Reports",
            "View/Update Profile",
            "Logout"};

        int menuX = startX + 12;
        int menuY = startY + 7;
        int choice = UIHelpers::showMenuScreen(startX, startY, contentWidth, menuX, menuY, menuItems);

        // Navigation
        switch (choice)
        {
        case 0: // View All Users
            return ViewAllUsersScreen();
        case 1: // Search Users
            return SearchUsersScreen();
        case 2: // Create Admin Account
            return CreateAdminScreen();
        case 3: // Generate Reports
            return GenerateReportsScreen();
        case 4: // View/Update Profile
            return ProfileScreen();
        case 5: // Logout
        default:
            return ScreenData{Screen::Welcome, NoData{}};
        }
    }

    // --- View All Users Screen ---
    ScreenData ViewAllUsersScreen()
    {
        system("cls");

        int startX = 5, startY = 2;
        const int contentWidth = 70;

        // Title
        tui::printAt(startX, startY, "=== All Users ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        auto users = adminService.getAllUsers();

        if (users.empty())
        {
            tui::printAt(startX, startY + 3, "No users found.", 12);
            tui::printAt(startX, startY + 5, "Press any key to return...", 8);
            getch();
            return ScreenData{Screen::AdminMenu, currentUser};
        }

        // Display users in ASCII table
        int tableY = startY + 3;
        tui::printAt(startX, tableY, "Name", 14);
        tui::printAt(startX + 25, tableY, "Role", 14);
        tui::printAt(startX + 35, tableY, "Email", 14);
        tui::drawHLine(startX, tableY + 1, contentWidth, '-');

        std::vector<std::string> menuItems;
        for (const auto &user : users)
        {
            menuItems.push_back(user.name + " - " + user.role);
        }
        menuItems.push_back("Back");

        int menuX = startX + 5;
        int menuY = tableY + 3;

        int hintY = menuY + menuItems.size() + 2;
        tui::drawHLine(startX, hintY, contentWidth, '-');
        tui::printAt(startX + 4, hintY + 1, "UP / DOWN : Navigate    ENTER : View/Edit    ESC : Back", 8);

        int choice = tui::showMenu(menuX, menuY, menuItems);

        if (choice == -1 || choice == menuItems.size() - 1)
        {
            return ScreenData{Screen::AdminMenu, currentUser};
        }

        // View/Edit user
        return ViewEditUserScreen(users[choice].id);
    }

    // --- Search Users Screen ---
    ScreenData SearchUsersScreen()
    {
        system("cls");

        int startX = 5, startY = 2;
        const int contentWidth = 70;

        // Title
        tui::printAt(startX, startY, "=== Search Users ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        tui::printAt(startX, startY + 3, "Search by name:", 7);

        std::vector<int> fieldX = {startX + 18};
        std::vector<int> fieldY = {startY + 3};
        std::vector<int> fieldLen = {30};
        auto textRange = UIHelpers::getTextRange();
        std::vector<char> sRange = {textRange[0]};
        std::vector<char> eRange = {textRange[1]};
        std::vector<std::string> initialData = {""};

        std::vector<std::string> results = tui::addMultiTextField(
            fieldX, fieldY, fieldLen, 1, sRange, eRange, initialData);

        if (results.empty())
        {
            return ScreenData{Screen::AdminMenu, currentUser};
        }

        std::string keyword = results[0];
        auto users = adminService.searchUsersByName(keyword);

        if (users.empty())
        {
            tui::printAt(startX, startY + 5, "No users found.", 12);
            tui::printAt(startX, startY + 7, "Press any key to return...", 8);
            getch();
            return ScreenData{Screen::AdminMenu, currentUser};
        }

        // Display results
        std::vector<std::string> menuItems;
        for (const auto &user : users)
        {
            menuItems.push_back(user.name + " - " + user.role);
        }
        menuItems.push_back("Back");

        int menuX = startX + 5;
        int menuY = startY + 5;

        int choice = tui::showMenu(menuX, menuY, menuItems);

        if (choice == -1 || choice == menuItems.size() - 1)
        {
            return ScreenData{Screen::AdminMenu, currentUser};
        }

        return ViewEditUserScreen(users[choice].id);
    }

    // --- View/Edit User Screen ---
    ScreenData ViewEditUserScreen(int userId)
    {
        system("cls");

        int startX = 10, startY = 5;
        const int contentWidth = 50;

        auto userOpt = adminService.getUserById(userId);
        if (!userOpt.has_value())
        {
            tui::printAt(startX, startY, "User not found!", 12);
            tui::printAt(startX, startY + 2, "Press any key to return...", 8);
            getch();
            return ScreenData{Screen::AdminMenu, currentUser};
        }

        User user = userOpt.value();

        // Title
        tui::printAt(startX, startY, "=== User Details ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        // Display user info
        tui::printAt(startX, startY + 3, "ID: " + std::to_string(user.id), 11);
        tui::printAt(startX, startY + 4, "Username: " + user.username, 11);
        tui::printAt(startX, startY + 5, "Email: " + user.email, 11);
        tui::printAt(startX, startY + 6, "Name: " + user.name, 11);
        tui::printAt(startX, startY + 7, "Age: " + std::to_string(user.age), 11);
        tui::printAt(startX, startY + 8, "Gender: " + user.gender, 11);
        tui::printAt(startX, startY + 9, "Role: " + user.role, 11);
        std::string avail = user.availability ? "Available" : "Not Available";
        tui::printAt(startX, startY + 10, "Availability: " + avail, 11);

        // Menu options
        std::vector<std::string> menuItems = {"Edit User", "Delete User", "Back"};
        int menuX = startX + 15;
        int menuY = startY + 13;

        int choice = tui::showMenu(menuX, menuY, menuItems);

        switch (choice)
        {
        case 0: // Edit User
            return EditUserScreen(user);
        case 1: // Delete User
            return DeleteUserScreen(userId);
        default:
            return ScreenData{Screen::AdminMenu, currentUser};
        }
    }

    // --- Edit User Screen ---
    ScreenData EditUserScreen(User &user)
    {
        system("cls");

        int startX = 10, startY = 4;
        int fieldLen = 22;

        // Title
        tui::printAt(startX, startY, "=== Edit User ===", 14);

        // Labels
        tui::printAt(startX, startY + 2, "Username:", 7);
        tui::printAt(startX, startY + 4, "Password:", 7);
        tui::printAt(startX, startY + 6, "Email:", 7);
        tui::printAt(startX, startY + 8, "Name:", 7);
        tui::printAt(startX, startY + 10, "Age:", 7);
        tui::printAt(startX, startY + 12, "Gender (Male/Female):", 7);
        tui::printAt(startX, startY + 14, "Availability (1/0):", 7);

        // Error message area
        int errX = startX;
        int errY = startY + 18;
        std::string errorMessage = "";

        // Controller instructions
        tui::printAt(startX, errY + 2, "UP/DOWN: Navigate   ENTER: Submit   ESC: Cancel", 8);
        tui::drawHLine(startX, errY + 3, 50, '-');

        // Pre-fill with current data
        std::vector<std::string> initialData = {
            user.username,
            user.password,
            user.email,
            user.name,
            std::to_string(user.age),
            user.gender,
            std::to_string(user.availability ? 1 : 0)};

        // Multi-field configuration
        std::vector<int> fieldX(7, startX + 30);
        std::vector<int> fieldY = {
            startY + 2, startY + 4, startY + 6, startY + 8,
            startY + 10, startY + 12, startY + 14};
        std::vector<int> fieldLenVec(8, fieldLen);
        auto textRange = UIHelpers::getTextRange();
        auto ageRange = UIHelpers::getAgeRange();
        auto availRange = UIHelpers::getAvailabilityRange();
        std::vector<char> sRange = {textRange[0], textRange[0], textRange[0], textRange[0], ageRange[0], textRange[0],availRange[0]};
        std::vector<char> eRange = {textRange[1], textRange[1], textRange[1], textRange[1], ageRange[1], textRange[1],availRange[1]};

        while (true)
        {
            std::vector<std::string> results = tui::addMultiTextField(
                fieldX, fieldY, fieldLenVec, 7, sRange, eRange, initialData);

            if (results.empty())
            {
                return ScreenData{Screen::AdminMenu, currentUser};
            }

            initialData = results;

            int age;
            bool availability;
            if (!UIHelpers::validateAge(results[4], age))
            {
                errorMessage = "Age must be 12 or more !";
            }
            else if (!UIHelpers::validateAvailability(results[6], availability))
            {
                errorMessage = "Availability must be 0 or 1!";
            }
            else
            {
                User updatedUser(
                    user.id,
                    results[0],
                    results[1],
                    results[2],
                    results[3],
                    age,
                    results[5],
                    user.role,
                    availability);

                if (adminService.updateUser(updatedUser) > 0)
                {
                    tui::printAt(startX, errY, "User updated successfully!", 10);
                    tui::printAt(startX, errY + 1, "Press any key to continue...", 8);
                    getch();
                    return ScreenData{Screen::AdminMenu, currentUser};
                }
                else
                {
                    errorMessage = "Update failed. Check inputs please!";
                }
            }

            // Clear & show error
            tui::drawHLine(errX, errY, 60, ' ');
            tui::printAt(errX, errY, errorMessage, 12);
        }
    }

    // --- Delete User Screen ---
    ScreenData DeleteUserScreen(int userId)
    {
        system("cls");

        int startX = 10, startY = 5;
        const int contentWidth = 50;

        // Title
        tui::printAt(startX, startY, "=== Delete User ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        tui::printAt(startX, startY + 3, "Are you sure you want to delete this user?", 12);
        tui::printAt(startX, startY + 4, "This action cannot be undone.", 12);

        // Menu options
        std::vector<std::string> menuItems = {"Yes, Delete", "Cancel"};

        int menuX = startX + 15;
        int menuY = startY + 7;

        int choice = tui::showMenu(menuX, menuY, menuItems);

        if (choice == 0)
        {
            if (adminService.deleteUser(userId) > 0)
            {
                tui::printAt(startX, startY + 10, "User deleted successfully!", 10);
            }
            else
            {
                tui::printAt(startX, startY + 10, "Failed to delete user.", 12);
            }
            tui::printAt(startX, startY + 11, "Press any key to continue...", 8);
            getch();
        }

        return ScreenData{Screen::AdminMenu, currentUser};
    }

    // --- Create Admin Screen ---
    ScreenData CreateAdminScreen()
    {
        system("cls");

        int startX = 10, startY = 4;
        int fieldLen = 22;

        // Title
        tui::printAt(startX, startY, "=== Create Admin Account ===", 14);

        // Labels
        tui::printAt(startX, startY + 2, "Username:", 7);
        tui::printAt(startX, startY + 4, "Password:", 7);
        tui::printAt(startX, startY + 6, "Email:", 7);
        tui::printAt(startX, startY + 8, "Name:", 7);
        tui::printAt(startX, startY + 10, "Age:", 7);
        tui::printAt(startX, startY + 12, "Gender (Male/Female):", 7);
        tui::printAt(startX, startY + 14, "Availability (1/0):", 7);

        // Error message area
        int errX = startX;
        int errY = startY + 16;
        std::string errorMessage = "";

        // Controller instructions
        tui::printAt(startX, errY + 2, "UP/DOWN: Navigate   ENTER: Submit   ESC: Cancel", 8);
        tui::drawHLine(startX, errY + 3, 50, '-');

        // Multi-field configuration
        std::vector<int> fieldX(7, startX + 26);
        std::vector<int> fieldY = {
            startY + 2, startY + 4, startY + 6, startY + 8,
            startY + 10, startY + 12, startY + 14};
        std::vector<int> fieldLenVec(7, fieldLen);
        auto textRange = UIHelpers::getTextRange();
        auto ageRange = UIHelpers::getAgeRange();
        auto availRange = UIHelpers::getAvailabilityRange();
        std::vector<char> sRange = {textRange[0], textRange[0], textRange[0], textRange[0], ageRange[0], textRange[0], availRange[0]};
        std::vector<char> eRange = {textRange[1], textRange[1], textRange[1], textRange[1], ageRange[1], textRange[1], availRange[1]};
        std::vector<std::string> initialData = {};

        while (true)
        {
            std::vector<std::string> results = tui::addMultiTextField(
                fieldX, fieldY, fieldLenVec, 7, sRange, eRange, initialData);

            if (results.empty())
            {
                return ScreenData{Screen::AdminMenu, currentUser};
            }

            initialData = results;

            int age;
            bool availability;
            if (!UIHelpers::validateAge(results[4], age))
            {
                errorMessage = "Age must be 12 or more !";
            }
            else if (!UIHelpers::validateAvailability(results[6], availability))
            {
                errorMessage = "Availability must be 0 or 1!";
            }
            else
            {
                User newAdmin(
                    0,
                    results[0],
                    results[1],
                    results[2],
                    results[3],
                    age,
                    results[5],
                    "Admin",
                    availability);

                if (adminService.createAdmin(newAdmin) > 0)
                {
                    tui::printAt(startX, errY, "Admin account created successfully!", 10);
                    tui::printAt(startX, errY + 1, "Press any key to continue...", 8);
                    getch();
                    return ScreenData{Screen::AdminMenu, currentUser};
                }
                else
                {
                    errorMessage = "Creation failed. Check inputs please!";
                }
            }

            // Clear & show error
            tui::drawHLine(errX, errY, 60, ' ');
            tui::printAt(errX, errY, errorMessage, 12);
        }
    }

    // --- Generate Reports Screen ---
    ScreenData GenerateReportsScreen()
    {
        system("cls");

        int startX = 10, startY = 5;
        const int contentWidth = 50;

        // Title
        tui::printAt(startX, startY, "=== System Reports ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        auto report = adminService.getReportData();

        // Display report
        tui::printAt(startX, startY + 3, "Total Trainees: " + std::to_string(report.totalTrainees), 11);
        tui::printAt(startX, startY + 4, "Total Trainers: " + std::to_string(report.totalTrainers), 11);
        tui::printAt(startX, startY + 5, "Total Admins: " + std::to_string(report.totalAdmins), 11);
        tui::printAt(startX, startY + 6, "Total Trainee-Trainer Pairs: " + std::to_string(report.totalPairs), 11);

        // Controller hints
        int hintY = startY + 9;
        tui::drawHLine(startX, hintY, contentWidth, '-');
        tui::printAt(startX + 4, hintY + 1, "Press any key to return...", 8);

        getch();
        return ScreenData{Screen::AdminMenu, currentUser};
    }

    // --- Profile Screen ---
    ScreenData ProfileScreen()
    {
        system("cls");

        int startX = 10, startY = 5;
        const int contentWidth = 50;

        // Title
        tui::printAt(startX, startY, "=== My Profile ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');

        // Display user info
        tui::printAt(startX, startY + 3, "Name: " + currentUser.name, 11);
        tui::printAt(startX, startY + 4, "Username: " + currentUser.username, 11);
        tui::printAt(startX, startY + 5, "Email: " + currentUser.email, 11);
        tui::printAt(startX, startY + 6, "Age: " + std::to_string(currentUser.age), 11);
        tui::printAt(startX, startY + 7, "Gender: " + currentUser.gender, 11);
        tui::printAt(startX, startY + 8, "Role: " + currentUser.role, 11);

        // Menu options
        std::vector<std::string> menuItems = {"Update Profile", "Change Password", "Back"};
        int menuX = startX + 15;
        int menuY = startY + 11;

        int choice = tui::showMenu(menuX, menuY, menuItems);

        switch (choice)
        {
        case 0: // Update Profile
            return ScreenData{Screen::UpdateProfile, currentUser};
        case 1: // Change Password
            return ScreenData{Screen::ChangePassword, currentUser};
        default:
            return ScreenData{Screen::AdminMenu, currentUser};
        }
    }
};
