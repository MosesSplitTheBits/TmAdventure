#pragma once
#include "GameObject.h"

class Torch : public GameObject {
    bool collected = false;
public:
    Torch(int x, int y) : GameObject(x, y, 'T', -1) {}
    
    bool isCollected() const { return collected; }
    void collect() { collected = true; }

    char renderChar() const override { return collected ? ' ' : '!'; }
    int renderColor() const override { return collected ? 2 : 14; } // 14 = bright amber
    bool isPassable() const override { return true; } // torches don't block
    virtual char typeChar() const override { return 'T'; }
    virtual bool interact(Game& game, Player& player) override;
};