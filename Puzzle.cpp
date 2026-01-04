#include "Puzzle.h"
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
    // map 0 – puzzle 1
    puzzles[0].question = "Solve for X: 2X + 3 = 11";
    puzzles[0].answers[0] = "1";
    puzzles[0].answers[1] = "2";
    puzzles[0].answers[2] = "3";
    puzzles[0].answers[3] = "4";
    puzzles[0].correctIndex = 3;

    // map 1 – puzzle 2
    puzzles[1].question = "What is the capital of France?";
    puzzles[1].answers[0] = "Berlin";
    puzzles[1].answers[1] = "Paris";
    puzzles[1].answers[2] = "Rome";
    puzzles[1].answers[3] = "Madrid";
    puzzles[1].correctIndex = 1;

    // map 2 – puzzle 3
    puzzles[2].question = "Which animal barks?";
    puzzles[2].answers[0] = "Cat";
    puzzles[2].answers[1] = "Cow";
    puzzles[2].answers[2] = "Dog";
    puzzles[2].answers[3] = "Bird";
    puzzles[2].correctIndex = 2;
}

bool PuzzleManager::showPuzzle(
    int mapIndex,
    int riddleId,
    const std::function<int()>& getHp,
    int maxHp,
    const std::function<void()>& onWrongAttempt
) {
    Puzzle& p = puzzles[mapIndex];

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
    return 3;
}