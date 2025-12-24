#pragma once
#include "GameObject.h"

class Obstacle : public GameObject {
    char glyph;      // renamed to avoid conflict with typeChar()
    bool openFlag = false;
public:
    Obstacle(int x, int y, int obcId, char t)
        : GameObject(x, y, t, obcId), glyph(t), openFlag(false) {}

    // info
    char getType() const { return glyph; }
    bool isOpen() const { return openFlag; }
    void setOpen(bool v) { openFlag = v; }

    // GameObject overrides
    bool isPassable() const override { return openFlag; }
    char renderChar() const override { return openFlag ? ' ' : glyph; }
    int renderColor() const override { return openFlag ? 2 /*green*/ : 7 /*default*/; }
    char typeChar() const override { return glyph; }
};