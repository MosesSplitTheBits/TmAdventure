#include "Switch.h"
#include "Game.h"
#include "Player.h"
#include "Obstacle.h"
#include "Door.h"
#include "Screen.h"
#include "PushableBlock.h"
#include <iostream>


std::vector<std::unique_ptr<Switch>> Switch::createFromMap(const std::vector<std::string>& mapData, int roomId) {
    std::vector<std::unique_ptr<Switch>> switches;
    for (int y = 0; y < (int)mapData.size(); ++y) {
        for (int x = 0; x < (int)mapData[y].size(); ++x) {
            if (mapData[y][x] == 'S') {
                switches.push_back(std::make_unique<Switch>(x, y, roomId, false, false));
            }
            else if (mapData[y][x] == 'P') {
                switches.push_back(std::make_unique<Switch>(x, y, roomId, true, false));
            }
            else if (mapData[y][x] == '/') { 
                switches.push_back(std::make_unique<Switch>(x, y, roomId, false, true));
            }
        }
    }
    return switches;
}

void Switch::updateAllSwitches(Game& game) {
    auto allSwitches = game.getSwitches();
    auto& p1 = game.getp1();
    auto& p2 = game.getp2();
    auto& screen = game.getScreen();
    auto pushableBlocks = game.getPushableBlocks();

    // 1. Update switch states
    for (auto s : allSwitches) {
        if (!s) continue;
        int sx = s->getPosition().getX();
        int sy = s->getPosition().getY();

        bool shouldToggle = false;
        bool covered = false;
        
        if (s->isPressure()) {
            // Pressure plate: activated by PushableBlock
            for (auto block : pushableBlocks) {
                if (block && block->occupies(sx, sy)) {
                    s->setState(true);
                    covered = true;
                    break;
                }
            }
        } 
        else if (s->isToggle()) {  // Add this block
            // Toggle switch: flip state when stepped on
            bool p1On = (p1.getPosition().getX() == sx && p1.getPosition().getY() == sy);
            bool p2On = (p2.getPosition().getX() == sx && p2.getPosition().getY() == sy);
            
            static bool wasPressed = false;
            bool currentlyPressed = (p1On || p2On);
            
            if (currentlyPressed && !wasPressed) {
                s->setState(!s->isOn());  // Toggle the state
                shouldToggle = true;
            }
            wasPressed = currentlyPressed;
        }
        else {
            // Regular switch: activated by player
            bool p1On = (p1.getPosition().getX() == sx && p1.getPosition().getY() == sy);
            bool p2On = (p2.getPosition().getX() == sx && p2.getPosition().getY() == sy);
            s->setState(p1On || p2On);
        }
        
        s->setCovered(covered);
        
        // Redraw if state changed or it's a toggle that was just activated
        if (shouldToggle || s->renderChar() != screen.getCharAt(sx, sy)) {
            screen.setCharAt(sx, sy, s->renderChar());
            screen.setColorAt(sx, sy, s->renderColor());
            screen.drawCell(sx, sy);
            
            // Redraw players if they're on the switch
            if (p1.getPosition().getX() == sx && p1.getPosition().getY() == sy) p1.draw();
            if (p2.getPosition().getX() == sx && p2.getPosition().getY() == sy) p2.draw();
        }
    }

    // 2. Update obstacles (existing code remains the same)
    for (auto obs : game.getObstacles()) {
        if (!obs) continue;
        int id = obs->getId();
        bool shouldOpen = false;

        for (auto s : allSwitches) {
            if (s && s->getId() == id && s->isOn()) {
                shouldOpen = true;
                break; 
            }
        }

        if (obs->isOpen() != shouldOpen) {
            obs->setOpen(shouldOpen);
            int ox = obs->getPosition().getX();
            int oy = obs->getPosition().getY();
            char visual = shouldOpen ? ' ' : '*';
            
            screen.setCharAt(ox, oy, visual);
            screen.setColorAt(ox, oy, shouldOpen ? 2 : 7);
            screen.drawCell(ox, oy);

            if (p1.getPosition().getX() == ox && p1.getPosition().getY() == oy) p1.draw();
            if (p2.getPosition().getX() == ox && p2.getPosition().getY() == oy) p2.draw();
        }
    }
}
char Switch::renderChar() const
{
    if (coveredByBlock) {
        return ' ';   // visually disappear under block
    }
    //Toggle switch
    if (isToggleSwitch) {
        return state ? '\\' : '/';
    }
    // Pressure plate
    if (isPressurePlate) {
        return state ? ' ' : 'p';
    }

    // Regular switch
    return state ? 'S' : 's';
}

int Switch::renderColor() const {
    if (isToggleSwitch) {
        return state ? 10 : 12;  // Green when on, Red when off
    }
    return state ? 10 : 7;  // Existing logic for other switches
}