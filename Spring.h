#pragma once
#include "GameObject.h"
#include "Direction.h"

class Spring : public GameObject {
    int releaseDirection;

public:
    // We'll use 'Z' as the map character for Springs
    Spring(int x, int y, int dir) 
        : GameObject(x, y, '#', false), releaseDirection(dir) {}

    // Called when player steps on the spring
    bool interact(Game& game, Player& player) override;
    char typeChar() const override { return '#'; }

    int getReleaseDirection() const { return releaseDirection; }
};