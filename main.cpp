#include "Game.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include "utils.h"
#include "Screen.h"
#include "Point.h"
#include "Direction.h"
#include "Player.h"
#include "Game.h"
#include "Puzzle.h"
#include "Key.h"
#include "menu.h"
#include "LevelData.h"
#include "Room.h"
#include "Wall.h"
#include "Door.h"
#include "Switch.h"
#include "Obstacle.h"
#include "Riddle.h"

int main()
{
    hideCursor();

    while (true) {
        MenuOptions action = runMenu();
        if (action == MENU_START) startGame();
        else if (action == MENU_EXIT) break;
    }
    return 0;
}