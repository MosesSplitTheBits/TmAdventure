#pragma once
#include "GameObject.h"
#include "Room.h"


class Door : public GameObject {
private:
    Room* targetRoom = nullptr;
    bool isLocked;
public:
    Door(int x, int y, int id, char type, Room* target, bool locked = true)
        : GameObject(x, y, type, id), targetRoom(target), isLocked(locked) {}


    // Implement pure virtual
    char typeChar() const override { return getChar(); }
    bool isPassable() const override { return !isLocked; }
    char renderChar() const override { return getChar(); }
    int renderColor() const override {
        // Keep locked doors red.
        if (isLocked) return 4;

        // Victory door: type '4' with no target room -> gold/amber.
        if (getChar() == '4' && targetRoom == nullptr) return 14;

        // Normal open door -> green.
        return 2;
    }
    bool interact(Game& game, Player& player) override;


    // Door state management
    void openDoor() { isLocked = false; }
    void closeDoor() { isLocked = true; }
    bool isOpen() const { return !isLocked; }
    
    


    Room* getTarget() const { return targetRoom; }
    void setTarget(Room* r) { targetRoom = r; }


     // פונקציה חדשה לניהול פתיחה בקרבה
    static void updateProximityDoors(Game& game);
    

    // animations
    static void animateOpening(int id, const std::vector<Door*>& doors, Screen& screen);
    static void makeDoorHollow(int id, const std::vector<Door*>& doors, Screen& screen);
};