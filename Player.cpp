#include "Player.h"
#include "Game.h" // Required to access Game::objectAt
#include "Direction.h"
#include <iostream>
#include <cctype>

// 1. SET DIRECTION LOGIC
void Player::keyPreesed(char k)
{
    // Check movement keys (0=Up, 1=Down, 2=Left, 3=Right usually)
    for (int i = 0; i < 4; ++i) {
        if (k == Keys[i] || k == toupper(Keys[i])) {
            p.changeDirection(Direction::directions[i]);
            return;
        }
    }
    
    // Check STAY key (Index 4)
    if (k == Keys[4] || k == toupper(Keys[4])) {
        p.changeDirection(Direction::directions[Direction::STAY]);
    }
}

// 2. MOVE LOGIC (With Collision Check)
void Player::move(Game& game)
{
    // Calculate where we WANT to go
    int nx = p.getX() + p.getDir().dx();
    int ny = p.getY() + p.getDir().dy();

    // A. Check Game Objects (Doors, Walls, Obstacles)
    GameObject* obj = game.objectAt(nx, ny);
    if (obj) {
        // This solves the riddle and removes it from the game
        bool keepMoving = obj->interact(game, *this);
        
        // Re-fetch object (it might be gone now!)
        obj = game.objectAt(nx, ny);
        
        // If interaction failed or object still blocks us
        if (!keepMoving || (obj && !obj->isPassable())) {
            return; 
        }
    }

    erase();      
    p.move();     
    
    // NOW this will capture ' ' because the riddle was already removed/cleared
    underChar = screen.getCharAt(p.getX(), p.getY());
    if (underChar == '?') underChar = ' ';
    
    draw();       
}

void Player::draw()
{
    p.draw();
}

void Player::erase()
{
    p.draw(underChar);
}

// --- INVENTORY LOGIC ---

bool Player::hasKey(int keyId) const {
    return playerInventory.type == 'K' && playerInventory.id == keyId;
}

int Player::dropKey() {
    if (playerInventory.type == 'K') {
        int id = playerInventory.id;
        playerInventory.type = ' ';
        playerInventory.id = 0;
        return id;
    }
    return -1;
}

void Player::useKey(int keyId) {
    if (hasKey(keyId)) {
        playerInventory.type = ' ';
        playerInventory.id = 0;
    }
}

bool Player::hasKeyForDoor(int doorKeyId) const {
    return hasKey(doorKeyId);
}