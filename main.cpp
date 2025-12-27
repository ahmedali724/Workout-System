#include <iostream>
#include "tuiUtils.hpp"
#include <conio.h>

using namespace std;

using namespace tui;

/* ================= MAIN ================= */

int main()
{
    system("cls");

    printAt(20, 1, "=== CONSOLE UI SYSTEM ===", 11);

    /* ----- Single Text Field ----- */
    printAt(5, 3, "ID:");
    vector<char> idVec = addTextField(15, 3, 10, '0', '9');
    drawHLine(20,20,30);
    /* ----- MULTI TEXT FIELD ----- */
    printAt(5, 5, "Name:");
    printAt(5, 7, "Age:");
    printAt(5, 9, "City:");

    vector<int> xPos = {15, 50, 15};
    vector<int> yPos = {5, 5, 9};
    vector<int> length = {20, 3, 15};
    vector<string> data = {"1", "2", "3"};

    vector<char> sRange = {'A', '0', 'A'};
    vector<char> eRange = {'z', '9', 'z'};

    vector<string> dataVec =
        addMultiTextField(xPos, yPos, length, 3, sRange, eRange, data);

    /* ----- Menu ----- */
    vector<string> menu = {
        "Save",
        "Edit",
        "Exit"};

    int choice = showMenu(5, 12, menu);

    system("cls");

    // convert vectors to strings for display
    string id(idVec.begin(), idVec.end());
    string name = dataVec[0];
    string age = dataVec[1];
    string city = dataVec[2];

    printAt(5, 3, "ID   : " + id);
    printAt(5, 4, "Name : " + name);
    printAt(5, 5, "Age  : " + age);
    printAt(5, 6, "City : " + city);
    printAt(5, 8, "Menu : " + menu[choice], 10);

    getch();
    return 0;
}




