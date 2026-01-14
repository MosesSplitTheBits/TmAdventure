#pragma once
#include <vector>
#include <string>
#include "utils.h" // בשביל gotoxy ו-setTextColor
#include "VisionSystem.h"
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
    void clear(); // Clear buffer to spaces and white
    void loadMap(const std::vector<std::string>& mapData);
    void draw() const; // פונקציה שמציירת את כל המסך

    void applyVision(const std::vector<std::string>& mapData, const VisionSystem::Grid& vision);
    
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

    // Write text string to buffer at position (for StatusBar)
    void writeText(int x, int y, const std::string& text, int color = 7) {
        for (size_t i = 0; i < text.length() && (int)(x + i) <= MAX_X; ++i) {
            if (y >= 0 && y <= MAX_Y) {
                board[y][x + i] = text[i];
                colors[y][x + i] = color;
            }
        }
    }

};

