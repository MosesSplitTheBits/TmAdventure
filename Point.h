#pragma once
#include <cstdlib>
#include "Direction.h"
#include <cmath>
#include <utility>
#include "Screen.h"


class Point 
{
    int x = 0;
    int y = 0;
    Direction dir = Direction::directions[Direction::STAY];
    char ch = ' ' ;
public:
    Point() {}
    Point(int x1, int y1, const Direction& the_dir, char c)
    {
        x = x1;
        y = y1;
        dir = the_dir;
        ch = c;
    }
    void draw()
    {
        draw(ch);
    }
    void draw(char c);
    void move();
    void move(const Direction& d);

    void erase() 
    {
        draw(' '); // Draw a space to erase the character
    }

    void set(int new_x, int new_y)
    {
        x = new_x;
        y = new_y;
    }




    void setSymbol(char c)
    {
        ch = c;
    }
    const Direction& getDir() const;
    void changeDirection(const Direction& new_dir)
    {
        dir = new_dir;
    }
    int getX() const
    {
        return x;
    }
    int getY() const
    {
        return y;
    }
    char getChar() const
    {
        return ch;
    }


    bool operator==(const Point& other) const {
        return (x == other.x && y == other.y);
    }
    
    // Optional: Overload != as well
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};