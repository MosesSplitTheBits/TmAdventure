#include "Riddle.h"
#include "Game.h"
#include <iostream>
#include <string>

std::vector<std::unique_ptr<Riddle>> Riddle::createFromMap(const std::vector<std::string>& mapData, int roomId) 
{
    std::vector<std::unique_ptr<Riddle>> riddles;
    for (int y = 0; y < (int)mapData.size(); ++y) {
        for (int x = 0; x < (int)mapData[y].size(); ++x) {
            if (mapData[y][x] == '?') {
                riddles.push_back(std::make_unique<Riddle>(x, y, roomId));
            }
        }
    }
    return riddles;
}


bool Riddle::interact(Game& game, Player& player) 
{
    if (!isSolved()) {
        // Access puzzles via the getter in Game
        bool solved = game.getPuzzles().showPuzzle(getId() - 1, getId());

        if (game.getPuzzles().isGameOver()) {
            return false; // Game Over
        }

        if (solved) {
            setSolved(true);
            
            // Let the player pick up the key (id same as riddle id)
            player.pickKey(getId());
            
            // Visuals
            game.getScreen().draw();
            player.draw();
        }
    }
    return true;
}