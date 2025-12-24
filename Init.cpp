#include "Game.h"
#include "Switch.h"
#include "Obstacle.h"
#include "Door.h"
#include "Riddle.h"
#include "Room.h"
#include "Screen.h"
#include <memory>

void Game::initSwitches() {
    if (!currentRoom) return;
    auto newSwitches = Switch::createFromMap(currentRoom->getMapData(), currentRoom->getID());
    for (auto& s : newSwitches) {
        addObject(std::move(s));
    }
}

void Game::initObstacles() {
    if (!currentRoom) return;
    int roomId = currentRoom->getID();
    const auto& map = currentRoom->getMapData();

    for (int y = 0; y < (int)map.size(); ++y) {
        for (int x = 0; x < (int)map[y].size(); ++x) {
            char cell = map[y][x];
            if (cell == '*') {
                auto obs = std::make_unique<Obstacle>(x, y, roomId, 'S');
                addObject(std::move(obs));
            }
            else if (cell == '+') {
                auto obs = std::make_unique<Obstacle>(x, y, roomId, 'H');
                addObject(std::move(obs));
                auto door = std::make_unique<Door>(x, y, roomId, '4', currentRoom->getNext(), true);
                addObject(std::move(door));
                screen.setCharAt(x, y, '*');
            }
        }
    }
}

void Game::initDoors() {
    if (!currentRoom) return;
    int requiredKeyId = currentRoom->getID();
    const auto& map = currentRoom->getMapData();

    for (int y = 0; y < (int)map.size(); ++y) {
        for (int x = 0; x < (int)map[y].size(); ++x) {
            char cell = map[y][x];
            if (cell == '4') {
                auto door = std::make_unique<Door>(x, y, requiredKeyId, '4', currentRoom->getNext());
                addObject(std::move(door));
                screen.setCharAt(x, y, '4');
                screen.setColorAt(x, y, 4);
                screen.drawCell(x, y);
            }
            else if (cell == '3') {
                // FIX: Swapped arguments to match Door(x, y, id, type, target)
                // ID = 0, Type = '3'
                auto door = std::make_unique<Door>(x, y, currentRoom->getID(), '3', currentRoom->getPrev());
                door->openDoor();
                addObject(std::move(door));
                screen.setCharAt(x, y, '3');
                screen.setColorAt(x, y, 2);
                screen.drawCell(x, y);
            }
        }
    }
    
    for (auto d : getDoors()) {
        if (d && d->getChar() == '3') {
            d->openDoor();
            int dx = d->getPosition().getX();
            int dy = d->getPosition().getY();
            screen.setCharAt(dx, dy, d->renderChar());
            screen.setColorAt(dx, dy, 2);
            screen.drawCell(dx, dy);
        }
    }
}

void Game::initRiddles() {
    if (!currentRoom) return;
    auto newRiddles = Riddle::createFromMap(currentRoom->getMapData(), currentRoom->getID());
    for (auto& r : newRiddles) {
        addObject(std::move(r));
    }
}