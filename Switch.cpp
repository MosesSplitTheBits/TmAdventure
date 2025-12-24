#include "Switch.h"
#include "Game.h"
#include "Player.h"
#include "Obstacle.h"
#include "Door.h"
#include "Screen.h"
#include <iostream>

// ...existing code...

std::vector<std::unique_ptr<Switch>> Switch::createFromMap(const std::vector<std::string>& mapData, int roomId) {
    std::vector<std::unique_ptr<Switch>> switches;
    for (int y = 0; y < (int)mapData.size(); ++y) {
        for (int x = 0; x < (int)mapData[y].size(); ++x) {
            if (mapData[y][x] == 'S') {
                switches.push_back(std::make_unique<Switch>(x, y, roomId));
            }
        }
    }
    return switches;
}

void Switch::updateAllSwitches(Game& game) {
    auto allSwitches = game.getSwitches();
    auto& p1 = game.getp1(); // נניח שיש גטרים כאלו ב-Game
    auto& p2 = game.getp2();
    auto& screen = game.getScreen();

    // 1. עדכון מצב המתגים
    for (auto s : allSwitches) {
        if (!s) continue;
        int sx = s->getPosition().getX();
        int sy = s->getPosition().getY();

        bool p1On = (p1.getPosition().getX() == sx && p1.getPosition().getY() == sy);
        bool p2On = (p2.getPosition().getX() == sx && p2.getPosition().getY() == sy);
        bool isPressed = (p1On || p2On);

        if (s->isOn() != isPressed) {
            s->setState(isPressed);
            if (p1On) p1.draw();
            if (p2On) p2.draw();
        }
    }

    // 2. עדכון מכשולים (Obstacles)
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
            gotoxy(ox, oy);
            std::cout << visual << std::flush;

            if (p1.getPosition().getX() == ox && p1.getPosition().getY() == oy) p1.draw();
            if (p2.getPosition().getX() == ox && p2.getPosition().getY() == oy) p2.draw();
        }
    }

    // 3. עדכון דלתות
    for (auto d : game.getDoors()) {
        if (d && d->isSwitchControlled()) {
            int id = d->getId();
            bool shouldOpen = false;

            for (auto s : allSwitches) {
                if (s && s->getId() == id && s->isOn()) {
                    shouldOpen = true;
                    break;
                }
            }
            if (shouldOpen) d->openDoor();
            else d->closeDoor();
        }
    }
}