#include "tuiUtils.hpp"
#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

/* ================= Console Helpers ================= */

namespace tui
{
    void gotoxy(int x, int y)
    {
        COORD c = {(SHORT)x, (SHORT)y};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    }

    void textattr(int color)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    void printAt(int x, int y, const string &text, int color)
    {
        textattr(color);
        gotoxy(x, y);
        cout << text;
        textattr(7);
    }

    /* ================= ASCII Lines ================= */

    void drawHLine(int x, int y, int len, char ch)
    {
        gotoxy(x, y);
        for (int i = 0; i < len; i++)
            cout << ch;
    }

    void drawVLine(int x, int y, int len, char ch)
    {
        for (int i = 0; i < len; i++)
        {
            gotoxy(x, y + i);
            cout << ch;
        }
    }

    /* ================= Text Editing Helpers ================= */

    void insertChar(vector<char> &text, int cursor, char key, int maxLen)
    {
        if (text.size() >= maxLen)
            return;
        text.insert(text.begin() + cursor, key);
    }

    void deleteChar(vector<char> &text, int cursor)
    {
        if (cursor < 0 || cursor >= text.size())
            return;
        text.erase(text.begin() + cursor);
    }

    /* ================= Single Text Field ================= */

    void insertCharAt(vector<char> &text, int &size, int maxSize, int cursor, char key)
    {
        if (size >= maxSize)
            return;

        text.insert(text.begin() + cursor, key);
        size++;
    }

    void deleteCharAt(vector<char> &text, int &size, int cursor)
    {
        if (cursor < 0 || cursor >= size)
            return;

        text.erase(text.begin() + cursor);
        size--;
    }

    void updateScreenAfterChange(const vector<char> &text, int cursor, int startX, int startY)
    {
        gotoxy(startX + cursor, startY);
        textattr(79);

        for (int i = cursor; i < text.size(); i++)
            cout << text[i];

        cout << " ";
        textattr(7);
    }

    void updateScreenAfterChange(const string &text, int cursor, int startX, int startY)
    {
        gotoxy(startX + cursor, startY);
        textattr(79);

        for (int i = cursor; i < (int)text.size(); i++)
            cout << text[i];

        cout << " ";
        textattr(7);
    }

    /* ================= Single Text Field ================= */

    vector<char> addTextField(int xPos, int yPos, int length, char sRange, char eRange)
    {
        vector<char> text;
        int size = 0;
        int cursor = 0;

        gotoxy(xPos, yPos);
        textattr(79);
        for (int i = 0; i < length; i++)
            cout << " ";
        textattr(7);

        while (true)
        {
            gotoxy(xPos + cursor, yPos);
            char key = getch();

            if (key == -32)
            {
                key = getch();

                if (key == 75 && cursor > 0)
                    cursor--;
                else if (key == 77 && cursor < size)
                    cursor++;
                else if (key == 71)
                    cursor = 0;
                else if (key == 79)
                    cursor = size;

                else if (key == 83)
                {
                    if (cursor < size)
                    {
                        deleteCharAt(text, size, cursor);
                        updateScreenAfterChange(text, cursor, xPos, yPos);
                    }
                }
            }
            else if (key == 8)
            {
                if (cursor > 0)
                {
                    cursor--;
                    deleteCharAt(text, size, cursor);
                    updateScreenAfterChange(text, cursor, xPos, yPos);
                }
            }
            else if (key == 13)
            {
                return text;
            }
            else if (key >= sRange && key <= eRange)
            {
                if (size < length)
                {
                    insertCharAt(text, size, length, cursor, key);
                    updateScreenAfterChange(text, cursor, xPos, yPos);
                    cursor++;
                }
            }
        }
    }

    /* ================= Multi Text Field ================= */

    vector<string> addMultiTextField(
        vector<int> &xPos,
        vector<int> &yPos,
        vector<int> &length,
        int nLines,
        vector<char> &sRange,
        vector<char> &eRange,
        vector<string> data)

    {
        vector<string> dataLines(nLines);

        int cursor = 0;
        int currentLine = 0;

        for (int i = 0; i < nLines; i++)
        {
            gotoxy(xPos[i], yPos[i]);
            textattr(79);
            for (int j = 0; j < length[i]; j++)
                cout << " ";
            textattr(7);
        }

        for (int i = 0; i < (int)data.size() && i < nLines; i++)
        {
            dataLines[i] = data[i];
            updateScreenAfterChange(dataLines[i], 0, xPos[i], yPos[i]);
        }

        cursor = 0;
        currentLine = 0;

        while (true)
        {
            gotoxy(xPos[currentLine] + cursor, yPos[currentLine]);
            char key = getch();

            if (key == -32)
            {
                key = getch();

                if (key == 75)
                {
                    if (cursor == 0)
                    {
                        continue;
                    }
                    else
                        cursor--;
                }
                else if (key == 77)
                {
                    if (cursor == (int)dataLines[currentLine].size())
                    {
                        continue;
                    }
                    else
                        cursor++;
                }
                else if (key == 71)
                    cursor = 0;
                else if (key == 79)
                    cursor = (int)dataLines[currentLine].size();

                else if (key == 72)
                {
                    cursor = 0;
                    currentLine = (currentLine - 1 + nLines) % nLines;
                }
                else if (key == 80)
                {
                    cursor = 0;
                    currentLine = (currentLine + 1) % nLines;
                }

                else if (key == 83)
                {
                    if (cursor < (int)dataLines[currentLine].size())
                    {
                        dataLines[currentLine].erase(dataLines[currentLine].begin() + cursor);
                        updateScreenAfterChange(dataLines[currentLine], cursor,
                                                xPos[currentLine], yPos[currentLine]);
                    }
                }
            }
            else if (key == 8)
            {
                if (cursor > 0)
                {
                    cursor--;
                    dataLines[currentLine].erase(dataLines[currentLine].begin() + cursor);
                    updateScreenAfterChange(dataLines[currentLine], cursor,
                                            xPos[currentLine], yPos[currentLine]);
                }
            }
            else if (key == 9)
            {
                currentLine = (currentLine + 1) % nLines;
                cursor = 0;
            }
            else if (key == 13)
            {
                return dataLines;
            }

            else if (key == 27)
            {
                return {};
            }
            else if (key >= sRange[currentLine] && key <= eRange[currentLine])
            {
                if ((int)dataLines[currentLine].size() < length[currentLine])
                {
                    dataLines[currentLine].insert(dataLines[currentLine].begin() + cursor, key);

                    updateScreenAfterChange(dataLines[currentLine], cursor,
                                            xPos[currentLine], yPos[currentLine]);
                    cursor++;
                }
            }
        }
    }
    /* ================= MENU ================= */

    int showMenu(int x, int y, const vector<string> &items)
    {
        int selected = 0;

        while (true)
        {
            for (int i = 0; i < items.size(); i++)
            {
                gotoxy(x, y + i);
                textattr(i == selected ? 112 : 7);
                cout << items[i] << "   ";
            }

            char key = getch();
            if (key == -32)
            {
                key = getch();
                if (key == 72)
                    selected = (selected - 1 + items.size()) % items.size();
                else if (key == 80)
                    selected = (selected + 1) % items.size();
            }
            else if (key == 13)
                return selected;
        }
    }
}
