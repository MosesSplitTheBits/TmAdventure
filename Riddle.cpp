#include "Riddle.h"
#include "Game.h"
#include "Key.h"
#include <iostream>
#include <string>
#include "Sound.h"


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
            Sound::RiddleCorrect();
            setSolved(true);
            
            // Capture data before removing the Riddle object
            int x = getPosition().getX();
            int y = getPosition().getY();
            int keyId = getId();
            
            // 1. Remove the Riddle object (so the '?' goes away)
            game.removeObjectAt(x, y);

            // 2. Spawn the Key at the same spot
            game.addObject(std::make_unique<Key>(x, y, keyId));

            // 3. Update visuals
            game.getScreen().setCharAt(x, y, 'K');
            game.getScreen().draw();
            player.draw();
        }
    }
    return true;
}