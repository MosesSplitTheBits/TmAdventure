#pragma once
#include "Point.h"
#include "Direction.h"

class Game;
class Player;


class GameObject {
protected:
    Point p;
    int id;
public:
    // store the character in Point via constructor
    GameObject(int x, int y, char c, int id_in = -1) 
        : p(x, y, Direction::directions[Direction::STAY], c), id(id_in) {}
    
    virtual ~GameObject() = default;

    virtual void draw() { p.draw(); }

    // NEW: Polymorphic interaction
    // Returns false if the game should stop (e.g. Game Over from a riddle)
    virtual bool interact(Game& game, Player& player) { 
        (void)game; (void)player; 
        return true; 
    }

    // Position / id
    Point& getPosition() { return p; }
    const Point& getPosition() const { return p; }
    int getId() const { return id; }
    void setId(int v) { id = v; }
    
    // set the object character
    void setChar(char c) { p.setSymbol(c); }
    
    // expose underlying char for legacy checks
    char getChar() const { return p.getChar(); }

    // Polymorphic behavior
    virtual char renderChar() const { return getChar(); }
    virtual int renderColor() const { return 7; }
    virtual bool isPassable() const { return false; }
    
    // Pure virtual function - must be implemented by children
    virtual char typeChar() const = 0; // e.g. 'K','4','S','*'
};