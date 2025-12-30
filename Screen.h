#pragma once
#include <vector>
#include <string>
#include "utils.h" // בשביל gotoxy ו-setTextColor
#include <iostream>

class Screen {
public:
    static const int MAX_X = 79;
    static const int MAX_Y = 24;
    static const int MAP_OFFSET_Y = 0; // הוספנו את זה קודם

private:
    // --- כאן ההגדרה החסרה! ---
    char board[MAX_Y + 1][MAX_X + 1];
    int colors[MAX_Y + 1][MAX_X + 1];

public:
    

    Screen() {} // בנאי ריק

    // פונקציות קיימות
    void loadMap(const std::vector<std::string>& mapData);
    void draw() const; // פונקציה שמציירת את כל המסך
    
    // גטרים וסטררים
    char getCharAt(int x, int y) const {
        if (x < 0 || x > MAX_X || y < 0 || y > MAX_Y) return ' ';
        return board[y][x];
    }

    int getColorAt(int x, int y) const;

    void setCharAt(int x, int y, char c) {
        if (x >= 0 && x <= MAX_X && y >= 0 && y <= MAX_Y) {
            board[y][x] = c;
        }
    }

    void setColorAt(int x, int y, int color) {
        if (x >= 0 && x <= MAX_X && y >= 0 && y <= MAX_Y) {
            colors[y][x] = color;
        }
    }

    // פונקציה לציור תא בודד (יעיל יותר מציור כל המסך)
    void drawCell(int x, int y) const {
        if (x < 0 || x > MAX_X || y < 0 || y > MAX_Y) return;
        gotoxy(x, y);
        setTextColor(colors[y][x]);
        std::cout << board[y][x];
        setTextColor(7); // החזרת צבע לבן
    }

    

    

};

