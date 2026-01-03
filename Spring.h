#pragma once
#include "GameObject.h"
#include "Direction.h"

class Spring : public GameObject {
    int releaseDirection;
    bool compressed = false;

public:
    Spring(int x, int y, int dir) 
        : GameObject(x, y, '#', -1), releaseDirection(dir) {}

    // Called when player steps on the spring
    bool interact(Game& game, Player& player) override;
    char typeChar() const override { return '#'; }

    int getReleaseDirection() const { return releaseDirection; }
    void reset() { compressed = false; }

    //ObjectManager overrides
    bool isPassable() const override { return true; }
    char renderChar() const override { return compressed ? ' ' : '#'; }
    int renderColor() const override { return compressed ? 7 : 6; } // Yellow color
};