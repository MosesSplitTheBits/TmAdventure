#include "Riddle.h"
#include "Game.h"
#include "Key.h"
#include <iostream>
#include <string>
#include "Sound.h"
#include "ResultsRecorder.h"


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
    if (isSolved()) {
        return true;
    }

    const Puzzle& puzzle = game.getPuzzles().getPuzzleForRoom(getId());

    const bool solved = game.getPuzzles().showPuzzle(
        puzzle,
        getId(),
        [&]() { return player.getHP(); },
        player.getMaxHP(),
        [&]() { game.damagePlayer(player, 1); } // -1 HP לכל טעות
    );
    
    if (game.getMode() == GameMode::Save && game.getResultsRecorder()) {
        game.getResultsRecorder()->recordRiddle(
            game.getGameTime(),
            puzzle.question,
            "",  // empty string for answer field
            solved
        );
    }

    if (solved) {
        Sound::RiddleCorrect();
        setSolved(true);

        int x = getPosition().getX();
        int y = getPosition().getY();
        int keyId = getId();

        game.removeObjectAt(x, y);
        game.addObject(std::make_unique<Key>(x, y, keyId));

        return true;
    }

    // אם נכשל בגלל שנגמר HP (showPuzzle החזיר false)
    if (player.isDead()) {
        return false;
    }

    return true;
}