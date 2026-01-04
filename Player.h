#pragma once
#include "Point.h"
#include "Screen.h"
#include <vector>
#include <algorithm>

// Forward declaration (tells compiler "Game" exists)
class Game; 


struct SpringLaunchState 
{
    bool active = false;
    int speed = 0;
    int remainingCycles = 0;
    Direction launchDir;
};

    
struct Inventory {
    char type = ' '; 
    int id = 0;      
};

class Player {
private:
    const char* Keys;
    Screen& screen;
    Point p;
    char underChar; 
    Inventory playerInventory; 
    bool won = false;
    bool waitingAtDoor = false;

    int hp = 3; // Health points
    int maxHp = 3;

public:
    SpringLaunchState springState;
    int compressedSprings = 0;

    Player(const char* keys, Screen& s, Point start)
        : Keys(keys), screen(s), p(start), underChar(' '), won(false), hp(3), maxHp(3) {}


    int getHP() const { return hp; }
    int getMaxHP() const { return maxHp; }
    bool isDead() const { return hp <= 0; }

    void setMaxHP(int v) { maxHp = std::max(1, v); hp = std::min(hp, maxHp); }
    void setHP(int v) { hp = std::clamp(v, 0, maxHp); }

    void takeDamage(int dmg)
    {
        if (dmg <= 0) return;
        hp = std::max(0, hp - dmg);
    }

    void heal(int healAmount)
    {
        if (healAmount <= 0) return;
        hp = std::min(maxHp, hp + healAmount);
    }


    void keyPreesed(char k);
    
    void move(Game& game); 

    void draw();
    void erase();
    void setPosition(int x, int y) { p.set(x, y); }
    Point& getPosition() { return p; }
    const Point& getPosition() const { return p; }; //Const overload
    
    // Inventory
    bool hasItem() const { return playerInventory.type != ' '; }
    bool hasKey(int keyId) const;
    bool hasTorch() const;
    bool hasBomb() const;
    void pickKey(int keyId) { playerInventory = { 'K', keyId }; }
    void pickTorch() { playerInventory = { 'T', -1 }; }
    void pickBomb() { playerInventory = { '0', -1 }; }
    int dropKey();
    bool dropTorch();
    bool dropBomb();
    void useKey(int keyId);
    void tryDropKey(Game& game);
    void tryDropTorch(Game& game);
    void tryDropItem(Game& game);
    void tryDropBomb(Game& game);
    bool hasKeyForDoor(int doorKeyId) const;
    
    
    bool hasWon() const { return won; }
    void setWon(bool w) { won = w; }
    
    bool isWaitingAtDoor() const { return waitingAtDoor; }
    void setWaitingAtDoor(bool waiting) { waitingAtDoor = waiting; }

    // NEW: expose held item char for StatusBar (' ' means none)
    char getHeldItemChar() const { return playerInventory.type; }
};










