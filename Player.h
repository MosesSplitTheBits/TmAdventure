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
    void pickKey(int keyId) { playerInventory = { 'K', keyId }; }
    int dropKey();
    void useKey(int keyId);
    bool hasKeyForDoor(int doorKeyId) const;
    
    
    bool hasWon() const { return won; }
    void setWon(bool w) { won = w; }
};










