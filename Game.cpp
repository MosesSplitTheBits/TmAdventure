#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <ctime> 
#include "menu.h"
#include "utils.h"
#include "Screen.h"
#include "Game.h"
#include "Player.h"
#include "Key.h"
#include "Switch.h"
#include "Obstacle.h"
#include "Door.h"
#include "Room.h" 
#include "LevelData.h"
#include "Wall.h"
#include "Point.h"
#include "Direction.h"
#include <algorithm>
#include <set>
#include <utility>
#include <memory>
#include "GameObject.h"
#include "Riddle.h" 

Game::Game(Screen& s, Player& p1_ref, Player& p2_ref, Room* startRoom)
    : screen(s), p1(p1_ref), p2(p2_ref), currentRoom(nullptr)
{
    puzzles.resetCounters(); 
    loadLevel(startRoom);
}

void startGame()
{
    system("cls");
    std::srand((unsigned)std::time(nullptr));

    Room* room1 = new Room(LevelData::MAP_1, 1);
    Room* room2 = new Room(LevelData::MAP_2, 2);
    Room* room3 = new Room(LevelData::MAP_3, 3);

    room1->setNext(room2);
    room2->setPrev(room1);
    room2->setNext(room3);
    room3->setPrev(room2);
    room3->setNext(nullptr); // Victory room
    Screen the_screen;

    const char keys1[7] = { 'w', 'x', 'a', 'd', 's', 'e', '\0' };
    Point p1_start = Point(10, 5, Direction::directions[Direction::STAY], '$');
    Player player1(keys1, the_screen, p1_start);

    const char keys2[7] = { 'i', 'm', 'j', 'l', 'k', 'o', '\0' };
    Point p2_start = Point(60, 10, Direction::directions[Direction::STAY], '&');
    Player player2(keys2, the_screen, p2_start);

    Game game(the_screen, player1, player2, room1);
    game.run();

    delete room1;
    delete room2;
    delete room3;
}

void Game::run()
{
    screen.draw();

    const char p1_drop_key = 'e';
    const char p2_drop_key = 'o';

    while (true)
    {
        if (_kbhit())
        {
            char ch = _getch();

            if (ch == 'h' || ch == 'H') {
                if (pause()) break;
            }

            if (ch == p1_drop_key || ch == toupper(p1_drop_key)) handleDrop(p1);
            if (ch == p2_drop_key || ch == toupper(p2_drop_key)) handleDrop(p2);

            p1.keyPreesed(ch);
            p2.keyPreesed(ch);
        }

        p1.move(*this);
        p2.move(*this);

        p1.getPosition().draw();
        p2.getPosition().draw();

        if (!handleEvents()) {
            // If the loop broke because of winning (not quitting via menu)
            if (isGameWon()) {
                system("cls");
                // Simple Win Screen
                std::cout << "\n\n\n";
                std::cout << "\t\t**********************************\n";
                std::cout << "\t\t*                                *\n";
                std::cout << "\t\t*    CONGRATULATIONS! YOU WON!   *\n";
                std::cout << "\t\t*                                *\n";
                std::cout << "\t\t**********************************\n\n";
                std::cout << "\t\tPress any key to return to menu...";
                _getch();
            }
            break;
        }
        

        Sleep(100);
    }
}

// --- NEW GENERIC HANDLER ---
bool Game::handlePlayerInteraction(Player& player) {
    int x = player.getPosition().getX();
    int y = player.getPosition().getY();
    
    GameObject* obj = objectAt(x, y);
    if (obj) {
        // This calls the code in Interactions.cpp
        return obj->interact(*this, player);
    }
    return true;
}

bool Game::handleEvents() {
    // 1. Generic Interaction (Polymorphism!)
    if (!handlePlayerInteraction(p1)) return false;
    if (!handlePlayerInteraction(p2)) return false;
    
    // 2. Global State Updates
    updateSwitches();
    updateProximityDoors();

    if (p1.hasWon() && p2.hasWon()) return false;
    return true;
}

bool Game::pause() {
    system("cls");
    std::cout << "=== PAUSED ===\n";
    std::cout << "Press any key to resume...\n";
    std::cout << "Press Q to quit to menu...\n";
    char ch = _getch();
    if (ch == 'q' || ch == 'Q') return true;
    
    system("cls");
    screen.draw();
    p1.draw();
    p2.draw();
    return false;
}

bool Game::isGameWon() const {
    return p1.hasWon() && p2.hasWon();
}

void Game::loadLevel(Room* nextRoom, bool comingBack) {
    if (currentRoom) {
        savedRoomObjects[currentRoom] = std::move(objects);
    }
    
    currentRoom = nextRoom;
    const auto& mapData = currentRoom->getMapData();
    screen.loadMap(mapData); 
    
    auto it = savedRoomObjects.find(nextRoom);
    if (it != savedRoomObjects.end()) {
        objects = std::move(it->second); 
        savedRoomObjects.erase(it);      
        rebuildObjectGrid();             

        // --- SYNC VISUALS ---
        for (int y = 0; y < (int)mapData.size(); ++y) {
            for (int x = 0; x < (int)mapData[y].size(); ++x) {
                char c = mapData[y][x];
                
                if (c == 'K' || c == '?' || c == '4' || c == '3' || c == '*' || c == '+') {
                    GameObject* obj = objectAt(x, y);
                    
                    if (!obj) {
                        if (c == 'K' || c == '?') screen.setCharAt(x, y, ' ');
                    } else {
                        if (auto d = dynamic_cast<Door*>(obj)) {
                            screen.setCharAt(x, y, d->renderChar());
                            screen.setColorAt(x, y, d->renderColor());
                        }
                        else if (auto obs = dynamic_cast<Obstacle*>(obj)) {
                            if (obs->isOpen()) screen.setCharAt(x, y, ' ');
                            else screen.setCharAt(x, y, c);
                        }
                        else if (auto r = dynamic_cast<Riddle*>(obj)) {
                            screen.setCharAt(x, y, r->typeChar());
                        }
                    }
                }
            }
        }

        // --- FIX: Re-apply "Hollow" effect for open doors ---
        // This ensures the door looks like a frame (4  4) instead of a block (4444)
        std::set<int> processedDoorIds;
        for (auto d : getDoors()) {
            if (d && d->isOpen() && d->getChar() == '4') {
                int id = d->getId();
                // Only process each door ID once
                if (processedDoorIds.find(id) == processedDoorIds.end()) {
                    makeDoorAreaHollow(id);
                    processedDoorIds.insert(id);
                }
            }
        }
    } else {
        objects.clear();
        objectGrid.clear();
        
        for (int y = 0; y < (int)mapData.size(); ++y) {
            for (int x = 0; x < (int)mapData[y].size(); ++x) {
                if (mapData[y][x] == 'W') {
                    addObject(std::make_unique<Wall>(x, y));
                }
            }
        }
        rebuildObjectGrid();

        // These functions are now in Init.cpp!
        initSwitches();
        initObstacles();
        initDoors();
        initRiddles(); 

        if (!currentRoom->isVisited()) {
            currentRoom->setVisited(true);
        }
    }

    if (comingBack) {
        bool doorFound = false;
        for (int y = 0; y <= Screen::MAX_Y; ++y) {
            for (int x = 0; x <= Screen::MAX_X; ++x) {
                if (screen.getCharAt(x, y) == '4') {
                    int nx = std::max(0, std::min(x - 3, Screen::MAX_X));
                    int py = std::min(y + 1, Screen::MAX_Y);
                    p1.setPosition(nx, y);
                    p1.getPosition().changeDirection(Direction::directions[Direction::STAY]);
                    p2.setPosition(nx, py);
                    p2.getPosition().changeDirection(Direction::directions[Direction::STAY]);
                    doorFound = true;
                    break;
                }
            }
            if (doorFound) break;
        }
        if (!doorFound) { p1.setPosition(3, 2); p2.setPosition(75, 2); }
    } else {
        bool doorFound = false;
        for (int y = 0; y <= Screen::MAX_Y; ++y) {
            for (int x = 0; x <= Screen::MAX_X; ++x) {
                if (screen.getCharAt(x, y) == '3') {
                    int nx = std::max(0, std::min(x - 4, Screen::MAX_X));
                    int py = std::min(y + 1, Screen::MAX_Y);
                    p1.setPosition(nx, y);
                    p1.getPosition().changeDirection(Direction::directions[Direction::STAY]);
                    p2.setPosition(nx, py);
                    p2.getPosition().changeDirection(Direction::directions[Direction::STAY]);
                    doorFound = true;
                    break;
                }
            }
            if (doorFound) break;
        }
        if (!doorFound) { p1.setPosition(3, 2); p2.setPosition(75, 2); }
    }

    if (currentRoom && currentRoom->getID() == 2) {
        p1.setPosition(4, 5);
        p2.setPosition(5, 5);
        p1.getPosition().changeDirection(Direction::directions[Direction::STAY]);
        p2.getPosition().changeDirection(Direction::directions[Direction::STAY]);
    }
}

void Game::handleDrop(Player& player) {
    int droppedKeyId = player.dropKey();
    if (droppedKeyId == -1) return;

    int dropX = player.getPosition().getX();
    int dropY = player.getPosition().getY();
    Direction dir = player.getPosition().getDir();
    int keyX = dropX - dir.dx();
    int keyY = dropY - dir.dy();

    addObject(std::make_unique<Key>(keyX, keyY, droppedKeyId));
    screen.setCharAt(keyX, keyY, 'K');
    gotoxy(keyX, keyY);
    std::cout << 'K' << std::flush;
    player.draw();
}

void Game::updateSwitches() {
    auto allSwitches = getSwitches();

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

    for (auto obs : getObstacles()) {
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

    for (auto d : getDoors()) {
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

bool Game::withinRadius2(const Point& a, const Point& b) {
    int dx = a.getX() - b.getX();
    int dy = a.getY() - b.getY();
    return (dx * dx + dy * dy) <= 4;
}

void Game::updateProximityDoors() {
    for (auto triggerDoor : getDoors()) {
        if (!triggerDoor || triggerDoor->isOpen()) continue;
        int id = triggerDoor->getId();
        if (id == -1) continue;

        Player* opener = nullptr;
        if (p1.hasKey(id) && withinRadius2(p1.getPosition(), triggerDoor->getPosition())) opener = &p1;
        else if (p2.hasKey(id) && withinRadius2(p2.getPosition(), triggerDoor->getPosition())) opener = &p2;
        if (!opener) continue;

        opener->useKey(id);

        for (auto door : getDoors()) {
            if (door && door->getId() == id && !door->isOpen()) {
                door->openDoor();
                int dx = door->getPosition().getX();
                int dy = door->getPosition().getY();
                screen.setCharAt(dx, dy, door->renderChar());
                screen.setColorAt(dx, dy, door->renderColor());
                screen.drawCell(dx, dy);
            }
        }
        animateDoorOpening(id);
    }
}

void Game::makeDoorAreaHollow(int id) {
    std::vector<std::pair<int,int>> positions;
    for (auto d : getDoors()) {
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
    for (auto d : getDoors()) if (d && d->getId() == id) { color = d->renderColor(); break; }

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

void Game::animateDoorOpening(int id) {
    std::vector<std::pair<int,int>> positions;
    for (auto d : getDoors()) {
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

    makeDoorAreaHollow(id);
}
