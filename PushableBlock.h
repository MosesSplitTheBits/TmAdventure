#pragma once
#include "GameObject.h"
#include "Direction.h"
#include <map>

class PushableBlock : public GameObject {
private:
    // Track forces accumulated this frame for each direction
    std::map<int, int> directionForces; // direction index -> total force
    int width;  
    int height;  
    int anchorX; // Top-left X of the block
    int anchorY; // Top-left Y of the blockv
    
public:
    PushableBlock(int x, int y, int w = 3, int h = 2) 
        : GameObject(x, y, 'X', -1), width(w), height(h), anchorX(x), anchorY(y) {}
    
    bool interact(Game& game, Player& player) override;
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    // Reset forces at the start of each game frame
    void resetForces() { directionForces.clear(); }
    
    // Attempt to move the block if enough force accumulated
    void tryPush(Game& game);
    bool occupies(int x, int y) const;
    std::vector<std::pair<int, int>> getOccupiedTiles() const;
    
    // GameObject overrides
    char typeChar() const override { return 'X'; }
    char renderChar() const override { return 'X'; }
    int renderColor() const override { return 6; } // Yellow/brown
    bool isPassable() const override { return false; } // Can't walk through it
};