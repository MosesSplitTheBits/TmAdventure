#pragma once
#include "GameObject.h"

class Key : public GameObject {
    bool collected = false;
public:
    Key(int x, int y, int keyId) : GameObject(x, y, 'K', keyId) {}
    bool isCollected() const { return collected; }
    void collect() { collected = true; }

    
    char renderChar() const override { return collected ? ' ' : 'K'; }
    int renderColor() const override { return collected ? 2 : 14; }
    bool isPassable() const override { return true; } // keys don't block
    virtual char typeChar() const override { return 'K'; }
    virtual bool interact(Game& game, Player& player) override;
};
