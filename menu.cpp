#include <iostream>
#include <vector>
#include <conio.h>
#include "menu.h"
#include "utils.h"

static const std::vector<const char*> options = {
    "Start a new game",
    "Present instructions and keys",
    "EXIT"
};

static void DrawMenu(int selected)
{
    clearScreen();
    hideCursor();
    const int startX = 10;
    const int startY = 5;
    for (size_t i = 0; i < options.size(); ++i)
    {
        gotoxy(startX, startY + (int)i);
        if ((int)i == selected) std::cout << "> " << options[i];
        else std::cout << "  " << options[i];
    }
    gotoxy(startX, startY + (int)options.size() + 1);
    std::cout << "Use arrows / 1 / 8 / 9 / Enter / ESC";
}

static void PrintInstructionScreen()
{
    clearScreen();
    gotoxy(0, 0);
    std::cout << "Instructions / Keys:\n";
    std::cout << "Movment : ";
    std::cout << "W - UP / A - LEFT / S - DOWN / D - RIGHT / E = STAY\n";
    std::cout << "Interactions : \n";
    std::cout << "Dispose Element - E\n";
    std::cout << "Pause Game - H\n";
    std::cout << "\nPress any key to return to the menu.";
    _getch();
}

MenuOptions runMenu()
{
    int selected = 0;
    while (true) {
        DrawMenu(selected);
        int ch = _getch();
        if (ch == 224 || ch == 0) { // arrow keys
            int code = _getch();
            if (code == 72) { // up
                selected = (selected - 1 + (int)options.size()) % (int)options.size();
            } else if (code == 80) { // down
                selected = (selected + 1) % (int)options.size();
            }
        } else if (ch == 13) { // Enter
            if (selected == 0) return MENU_START;
            if (selected == 1) { PrintInstructionScreen(); continue; }
            return MENU_EXIT;
        } else if (ch == 27) { // ESC
            return MENU_EXIT;
        } else if (ch == '1') {
            return MENU_START;
        } else if (ch == '8') {
            PrintInstructionScreen();
        } else if (ch == '9') {
            return MENU_EXIT;
        }
    }
}

















