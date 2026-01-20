#pragma once
#include <string>
#include <iostream>
#include <conio.h>
#include <limits>
#include <functional>
#include <vector>

struct Puzzle {
    int roomId;
    std::string question;
    std::string answers[4];
    int correctIndex;
};

class PuzzleManager {
public:
    PuzzleManager();

    // CHANGED: showPuzzle יודע להציג HP ולקנוס על טעות באמצעות callback
    bool showPuzzle(
        const Puzzle& puzzle,
        int riddleId,
        const std::function<int()>& getHp,
        int maxHp,
        const std::function<void()>& onWrongAttempt
    );

    int getPuzzleCount() const ;

    // global attempt counter
    int getTotalAttempts() const { return totalAttempts; }
    int getTotalWrongAttempts() const { return totalWrongAttempts; }
    void resetCounters() { totalAttempts = 0; totalWrongAttempts = 0; }
    
    const Puzzle& getPuzzleData(int mapIndex) const { return puzzles[mapIndex]; }
    const Puzzle& getPuzzleForRoom(int roomId) const;

    // setter & getters - for limits
    void setMaxTotalAttempts(int max) { maxTotalAttempts = max; }
    void setMaxWrongAttempts(int max) { maxWrongAttempts = max; }
    int getMaxTotalAttempts() const { return maxTotalAttempts; }
    int getMaxWrongAttempts() const { return maxWrongAttempts; }

    bool isGameOver() const {
        return (maxWrongAttempts > 0 && totalWrongAttempts >= maxWrongAttempts);
    }

private:
    std::vector<Puzzle> puzzles;
    int totalAttempts;
    int totalWrongAttempts;
    int maxTotalAttempts; // Disabled
    int maxWrongAttempts; // legacy (לא חובה להסיר, אבל לא ישמש ל-GameOver)
};