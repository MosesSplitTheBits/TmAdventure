#pragma once
#include <ctime>
#include <string>

class Screen; // Forward declaration

class StatusBar {
private:
    time_t appStartTime;      // הזמן שבו ה-EXE נפתח
    time_t runStartTime;      // הזמן שבו המשחק הנוכחי התחיל
    
public:
    StatusBar();
    
    void resetRunTime();      // איפוס זמן לריצה נוכחית (כשמתחילים שלב/משחק מחדש)
    
    // פונקציה שמציירת את הנתונים
    void draw(int levelId, int p1Hp, int p1MaxHp, int p2Hp, int p2MaxHp, char p1ItemChar, char p2ItemChar);
    
    // עזרי זמן
    std::string formatTime(time_t seconds) const;
};