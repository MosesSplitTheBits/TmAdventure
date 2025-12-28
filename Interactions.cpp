#include "Game.h"
#include "Key.h"
#include "Riddle.h"
#include "Door.h"
#include "Player.h"
#include "Bomb.h"
#include <iostream>
#include "Screen.h"
#include "utils.h" // Added for system("cls") if needed

// --- KEY INTERACTION ---
bool Key::interact(Game& game, Player& player) {
    if (!isCollected()) {
        collect();
        player.pickKey(getId());
        game.removeObjectAt(getPosition().getX(), getPosition().getY());
        game.getScreen().setCharAt(getPosition().getX(), getPosition().getY(), ' ');
        player.draw();
    }
    return true;
}

// --- RIDDLE INTERACTION ---
bool Riddle::interact(Game& game, Player& player) {
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

// --- DOOR INTERACTION ---
bool Door::interact(Game& game, Player& player) {
    // Check if the door is open (isPassable returns true if open)
    if (isPassable()) {
        Room* target = getTarget();
        char doorType = getChar();

        // 1. Transition Doors (3 or 4) - Need both players
        if (doorType == '3' || doorType == '4') {
            if (target) {
                // Get both players
                Player& p1 = game.getp1();
                Player& p2 = game.getp2();
                
                // If this player isn't waiting yet, set them to waiting
                if (!player.isWaitingAtDoor()) {
                    player.setWaitingAtDoor(true);     // Mark as waiting
                    player.getPosition().erase();      // Make them disappear
                }
                
                // NOW check if BOTH players are waiting
                if (p1.isWaitingAtDoor() && p2.isWaitingAtDoor()) {
                    // Reset waiting flags before transition
                    p1.setWaitingAtDoor(false);
                    p2.setWaitingAtDoor(false);
                    game.setLastUsedDoor(this); //save last used door
                    // Load the next level
                    game.loadLevel(target, doorType == '3');
                    system("cls");
                    game.getScreen().draw();
                    return true;
                }
                
                return true; // Still waiting for other player
            }
        }
        
        // 2. Victory Door: Type '4', Open, and No Target (Room 3)
        if (doorType == '4' && target == nullptr) {
            player.setWon(true); // Keep this for actual game completion
            return true;
        }
    }
    return true;
}

bool Bomb::interact(Game& game, Player& player) {
    if (!isCollected()) {
        collect();
        player.pickBomb();
        game.removeObjectAt(getPosition().getX(), getPosition().getY());
        game.getScreen().setCharAt(getPosition().getX(), getPosition().getY(), ' ');
        player.draw();
    }
    return true;
}

