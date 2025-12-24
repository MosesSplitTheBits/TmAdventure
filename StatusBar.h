#pragma once
#include <ctime>
#include <string>

class Screen; // Forward declaration

class StatusBar {
private:
    time_t appStartTime;      // הזמן שבו ה-EXE נפתח
    time_t runStartTime;      // הזמן שבו המשחק הנוכחי התחיל
    int mistakes;             // מונה טעויות
    
public:
    StatusBar();
    
    void resetRunTime();      // איפוס זמן לריצה נוכחית (כשמתחילים שלב/משחק מחדש)
    void addMistake();        // הוספת טעות
    void resetMistakes();     // איפוס טעויות
    
    // פונקציה שמציירת את הנתונים
    void draw(int levelId, int puzzleMistakes);
    
    // עזרי זמן
    std::string formatTime(time_t seconds) const;
};