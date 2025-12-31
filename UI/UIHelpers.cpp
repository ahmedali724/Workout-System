#include "UIHelpers.hpp"
#include "tuiUtils.hpp"

namespace UIHelpers {
    ScreenData routeToUserMenu(const User& user) {
        if (user.role == "Trainee")
            return ScreenData{Screen::TraineeMenu, user};
        else if (user.role == "Trainer")
            return ScreenData{Screen::TrainerMenu, user};
        else if (user.role == "Admin")
            return ScreenData{Screen::AdminMenu, user};
        return ScreenData{Screen::Welcome, NoData{}};
    }

    void showDashboardHeader(int startX, int startY, int contentWidth, const std::string& title, const User& user) {
        tui::printAt(startX, startY, "=== " + title + " ===", 14);
        tui::drawHLine(startX, startY + 1, contentWidth, '=');
        tui::printAt(startX, startY + 3, "Welcome, " + user.name + "!", 11);
        tui::printAt(startX, startY + 4, "Role: " + user.role, 7);
    }

    int showMenuScreen(int startX, int startY, int contentWidth, int menuX, int menuY, 
                              const std::vector<std::string>& menuItems) {
        int hintY = menuY + menuItems.size() + 2;
        tui::drawHLine(startX, hintY, contentWidth, '-');
        tui::printAt(startX + 4, hintY + 1, "UP / DOWN : Navigate    ENTER : Select", 8);
        return tui::showMenu(menuX, menuY, menuItems);
    }

    std::vector<char> getTextRange() {
        return {' ', '~'};
    }

    std::vector<char> getAgeRange() {
        return {'0', '9'};
    }

    std::vector<char> getAvailabilityRange() {
        return {'0', '1'};
    }

    bool validateAge(const std::string& ageStr, int& age) {
        if (ageStr.empty() || ageStr.length() > 1) return false;
        if (ageStr[0] < '0' || ageStr[0] > '9') return false;
        age = ageStr[0] - '0';
        return true;
    }

    bool validateAvailability(const std::string& availStr, bool& availability) {
        if (availStr.empty() || availStr.length() > 1) return false;
        if (availStr[0] != '0' && availStr[0] != '1') return false;
        availability = (availStr[0] == '1');
        return true;
    }

    void showError(int errX, int errY, const std::string& message) {
        tui::drawHLine(errX, errY, 60, ' ');
        tui::printAt(errX, errY, message, 12);
    }

    void showInstructions(int x, int y) {
        tui::printAt(x, y, "UP/DOWN: Navigate   ENTER: Submit", 8);
        tui::drawHLine(x, y + 1, 50, '-');
    }
}

