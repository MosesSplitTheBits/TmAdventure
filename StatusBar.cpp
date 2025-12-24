#include "StatusBar.h"
#include "Screen.h"
#include "utils.h" // בשביל gotoxy
#include <iostream>
#include <iomanip>
#include <sstream>

StatusBar::StatusBar() : mistakes(0) {
    appStartTime = std::time(nullptr);
    runStartTime = std::time(nullptr);
}

void StatusBar::resetRunTime() {
    runStartTime = std::time(nullptr);
}

void StatusBar::addMistake() {
    mistakes++;
}

void StatusBar::resetMistakes() {
    mistakes = 0;
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

void StatusBar::draw(int levelId, int puzzleMistakes) {
    time_t currentTime = std::time(nullptr);
    time_t totalTime = currentTime - appStartTime;
    time_t currentRunTime = currentTime - runStartTime;

    // צבע רקע לסטטוס בר (למשל לבן על כחול או סתם לבן)
    // נניח שאנחנו מציירים בשורות 0-3

    int startY = 21;
    setTextColor(15); // לבן על כחול
    
    gotoxy(0, startY);
    std::cout << "===============================================================================";
    
    gotoxy(0, startY + 1);
    std::cout << " LEVEL: " << levelId << " \t MISTAKES: " << puzzleMistakes;
    
    gotoxy(0, startY + 2);
    std::cout << " TOTAL TIME: " << formatTime(totalTime) << " \t RUN TIME: " << formatTime(currentRunTime);
    
    gotoxy(0, startY + 3);
    std::cout << "===============================================================================";
}