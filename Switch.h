#pragma once
#include "GameObject.h"
#include <vector>
#include <string>
#include <memory>

class Switch : public GameObject {
    bool state = false;
    bool isPressurePlate = false;
    bool isToggleSwitch = false;
    bool coveredByBlock = false;
public:
    Switch(int x, int y, int id, bool isPressure = false, bool isToggle = false)  // Update constructor
        : GameObject(x, y, isPressure ? 'P' : (isToggle ? '/' : 'S'), id), 
          isPressurePlate(isPressure), isToggleSwitch(isToggle) {}
    
    bool isOn() const { return state; }
    void setState(bool s) { state = s; }
    void setCovered(bool v) { coveredByBlock = v; }
    bool isPressure() const { return isPressurePlate; }
    bool isToggle() const { return isToggleSwitch; }

    char renderChar() const override;
    int renderColor() const override;
    bool isPassable() const override { return true; }
    char typeChar() const override { return isPressurePlate ? 'P' : (isToggleSwitch ? '/' : 'S'); }

    // Static factory method
    static std::vector<std::unique_ptr<Switch>> createFromMap(const std::vector<std::string>& mapData, int roomId);

    // static function to update switches
    static void updateAllSwitches(Game& game);
};