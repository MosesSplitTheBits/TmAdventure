#include "Point.h"
#include <iostream>
#include "utils.h"
#include "Screen.h"


void Point::draw(char c)
{
    gotoxy(x, y);
    std::cout << c;
}

void Point::move()
{
    if (x + dir.dx() <= 0 || x + dir.dx() > Screen::MAX_X - 1 ||
        y + dir.dy() <= 0 || y + dir.dy() > Screen::MAX_Y - 1)
    {
        dir = Direction::directions[Direction::STAY];
        return;
    }
    x += dir.dx();
    y += dir.dy();
}

const Direction& Point::getDir() const {
    return dir;
}