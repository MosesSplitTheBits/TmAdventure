#include "Puzzle.h"
#include "RiddleLoader.h"
#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <limits>

PuzzleManager::PuzzleManager()
    : totalAttempts(0),
      totalWrongAttempts(0),
      maxTotalAttempts(0), // Disabled
      maxWrongAttempts(3)
{
    // Load riddles from file
    puzzles = RiddleLoader::load("riddles.txt");
}

bool PuzzleManager::showPuzzle(
    const Puzzle& puzzle,
    int riddleId,
    const std::function<int()>& getHp,
    int maxHp,
    const std::function<void()>& onWrongAttempt
) {
    const Puzzle& p = puzzle;

    while (true) {
        system("cls");

        // --- INFO HEADER ---
        std::cout << "\n\t========================================\n";
        std::cout << "\t             RIDDLE CHALLENGE           \n";
        std::cout << "\t========================================\n";
        std::cout << "\t Riddle Number: " << riddleId << "\n";
        std::cout << "\t HP: " << getHp() << " / " << maxHp << "\n";
        std::cout << "\t========================================\n\n";

        std::cout << p.question << "\n\n";
        for (int i = 0; i < 4; i++)
            std::cout << i + 1 << ") " << p.answers[i] << "\n";

        std::cout << "\nEnter choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

        totalAttempts++;

        if (choice - 1 == p.correctIndex) {
            std::cout << "\n\tCorrect! Press any key...";
            _getch();
            return true; // Success
        }
        else {
            totalWrongAttempts++;

            // NEW: כל טעות מורידה 1 HP לשחקן שניסה
            onWrongAttempt();

            // NEW: Game Over רק לפי HP
            if (getHp() <= 0) {
                system("cls");
                std::cout << "\n\n";
                std::cout << "\t  #####################################\n";
                std::cout << "\t  #                                   #\n";
                std::cout << "\t  #           GAME OVER               #\n";
                std::cout << "\t  #                                   #\n";
                std::cout << "\t  #            OUT OF HP              #\n";
                std::cout << "\t  #                                   #\n";
                std::cout << "\t  #####################################\n\n";
                std::cout << "\t     Press any key to return...";
                _getch();
                return false;
            }

            // ממשיכים לנסות, הכותרת תתעדכן בלולאה הבאה עם HP חדש
        }
    }
}

int PuzzleManager::getPuzzleCount() const {
    return static_cast<int>(puzzles.size());
}

const Puzzle& PuzzleManager::getPuzzleForRoom(int roomId) const {
    for (const auto& puzzle : puzzles) {
        if (puzzle.roomId == roomId) {
            return puzzle;
        }
    }
    throw std::runtime_error("No puzzle found for room " + std::to_string(roomId));
}