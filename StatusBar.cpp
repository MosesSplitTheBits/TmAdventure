#include "StatusBar.h"
#include "Screen.h"
#include <iostream>
#include <iomanip>
#include <sstream>

StatusBar::StatusBar() 
{
    appStartTime = std::time(nullptr);
    runStartTime = std::time(nullptr);
}

void StatusBar::resetRunTime() {
    runStartTime = std::time(nullptr);
}



std::string StatusBar::formatTime(time_t seconds) const {
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;
    
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << h << ":"
       << std::setfill('0') << std::setw(2) << m << ":"
       << std::setfill('0') << std::setw(2) << s;
    return ss.str();
}

void StatusBar::draw(Screen& screen, int levelId, int p1Hp, int p1MaxHp, int p2Hp, int p2MaxHp, char p1ItemChar, char p2ItemChar)
{
    time_t currentTime = std::time(nullptr);
    time_t totalTime = currentTime - appStartTime;
    time_t currentRunTime = currentTime - runStartTime;

    int startY = 21;
    
    // Line 1: Separator
    screen.writeText(0, startY, "===============================================================================", 15);

    // Line 2: Level and player stats
    const char p1Item = (p1ItemChar == ' ') ? '-' : p1ItemChar;
    const char p2Item = (p2ItemChar == ' ') ? '-' : p2ItemChar;
    
    std::stringstream ss1;
    ss1 << " LEVEL: " << levelId
        << " \t P1 HP: " << p1Hp << "/" << p1MaxHp << " ITEM: " << p1Item
        << " \t P2 HP: " << p2Hp << "/" << p2MaxHp << " ITEM: " << p2Item;
    screen.writeText(0, startY + 1, ss1.str(), 15);

    // Line 3: Timers
    std::stringstream ss2;
    ss2 << " TOTAL TIME: " << formatTime(totalTime)
        << " \t RUN TIME: " << formatTime(currentRunTime);
    screen.writeText(0, startY + 2, ss2.str(), 15);
}