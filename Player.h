#pragma once
#include "Point.h"
#include "Screen.h"

#include <vector>

// Forward declaration (tells compiler "Game" exists)
class Game; 

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

public:
    Player(const char* keys, Screen& s, Point start) 
        : Keys(keys), screen(s), p(start), underChar(' '), won(false) {}

    void keyPreesed(char k);
    
    void move(Game& game); 

    void draw();
    void erase();
    void setPosition(int x, int y) { p.set(x, y); }
    Point& getPosition() { return p; }
    
    // Inventory
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
};










