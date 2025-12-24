#pragma once
#include "GameObject.h"
#include <vector>
#include <string>
#include <memory>

class Switch : public GameObject {
    bool state = false;
public:
    Switch(int x, int y, int id) : GameObject(x, y, 'S', id) {}
    bool isOn() const { return state; }
    void setState(bool s) { state = s; }

    char renderChar() const override { return state ? 'S' : 's'; }
    int renderColor() const override { return state ? 10 : 7; }
    bool isPassable() const override { return true; } // stays blocking as map determines
    char typeChar() const override { return 'S'; }

    // Static factory method
    static std::vector<std::unique_ptr<Switch>> createFromMap(const std::vector<std::string>& mapData, int roomId);
};