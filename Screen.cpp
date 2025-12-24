#include "Screen.h"
#include <iostream>

const int Screen::MAX_X;
const int Screen::MAX_Y;

bool Screen::isWall(int x, int y) const
{
    return getCharAt(x, y) == 'W';
}

bool Screen::isKey(int x, int y) const
{
    return getCharAt(x, y) == 'K';
}

bool Screen::isTorch(int x, int y) const
{
    return getCharAt(x, y) == '!';
}

bool Screen::isBomb(int x, int y) const
{
    return getCharAt(x, y) == '@';
}

bool Screen::isObstacle(int x, int y) const
{
    return getCharAt(x, y) == '*';
}

bool Screen::isSpring(int x, int y) const 
{
    return getCharAt(x, y) == '^';
}

bool Screen::isSwitchOn(int x, int y) const 
{
    return getCharAt(x, y) == 'S';
}

bool Screen::isSwitchOff(int x, int y) const
{
    return getCharAt(x, y) == 's';
}

bool Screen::isRiddle(int x, int y) const
{
    return getCharAt(x, y) == '?';
}

bool Screen::isDoor(int x, int y) const
{
    return getCharAt(x, y) == '4';
}

