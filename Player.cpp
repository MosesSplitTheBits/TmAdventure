#include "Player.h"
#include "Game.h"
#include "Key.h"
#include "Direction.h"
#include "Torch.h"
#include "Bomb.h"
#include <iostream>
#include <cctype>
#include "utils.h"
#include "Sound.h"

// 1. SET DIRECTION LOGIC
void Player::keyPreesed(char k)
{
    // During spring: allow ONLY sideways movement
    if (springState.active) {
        Direction newDir;

        for (int i = 0; i < 4; ++i) {
            if (k == Keys[i] || k == toupper(Keys[i])) {
                newDir = Direction::directions[i];

                 // Allow only perpendicular movement
                if (newDir.dx() * springState.launchDir.dx() +
                    newDir.dy() * springState.launchDir.dy() == 0) {
                    p.changeDirection(newDir);
                }
            return;
        }
    }

    return;
}
    Direction oldDir = p.getDir();
    
    // Check movement keys (0=Up, 1=Down, 2=Left, 3=Right usually)
    for (int i = 0; i < 4; ++i) {
        if (k == Keys[i] || k == toupper(Keys[i])) {
            Direction newDir = Direction::directions[i];
            bool directionChanged =
            newDir.dx() != oldDir.dx() ||
            newDir.dy() != oldDir.dy();
            
            if (directionChanged && compressedSprings > 0) {
                springState.active = true;
                springState.speed = compressedSprings;
                springState.remainingCycles = compressedSprings * compressedSprings;
                compressedSprings = 0;
            }
            
            p.changeDirection(newDir);
            return;
        }
    }
    
    // Check STAY key (Index 4)
    if (k == Keys[4] || k == toupper(Keys[4])) {
    if (compressedSprings > 0) {
        springState.active = true;
        springState.speed = compressedSprings;
        springState.remainingCycles = compressedSprings * compressedSprings;
        compressedSprings = 0;
    }
    p.changeDirection(Direction::directions[Direction::STAY]);
}
}
    


// 2. MOVE LOGIC (With Collision Check)
void Player::move(Game& game)
{
    if (waitingAtDoor)
        return;

    int movesThisFrame = springState.active ? springState.speed : 1;

    Direction moveDir = p.getDir();
    if (springState.active) {
        moveDir = springState.launchDir;
    }

    for (int moveCount = 0; moveCount < movesThisFrame; ++moveCount) {

        int nx = p.getX() + moveDir.dx();
        int ny = p.getY() + moveDir.dy();

        GameObject* obj = game.objectAt(nx, ny);
        if (obj) {
            bool keepMoving = obj->interact(game, *this);
            obj = game.objectAt(nx, ny);

            if (!keepMoving || (obj && !obj->isPassable())) {

                if (springState.active) {
                    PushableBlock* block = dynamic_cast<PushableBlock*>(obj);

                    if (!block) {
                        Sound::ThumpWall();
                        springState.active = false;
                        springState.speed = 0;
                        springState.remainingCycles = 0;
                        p.changeDirection(Direction::directions[Direction::STAY]);
                        return;
                    }
                }

                break;
            }
        }

        // ACTUAL MOVE (state only)
        p.move(moveDir);
    }

    // SPRING TIMER
    if (springState.active) {
        springState.remainingCycles--;

        if (springState.remainingCycles <= 0) {
            springState.active = false;
            springState.speed = 0;

            // Reset spring state ONLY (no drawing)
            for (auto spring : game.getSprings()) {
                spring->setCompressed(false);
            }

            p.changeDirection(Direction::directions[Direction::STAY]);
        }
    }
}

void Player::draw()
{
    // DEPRECATED: Use Screen buffer instead
    // All rendering now happens through Game::renderFrame()
}

void Player::erase()
{
    // DEPRECATED: Use Screen buffer instead
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

bool Player::hasTorch() const { return playerInventory.type == 'T'; }

bool Player::dropTorch(){
    if (playerInventory.type == 'T') {
        playerInventory.type = ' ';
        playerInventory.id = 0;
        return true;
    }
    return false;
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

bool Player::hasBomb() const { return playerInventory.type == '0'; }

bool Player::dropBomb() {
    if (playerInventory.type == '0') {
        playerInventory.type = ' ';
        playerInventory.id = 0;
        return true;
    }
    return false;
}

void Player::tryDropBomb(Game& game) {
    if (!dropBomb()) return;

    Sound::BombPlant();
    int dropX = p.getX();
    int dropY = p.getY();
    Direction dir = p.getDir();
    int bx = dropX - dir.dx();
    int by = dropY - dir.dy();

    auto bomb = std::make_unique<Bomb>(bx, by);
    bomb->plant(5); // Start the 5-tick countdown
    game.addObject(std::move(bomb));
}

void Player::tryDropKey(Game& game) {
    int droppedKeyId = this->dropKey(); 
    if (droppedKeyId == -1) return;

    int dropX = p.getX();
    int dropY = p.getY();
    Direction dir = p.getDir();
    int keyX = dropX - dir.dx();
    int keyY = dropY - dir.dy();

    // הוספת המפתח למשחק
    game.addObject(std::make_unique<Key>(keyX, keyY, droppedKeyId));
}

void Player::tryDropTorch(Game& game) {
    if (!dropTorch()) return;

    int dropX = p.getX();
    int dropY = p.getY();
    Direction dir = p.getDir();
    int tx = dropX - dir.dx();
    int ty = dropY - dir.dy();

    game.addObject(std::make_unique<Torch>(tx, ty));
}


void Player::tryDropItem(Game& game) {
    if (hasBomb()) {
        tryDropBomb(game);
    } else if (hasTorch()) {
        tryDropTorch(game);
    } else {
        tryDropKey(game);
    }
}