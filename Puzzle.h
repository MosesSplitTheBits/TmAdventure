#pragma once
#include <string>
#include <iostream>
#include <conio.h>
#include <limits>

struct Puzzle {
    std::string question;
    std::string answers[4];
    int correctIndex;
};

class PuzzleManager {
public:
    PuzzleManager();
    // Updated signature to take riddleId for display
    bool showPuzzle(int mapIndex, int riddleId);
    int getPuzzleCount() const ;

    // global attempt counter
    int getTotalAttempts() const { return totalAttempts; }
    int getTotalWrongAttempts() const { return totalWrongAttempts; }
    void resetCounters() { totalAttempts = 0; totalWrongAttempts = 0; }

    // setter & getters - for limits
    void setMaxTotalAttempts(int max) { maxTotalAttempts = max; }
    void setMaxWrongAttempts(int max) { maxWrongAttempts = max; }
    int getMaxTotalAttempts() const { return maxTotalAttempts; }
    int getMaxWrongAttempts() const { return maxWrongAttempts; }

    bool isGameOver() const {
        return (maxWrongAttempts > 0 && totalWrongAttempts >= maxWrongAttempts);
    }

private:
    Puzzle puzzles[3];
    int totalAttempts = 0;
    int totalWrongAttempts = 0;
    int maxTotalAttempts = 0; // Set to 0 to disable total limit, focusing only on wrong attempts
    int maxWrongAttempts = 3; 
};