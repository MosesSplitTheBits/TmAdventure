#include "Screen.h"
#include "VisionSystem.h"
#include <iostream>

const int Screen::MAX_X;
const int Screen::MAX_Y;


void Screen::clear()
{
    for (int i = 0; i <= MAX_Y; ++i) {
        for (int j = 0; j <= MAX_X; ++j) {
            board[i][j] = ' ';
            colors[i][j] = 7; // לבן
        }
    }
}

void Screen::loadMap(const std::vector<std::string>& mapData) 
{
    // ניקוי המסך
    for (int i = 0; i <= MAX_Y; ++i) {
        for (int j = 0; j <= MAX_X; ++j) {
            board[i][j] = ' ';
            colors[i][j] = 7; // לבן
        }
    }

    // טעינת המפה עם הזזה (Offset)
    for (int i = 0; i < (int)mapData.size(); ++i) {
        // בדיקה שלא חורגים מהגבולות
        if (i > MAX_Y) break;

        for (int j = 0; j < (int)mapData[i].size(); ++j) {
            if (j > MAX_X) break;
            
            // כאן אנחנו מוסיפים את ה-OFFSET לשורה
            board[i][j] = mapData[i][j];
        }
    }
}


void Screen::draw() const 
{
    for (int i = 0; i <= MAX_Y; ++i) {
        gotoxy(0, i); 
        
        for (int j = 0; j <= MAX_X; ++j) {
            setTextColor(colors[i][j]);
            std::cout << board[i][j];
        }
    }
    setTextColor(7); // Reset to white
}



int Screen::getColorAt(int x, int y) const 
{
    if (x >= 0 && x <= MAX_X && y >= 0 && y <= MAX_Y) 
    {
        return colors[y][x];
    }
    return 7; // Default white if out of bounds
}


//Vision handling
void Screen::applyVision(
    const std::vector<std::string>& mapData,
    const VisionSystem::Grid& vision
) {
    int height = mapData.size();

    for (int y = 0; y < height && y <= MAX_Y; ++y) {
        int width = mapData[y].size();

        for (int x = 0; x < width && x <= MAX_X; ++x) {
            if (vision[y][x] == VisionSystem::Cell::Hidden) {
                board[y][x] = 'd';
                colors[y][x] = 8; // dark gray
            } else {
                board[y][x] = mapData[y][x];
                colors[y][x] = 7;
            }
        }
    }
}

