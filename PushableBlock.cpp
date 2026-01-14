#include "PushableBlock.h"
#include "Game.h"
#include "Player.h"
#include "Screen.h"
#include <iostream> 

bool PushableBlock::interact(Game& game, Player& player) {
    Direction pushDir;

    if (player.springState.active) {
        pushDir = player.springState.launchDir;

    } else {
        pushDir = player.getPosition().getDir();
    }
    
    int dirIndex = -1;
    for (int i = 0; i < 4; ++i) {
        if (pushDir.dx() == Direction::directions[i].dx() && 
            pushDir.dy() == Direction::directions[i].dy()) {
            dirIndex = i;
            break;
        }
    }
    
    if (dirIndex >= 0) {
        int playerForce = 1;
        
        if (player.springState.active) {
            playerForce = player.springState.speed;
        }
        
        directionForces[dirIndex] += playerForce;
        
    }
    
    return false;
}

void PushableBlock::tryPush(Game& game) {
    const int REQUIRED_FORCE = 2;
    
    for (const auto& [dirIndex, force] : directionForces) {
        if (force >= REQUIRED_FORCE) {
            
            Direction moveDir = Direction::directions[dirIndex];
            int newAnchorX = anchorX + moveDir.dx();
            int newAnchorY = anchorY + moveDir.dy();
            
            
            // Check if ALL tiles in the new position are valid and empty
            bool canMove = true;
            for (int dy = 0; dy < height; ++dy) {
                for (int dx = 0; dx < width; ++dx) {
                    int checkX = newAnchorX + dx;
                    int checkY = newAnchorY + dy;
                    
                    // Check bounds
                    if (checkX <= 0 || checkX >= Screen::MAX_X || 
                        checkY <= 0 || checkY >= Screen::MAX_Y) {
                        canMove = false;
                        break;
                    }
                    
                    // Check for blocking objects (ignore tiles that are part of THIS block)
                    GameObject* targetObj = game.objectAt(checkX, checkY);
                    if (targetObj && targetObj != this && !targetObj->isPassable()) {
                        canMove = false;
                        break;
                    }
                }
                if (!canMove) break;
            }
            if (canMove) {
                // Update anchor position
                anchorX = newAnchorX;
                anchorY = newAnchorY;
                p.set(anchorX, anchorY);
    
        // Rebuild grid
        game.rebuildObjectGrid();
    
        break;
}
            
            
        }
    }
    
    resetForces();
}
// Check if a coordinate is part of this block
bool PushableBlock::occupies(int x, int y) const {
    return (x >= anchorX && x < anchorX + width &&
            y >= anchorY && y < anchorY + height);
}
// Get all tiles occupied by this block
std::vector<std::pair<int, int>> PushableBlock::getOccupiedTiles() const {
        std::vector<std::pair<int, int>> tiles;
        for (int dy = 0; dy < height; ++dy) {
            for (int dx = 0; dx < width; ++dx) {
                tiles.push_back({anchorX + dx, anchorY + dy});
            }
        }
        return tiles;
    }