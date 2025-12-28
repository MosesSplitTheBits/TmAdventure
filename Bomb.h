#pragma once
#include "GameObject.h"

class Bomb : public GameObject{
    bool collected = false;
    int ticksUntilExplosion = -1; //-1 means not planted

public:
    Bomb(int x, int y) : GameObject(x, y, 'B', -1) {}
    
    bool isCollected() const { return collected; }
    void collect() { collected = true; }

    bool isPlanted() const { return ticksUntilExplosion >= 0; }
    void plant(int ticks) { 
    ticksUntilExplosion = ticks; 
    collected = false;  // Mark as placed on ground
    }
    void tick() { 
        if (isPlanted() && ticksUntilExplosion > 0) {
            ticksUntilExplosion--; 
        }
    }
    bool hasExploded() const { return isPlanted() && ticksUntilExplosion == 0; }

    char renderChar() const override { 
        if (isPlanted()) return 'o'; 
        return collected ? ' ' : 'B'; 
    }
    int renderColor() const override { 
        if (isPlanted()) return 12; // Red for planted
        return collected ? 2 : 8; // Dark gray when collected
    }
    bool isPassable() const override { return isPlanted(); } // bombs block only when not planted
    virtual char typeChar() const override { return 'B'; }
    virtual bool interact(Game& game, Player& player) override;

};