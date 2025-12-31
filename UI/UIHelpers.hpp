#pragma once
#include "ScreenData.hpp"
#include "User.hpp"
#include <vector>
#include <string>

namespace UIHelpers {
    ScreenData routeToUserMenu(const User& user);
    void showDashboardHeader(int startX, int startY, int contentWidth, const std::string& title, const User& user);
    int showMenuScreen(int startX, int startY, int contentWidth, int menuX, int menuY, 
                      const std::vector<std::string>& menuItems);
    std::vector<char> getTextRange();
    std::vector<char> getAgeRange();
    std::vector<char> getAvailabilityRange();
    bool validateAge(const std::string& ageStr, int& age);
    bool validateAvailability(const std::string& availStr, bool& availability);
    void showError(int errX, int errY, const std::string& message);
    void showInstructions(int x, int y);
}

