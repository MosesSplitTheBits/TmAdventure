#include "Point.h"
#include <iostream>
#include "utils.h"
#include "Screen.h"


void Point::draw(char c)
{
    // DEPRECATED: No console access allowed
    // All rendering now happens through Screen buffer
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

//Overloaded move to accept direction
void Point::move(const Direction& d) {
    x += d.dx();
    y += d.dy();
}

const Direction& Point::getDir() const {
    return dir;
}