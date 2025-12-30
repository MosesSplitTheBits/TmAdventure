#include "Screen.h"
#include <iostream>

const int Screen::MAX_X;
const int Screen::MAX_Y;


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
        // הזזת הסמן לתחילת השורה בלבד (הרבה יותר מהיר)
        gotoxy(0, i); 
        
        for (int j = 0; j <= MAX_X; ++j) {
            gotoxy(j, i);
            setTextColor(colors[i][j]);
            std::cout << board[i][j];
        }
    }
    setTextColor(7); // החזרת צבע לבן
}

int Screen::getColorAt(int x, int y) const 
{
    if (x >= 0 && x <= MAX_X && y >= 0 && y <= MAX_Y) 
    {
        return colors[y][x];
    }
    return 7; // Default white if out of bounds
}


