#include "Door.h"
#include "Screen.h"
#include <windows.h> // בשביל Sleep
#include <algorithm>
#include <iostream>
#include <set>
#include "Game.h"
#include "Sound.h"




void Door::makeDoorHollow(int id, const std::vector<Door*>& doors, Screen& screen) 
{
    std::vector<std::pair<int,int>> positions;
    for (auto d : doors) {
        // Only process '4' doors
        if (d && d->getId() == id && d->getChar() == '4') {
            positions.push_back({d->getPosition().getX(), d->getPosition().getY()});
        }
    }
    if (positions.empty()) return;

    int minx = positions[0].first, maxx = positions[0].first;
    int miny = positions[0].second, maxy = positions[0].second;
    for (auto &p : positions) {
        minx = std::min(minx, p.first); maxx = std::max(maxx, p.first);
        miny = std::min(miny, p.second); maxy = std::max(maxy, p.second);
    }

    int color = 2;
    for (auto d : doors) if (d && d->getId() == id) { color = d->renderColor(); break; }

    std::set<std::pair<int,int>> posset(positions.begin(), positions.end());

    bool isSmallDoor = (maxx - minx < 2) || (maxy - miny < 1);

    for (int y = miny; y <= maxy; ++y) {
        for (int x = minx; x <= maxx; ++x) {
            if (!posset.count({x,y})) continue;
            
            bool keepAs4 = false;
            if (!isSmallDoor) {
                // Keep frame: Top, Left, Right (Bottom is cleared for walking through)
                keepAs4 = (x == minx || x == maxx || y == miny);
            }
        }
    }
}

void Door::animateOpening(int id, const std::vector<Door*>& doors, Screen& screen) 
{
    // INSTANT OPENING - No animation, no Sleep(), no drawCell()
    // Just update buffer via makeDoorHollow
    makeDoorHollow(id, doors, screen);
}

// פונקציית עזר קטנה לחישוב מרחק (אפשר לשים אותה ב-utils.h בעתיד)
bool isNear(const Point& a, const Point& b) 
{
    int dx = a.getX() - b.getX();
    int dy = a.getY() - b.getY();
    return (dx * dx + dy * dy) <= 4;
}

void Door::updateProximityDoors(Game& game) 
{
    auto doors = game.getDoors();
    auto& p1 = game.getp1();
    auto& p2 = game.getp2();
    auto& screen = game.getScreen();

    for (auto triggerDoor : doors) {
        if (!triggerDoor || triggerDoor->isOpen()) continue;
        int id = triggerDoor->getId();
        if (id == -1) continue;

        Player* opener = nullptr;
        
        // בדיקה אם שחקן קרוב ויש לו מפתח
        if (p1.hasKey(id) && isNear(p1.getPosition(), triggerDoor->getPosition())) opener = &p1;
        else if (p2.hasKey(id) && isNear(p2.getPosition(), triggerDoor->getPosition())) opener = &p2;
        
        if (!opener) continue;

        opener->useKey(id);

        // פתיחת כל הדלתות עם אותו ID
        for (auto door : doors) {
            if (door && door->getId() == id && !door->isOpen()) {
                door->openDoor();
            }
        }
        // Instant opening
        Door::animateOpening(id, doors, screen);
    }
}

bool Door::interact(Game& game, Player& player) 
{
    if (!isPassable()) {
        return true;
    }

    const char doorType = getChar();
    Room* const target = getTarget();

    // Victory door: open '4' with no target
    if (doorType == '4' && target == nullptr) {
        Sound::DoorWinOpen();
        player.setWon(true);
        return true;
    }

    // Transition doors: '3' or '4' with a target room
    if ((doorType == '3' || doorType == '4') && target != nullptr) {
        Player& p1 = game.getp1();
        Player& p2 = game.getp2();

        // Mark this player as waiting (once)
        if (!player.isWaitingAtDoor()) {
            player.setWaitingAtDoor(true);
            player.getPosition().erase();
        }

        // Transition only when both players are waiting
        if (p1.isWaitingAtDoor() && p2.isWaitingAtDoor()) {
            p1.setWaitingAtDoor(false);
            p2.setWaitingAtDoor(false);

            game.setLastUsedDoor(this);
            game.loadLevel(target, doorType == '3');
        }

        return true;
    }

    return true;
}