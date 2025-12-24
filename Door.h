#pragma once
#include "GameObject.h"
#include "Room.h"


// Door class representing a door in the game
class Door : public GameObject {
private:
    Room* targetRoom = nullptr;
    bool isOpenState;
    bool switchControlled;

public:
    Door(int x, int y, int id, char type, Room* target, bool isSwitch = false)
        : GameObject(x, y, type, id), targetRoom(target), isOpenState(false), switchControlled(isSwitch) {}

    virtual char typeChar() const override { return getChar(); }

    void openDoor() { isOpenState = true; }
    void closeDoor() { isOpenState = false; }
    bool isOpen() const { return isOpenState; }
    
    // Passable if open OR if it's a back door ('3')
    virtual bool isPassable() const override { return isOpenState; }
    
    // --- FIX IS HERE ---
    virtual char renderChar() const override { return getChar(); }

    virtual int renderColor() const override { return isOpenState ? 2 : 4; } // Green if open, Red if closed

    Room* getTarget() const { return targetRoom; }
    bool isSwitchControlled() const { return switchControlled; }

    virtual bool interact(Game& game, Player& player) override;
};