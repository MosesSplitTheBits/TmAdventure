#include "Game.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <string>
#include "utils.h"
#include "Screen.h"
#include "Point.h"
#include "Direction.h"
#include "Player.h"
#include "Game.h"
#include "Puzzle.h"
#include "Key.h"
#include "menu.h"
#include "Room.h"
#include "Wall.h"
#include "Door.h"
#include "Switch.h"
#include "Obstacle.h"
#include "Riddle.h"
#include "GameMode.h"

int main(int argc, char** argv)
{
    hideCursor();

    GameMode mode = GameMode::Normal;
    bool silent = false;

    if (argc > 1) {
        std::string arg1 = argv[1];

        if (arg1 == "-save") {
            mode = GameMode::Save;
        }
        else if (arg1 == "-load") {
            mode = GameMode::Load;

            if (argc > 2 && std::string(argv[2]) == "-silent") {
                silent = true;
            }
        }
    }
    
    if (mode == GameMode::Load) {
        // NO MENU in load mode
        startGame(mode, silent);
    }
    else {
        // Normal or Save → menu exists
        while (true) {
            MenuOptions action = runMenu();

            if (action == MENU_START)
                startGame(mode, silent);
            else if (action == MENU_EXIT)
                break;
        }
    }

    return 0;
}
    