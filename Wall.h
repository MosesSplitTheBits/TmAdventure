#pragma once
#include "GameObject.h"

class Wall : public GameObject {
public:
    Wall(int x, int y) : GameObject(x, y, 'W', -1) {}
    char renderChar() const override { return 'W'; }
    int renderColor() const override { return 7; }
    bool isPassable() const override { return false; }
    char typeChar() const override { return 'W'; }
};