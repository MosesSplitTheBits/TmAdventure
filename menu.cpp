#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include "menu.h"
#include "Settings.h"
#include "utils.h"
#include "Sound.h"

static std::vector<std::string> BuildMenuOptions()
{
    std::vector<std::string> options;
    options.emplace_back("Start a new game");
    options.emplace_back("Present instructions and keys");
    options.emplace_back(std::string("Colors: ") + (gSettings.useColors ? "[X]" : "[ ]"));
    options.emplace_back(std::string("Sound : ") + (gSettings.useSound ? "[X]" : "[ ]"));
    options.emplace_back("EXIT");
    return options;
}

static void DrawMenu(int selected)
{
    clearScreen();
    hideCursor();
    const int startX = 10;
    const int startY = 5;

    const auto options = BuildMenuOptions();
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
        const auto options = BuildMenuOptions();
        DrawMenu(selected);
        int ch = _getch();
        if (ch == 224 || ch == 0) { // arrow keys
            int code = _getch();
            if (code == 72) { // up
                selected = (selected - 1 + (int)options.size()) % (int)options.size();
                Sound::TickMenuNav();
            } else if (code == 80) { // down
                selected = (selected + 1) % (int)options.size();
                Sound::TickMenuNav();
            }
        } else if (ch == 13) { // Enter
            if (selected == 0) { Sound::TickMenuSelect(); return MENU_START; }
            if (selected == 1) { Sound::TickMenuSelect(); PrintInstructionScreen(); continue; }
            if (selected == 2) { gSettings.useColors = !gSettings.useColors; Sound::TickMenuToggle(); continue; }
            if (selected == 3) { gSettings.useSound  = !gSettings.useSound;  Sound::TickMenuToggle(); continue; }
            Sound::TickMenuSelect();
            return MENU_EXIT;
        } else if (ch == 27) { // ESC
            return MENU_EXIT;
        } else if (ch == '1') {
            Sound::TickMenuSelect();
            return MENU_START;
        } else if (ch == '8') {
            Sound::TickMenuSelect();
            PrintInstructionScreen();
        } else if (ch == '9') {
            Sound::TickMenuSelect();
            return MENU_EXIT;
        }
    }
}

















