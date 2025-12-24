#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "utils.h"
#include <windows.h> // for SetConsoleTextAttribute

class Screen {
public:
    static const int MAX_X = 79;
    static const int MAX_Y = 24;

private:
    std::vector<std::string> currentMap; 
    std::vector<std::vector<int>> colorMap; // parallel to currentMap, stores color attributes

public:
    // Load a map (copying the vector) and initialize color map to default (7)
    void loadMap(const std::vector<std::string>& newMap) {
        currentMap = newMap;

        // Normalize each row to fixed width and sanitize non-printable chars
        for (auto &row : currentMap) {
            if ((int)row.size() < MAX_X + 1) row.resize(MAX_X + 1, ' ');
            else if ((int)row.size() > MAX_X + 1) row.resize(MAX_X + 1);
            for (char &c : row) if (static_cast<unsigned char>(c) < 32) c = ' ';
        }

        // Normalize number of rows
        if ((int)currentMap.size() < MAX_Y + 1) currentMap.resize(MAX_Y + 1, std::string(MAX_X + 1, ' '));
        else if ((int)currentMap.size() > MAX_Y + 1) currentMap.resize(MAX_Y + 1);

        colorMap.assign(currentMap.size(), std::vector<int>(MAX_X + 1, 7)); // default white
    }

    // The function used by Screen.cpp
    char getCharAt(int x, int y) const {
        if (y >= 0 && y < static_cast<int>(currentMap.size())
     && x >= 0 && x < static_cast<int>(currentMap[y].size()))
            return currentMap[y][x];
        return ' ';
    }

    void setCharAt(int x,int y,char c) {
        if (y >= 0 && y < static_cast<int>(currentMap.size())
     && x >= 0 && x < static_cast<int>(currentMap[y].size()))
            currentMap[y][x] = c;
    }

    // New: color accessors
    int getColorAt(int x,int y) const {
        if (y >= 0 && y < static_cast<int>(colorMap.size())
         && x >= 0 && x < static_cast<int>(colorMap[y].size()))
            return colorMap[y][x];
        return 7;
    }
    void setColorAt(int x,int y,int color) {
        if (y >= 0 && y < static_cast<int>(colorMap.size())
         && x >= 0 && x < static_cast<int>(colorMap[y].size()))
            colorMap[y][x] = color;
    }

    // Draw single cell using its color (useful for incremental updates)
    void drawCell(int x, int y) {
        if (y < 0 || y >= static_cast<int>(currentMap.size()) ||
            x < 0 || x >= static_cast<int>(currentMap[y].size())) return;

        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(h, (WORD)colorMap[y][x]);
        gotoxy(x, y);
        std::cout << currentMap[y][x];
        SetConsoleTextAttribute(h, 7);
    }

    void draw() {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        for (int y = 0; y < (int)currentMap.size(); ++y) {
            for (int x = 0; x < (int)currentMap[y].size(); ++x) {
                SetConsoleTextAttribute(h, (WORD)colorMap[y][x]);
                gotoxy(x, y);
                std::cout << currentMap[y][x];
            }
        }
        SetConsoleTextAttribute(h, 7); // reset
    }

    // Declarations only (Defined in Screen.cpp)
    bool isWall(int x, int y) const;
    bool isKey(int x, int y) const;
    bool isTorch(int x, int y) const;
    bool isBomb(int x, int y) const;
    bool isObstacle(int x, int y) const;
    bool isSpring(int x, int y) const; 
    bool isSwitchOn(int x, int y) const;
    bool isSwitchOff(int x, int y) const;
    bool isRiddle(int x, int y) const;
    bool isDoor(int x, int y) const;
};