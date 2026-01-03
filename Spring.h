#pragma once
#include "GameObject.h"
#include "Direction.h"

class Spring : public GameObject {
    int releaseDirection;

public:
    Spring(int x, int y, int dir) 
        : GameObject(x, y, '#', -1), releaseDirection(dir) {}

    // Called when player steps on the spring
    bool interact(Game& game, Player& player) override;
    char typeChar() const override { return '#'; }

    int getReleaseDirection() const { return releaseDirection; }

    //ObjectManager overrides
    bool isPassable() const override { return true; }
    char renderChar() const override { return '#'; }
    int renderColor() const override { return 14; } // Yellow color
};