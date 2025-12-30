#include "Door.h"
#include "Screen.h"
#include <windows.h> // בשביל Sleep
#include <algorithm>
#include <iostream>
#include <set>
#include "Game.h"




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

            screen.setCharAt(x, y, keepAs4 ? '4' : ' ');
            screen.setColorAt(x, y, color);
            screen.drawCell(x, y);
        }
    }
}

void Door::animateOpening(int id, const std::vector<Door*>& doors, Screen& screen) 
{
    std::vector<std::pair<int,int>> positions;
    for (auto d : doors) {
        // Only animate '4' doors
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

    std::set<std::pair<int,int>> posset(positions.begin(), positions.end());

    // 1. Flash Warm Color (Frame only)
    const int warmColor = 6;
    for (const auto& p : positions) {
        if (p.first == minx || p.first == maxx || p.second == miny || p.second == maxy) {
            screen.setColorAt(p.first, p.second, warmColor);
            screen.setCharAt(p.first, p.second, '4');
            screen.drawCell(p.first, p.second);
        }
    }
    Sleep(120);

    // 2. Clear the Inside (Hollow out logic)
    int width = maxx - minx + 1;
    int height = maxy - miny + 1;
    int layers = std::min((width - 1) / 2, (height - 1) / 2);

    for (int layer = 1; layer <= layers; ++layer) {
        int lx = minx + layer, rx = maxx - layer;
        int ty = miny + layer, by = maxy - layer;

        for (int y = ty; y <= by; ++y) {
            for (int x = lx; x <= rx; ++x) {
                if (!posset.count({x,y})) continue;
                screen.setCharAt(x, y, ' ');
                screen.setColorAt(x, y, 2);
                screen.drawCell(x, y);
                Sleep(20);
            }
        }
        Sleep(60);
    }

    // 3. Clear bottom row (for the "Arch" look)
    if (maxx - minx >= 2 && maxy - miny >= 1) {
        int by = maxy;
        for (int x = minx + 1; x <= maxx - 1; ++x) {
            if (!posset.count({x, by})) continue;
            screen.setCharAt(x, by, ' ');
            screen.setColorAt(x, by, 2);
            screen.drawCell(x, by);
            Sleep(20);
        }
        Sleep(60);
    }

    // 4. Restore Frame Color to Green
    for (const auto& p : positions) {
        if (p.first == minx || p.first == maxx || p.second == miny || p.second == maxy) {
            screen.setColorAt(p.first, p.second, 2);
            screen.setCharAt(p.first, p.second, '4');
            screen.drawCell(p.first, p.second);
        }
    }

    // קריאה לפונקציה הסטטית השנייה
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
                int dx = door->getPosition().getX();
                int dy = door->getPosition().getY();
                screen.setCharAt(dx, dy, door->renderChar());
                screen.setColorAt(dx, dy, door->renderColor());
                screen.drawCell(dx, dy);
            }
        }
        // הפעלת האנימציה
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