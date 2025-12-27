#pragma once
#include <iostream>
#include <vector>
#include <string>

namespace tui
{
    void gotoxy(int x, int y);
    void textattr(int color);
    void printAt(int x, int y, const std::string &text, int color = 7);
    void drawHLine(int x, int y, int len, char ch = '_');
    void drawVLine(int x, int y, int len, char ch = '|');
    std::vector<char> addTextField(int xPos, int yPos, int length, char sRange, char eRange);
    std::vector<std::string> addMultiTextField(
        std::vector<int> &xPos,
        std::vector<int> &yPos,
        std::vector<int> &length,
        int nLines,
        std::vector<char> &sRange,
        std::vector<char> &eRange,
        std::vector<std::string> data = {});
    int showMenu(int x, int y, const std::vector<std::string> &items);
}
