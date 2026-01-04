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
#include "Wall.h"
#include "Point.h"
#include "Direction.h"
#include <algorithm>
#include <set>
#include <utility>
#include <memory>
#include "GameObject.h"
#include "Puzzle.h"
#include "Riddle.h" 
#include "ObjectFactory.h"
#include "Bomb.h"
#include "MapLoader.h"

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

    Room* room1 = nullptr;
    Room* room2 = nullptr;
    Room* room3 = nullptr;

    try 
    {
        room1 = new Room(MapLoader::load("Maps/map_1.txt"), 1);
        room2 = new Room(MapLoader::load("Maps/map_2.txt"), 2);
        room3 = new Room(MapLoader::load("Maps/map_3.txt"), 3);
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Failed to load maps: " << e.what() << std::endl;
        _getch();
        return;
    }

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
    statusBar.resetRunTime();
    const char p1_drop_key = 'e';
    const char p2_drop_key = 'o';

    while (true)
    {
        int roomId = currentRoom ? currentRoom->getID() : 0;
        int puzzleFails = puzzles.getTotalWrongAttempts();
        statusBar.draw(currentRoom->getID(), p1.getHP(), p1.getMaxHP(), p2.getHP(), p2.getMaxHP(), p1.getHeldItemChar(), p2.getHeldItemChar());

        if (_kbhit())
        {
            char ch = _getch();

            if (ch == 'h' || ch == 'H') {
                if (pause()) break;
            }

            // Only process input for players who aren't waiting at door
            if (!p1.isWaitingAtDoor()) {
                if (ch == p1_drop_key || ch == toupper(p1_drop_key)) {
                    p1.tryDropItem(*this);
                }
                p1.keyPreesed(ch);
            }
            if (!p2.isWaitingAtDoor()) {
                if (ch == p2_drop_key || ch == toupper(p2_drop_key)) {
                     p2.tryDropItem(*this);
            }
            p2.keyPreesed(ch);
}
        }

        // Only move and draw players who aren't waiting at door
        if (!p1.isWaitingAtDoor()) p1.move(*this);
        if (!p2.isWaitingAtDoor()) p2.move(*this);

        if (!handleEvents()) {
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
            else if (p1.isDead() || p2.isDead()) 
            {
                system("cls");
                std::cout << "\n\n\n";
                std::cout << "\t\t**********************************\n";
                std::cout << "\t\t*                                *\n";
                std::cout << "\t\t*          GAME OVER!            *\n";
                std::cout << "\t\t*      A PLAYER RAN OUT OF HP    *\n";
                std::cout << "\t\t*                                *\n";
                std::cout << "\t\t**********************************\n\n";
                std::cout << "\t\tPress any key to return to menu...";
                _getch();
            }
            break;
        }

        refreshVision();
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

    // 1. Generic Interaction (Polymorphism!) - Skip players waiting at door
    if (!p1.isWaitingAtDoor() && !handlePlayerInteraction(p1)) return false;
    if (!p2.isWaitingAtDoor() && !handlePlayerInteraction(p2)) return false;
    
    // 2 Try to push blocks after both players have interacted
    for (auto block : getPushableBlocks()) {
        block->tryPush(*this);
    }

    // 3. Global State Updates
    Switch::updateAllSwitches(*this);
    Door::updateProximityDoors(*this);
    Bomb::handleBombExplosions(*this);

    if (currentRoom && currentRoom->getID() == 3) 
    {
        if (p1.hasWon()) p1.setWaitingAtDoor(true);
        if (p2.hasWon()) p2.setWaitingAtDoor(true);
    }

    // NEW: Game over if any player died
    if (p1.isDead() || p2.isDead()) {
        return false;
    }

    // Check for game end (only in final room when both actually won)
    if (currentRoom && currentRoom->getID() == 3 && p1.hasWon() && p2.hasWon()) return false;
    return true;
}

bool Game::pause() {
    system("cls");
    std::cout << "=== PAUSED ===\n";
    std::cout << "Press any key to resume...\n";
    std::cout << "Press ESC to quit to menu...\n";
    char ch = _getch();
    if (ch == 27) return true;
    
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
    // 1. Save current room state before leaving
    if (currentRoom) {
        savedRoomObjects[currentRoom] = std::move(objects);
    }
    
    // 2. Switch Room
    currentRoom = nextRoom;
    p1.setWaitingAtDoor(false);
    p2.setWaitingAtDoor(false);
    
    // 3. Load Map Background (Walls and Floor)
    const auto& mapData = currentRoom->getMapData();
    screen.loadMap(mapData); 
    
    // 4. Load Objects (Either restore from save or create new)
    auto it = savedRoomObjects.find(nextRoom);
    if (it != savedRoomObjects.end()) {
        // RESTORE: Move objects back from storage
        objects = std::move(it->second); 
        // Do NOT erase from map, so we can save it again later
    } else {
        // NEW: Create objects from scratch using Factory
        objects.clear();
        objectGrid.clear(); // Clear the lookup grid
        
        for (int y = 0; y < (int)mapData.size(); ++y) {
            for (int x = 0; x < (int)mapData[y].size(); ++x) {
                char tile = mapData[y][x];
                // Factory handles Doors, Enemies, Keys, etc.
                if (auto obj = ObjectFactory::createFromTile(*this, x, y, tile)) {
                    addObject(std::move(obj));
                }
            }
        }
        
        // Add invisible HUD wall
        for (int x = 0; x <= Screen::MAX_X; ++x) {
            addObject(std::make_unique<Wall>(x, 21));
        }

        if (!currentRoom->isVisited()) {
            currentRoom->setVisited(true);
        }
    }

    // 5. Rebuild Grid & Render Objects (The "Heritage" Way)
    rebuildObjectGrid();             

    // Iterate ALL objects and let them draw themselves. 
    // No more manual checks for "is this a door?" or "is this a key?"
    for (const auto& obj : objects) {
    // Special handling for multi-tile PushableBlocks
    if (auto pb = dynamic_cast<PushableBlock*>(obj.get())) {
        // Draw all tiles of the block
        auto tiles = pb->getOccupiedTiles();
        for (const auto& [x, y] : tiles) {
            screen.setCharAt(x, y, pb->renderChar());
            screen.setColorAt(x, y, pb->renderColor());
        }
    } else {
        // Single-tile objects (normal behavior)
        screen.setCharAt(obj->getPosition().getX(), obj->getPosition().getY(), obj->renderChar());
        screen.setColorAt(obj->getPosition().getX(), obj->getPosition().getY(), obj->renderColor());
    }
}


    // 6. Handle specific visual updates (like hollow doors)
    // This is cleaner than doing it inside the render loop
    Door::updateProximityDoors(*this);

    // 7. Position Players
    spawnPlayers(comingBack);

    // 8. Final Vision Update
    refreshVision();
}

void Game::spawnPlayers(bool comingBack) {
    // Default fallback position
    int p1x = 4, p1y = 6;
    int p2x = 5, p2y = 6;
    bool foundSpawn = false;

    // Manual override for MAP_2 only
    if (currentRoom && currentRoom->getID() == 2) {
        p1x = 4;  
        p1y = 6;  
        p2x = 6;  
        p2y = 6;  
        foundSpawn = true;
    }

    // Case 2: Coming Back (Find the door we just exited from)
    else if (comingBack && lastUsedDoor) {
        // We look for a door that leads to the room we just came from
        // But simpler: look for the '4' (Exit) door in this room
        for (auto d : getDoors()) {
            if (d && d->getChar() == '4') {
                // Spawn near the exit door
                p1x = std::max(1, d->getPosition().getX() - 3);
                p1y = d->getPosition().getY();
                p2x = p1x;
                p2y = std::min(d->getPosition().getY() + 1, Screen::MAX_Y);
                foundSpawn = true;
                break;
            }
        }
    }
    // Case 3: Going Forward (Find the '3' Entrance)
    else {
        // Scan map for '3'
        const auto& map = currentRoom->getMapData();
        for (int y = 0; y < map.size() && !foundSpawn; ++y) {
            for (int x = 0; x < map[y].size(); ++x) {
                if (map[y][x] == '3') {
                    p1x = std::max(1, x - 4);
                    p1y = y;
                    p2x = p1x;
                    p2y = std::min(y + 1, Screen::MAX_Y);
                    foundSpawn = true;
                    break;
                }
            }
        }
    }

    // Apply positions
    p1.setPosition(p1x, p1y);
    p2.setPosition(p2x, p2y);
    
    // Reset directions
    p1.getPosition().changeDirection(Direction::directions[Direction::STAY]);
    p2.getPosition().changeDirection(Direction::directions[Direction::STAY]);
}


//Calc vision radius for dark rooms
void Game::refreshVision() {
    bool dark = false;
    if (currentRoom) {
        const auto& map = currentRoom->getMapData();
        for (const auto& row : map) {
            if (row.find('T') != std::string::npos) { dark = true; break; }
        }
    }

    // Only calculate vision for players who are NOT waiting at door
    int r1 = (p1.hasTorch() && !p1.isWaitingAtDoor()) ? 20 : (p1.isWaitingAtDoor() ? 0 : 10);
    int r2 = (p2.hasTorch() && !p2.isWaitingAtDoor()) ? 20 : (p2.isWaitingAtDoor() ? 0 : 10);

    std::vector<std::pair<int, int>> floorTorches;
    if (dark) {
        for (auto t : getTorches()) {
            if (t && !t->isCollected()) {
                floorTorches.push_back({t->getPosition().getX(), t->getPosition().getY()});
            }
        }
    }

    auto inVision = [&](int x, int y) {
        if (!dark) return true;

        // Skip vision calculation for players waiting at door (set radius to 0)
        int d1 = std::abs(p1.getPosition().getX() - x) + std::abs(p1.getPosition().getY() - y);
        int d2 = std::abs(p2.getPosition().getX() - x) + std::abs(p2.getPosition().getY() - y);
        
        if (!p1.isWaitingAtDoor() && d1 <= r1) return true;
        if (!p2.isWaitingAtDoor() && d2 <= r2) return true;

        for (const auto& torch : floorTorches) {
            int dt = std::abs(torch.first - x) + std::abs(torch.second - y);
            if (dt <= 5) return true;
        }
        return false;
    };

    for (int y = 0; y <= Screen::MAX_Y; ++y) {
        if (y >= 21) continue;

        for (int x = 0; x <= Screen::MAX_X; ++x) {
            // אל תכסה את השחקנים כאן (הם מצוירים בנפרד או בפונקציה שלהם)
            if ((!p1.isWaitingAtDoor() &&
                 x == p1.getPosition().getX() && y == p1.getPosition().getY()) ||
                (!p2.isWaitingAtDoor() &&
                 x == p2.getPosition().getX() && y == p2.getPosition().getY()))
            {
                continue;
            }

            // Don't overwrite PushableBlocks - they should always be visible
            bool isPushableBlock = false;
            for (auto block : getPushableBlocks()) {
                if (block && block->occupies(x, y)) {
                    isPushableBlock = true;
                    break;
                }
            }
            if (isPushableBlock) continue;
            bool vision = inVision(x, y);

            char ch = (!dark) ? screen.getCharAt(x, y) : (vision ? ' ' : 'd');
            int color = (!dark) ? screen.getColorAt(x, y) : (vision ? 7 : 8);

            if (auto obj = objectAt(x, y)) {
                bool visible = !dark || obj->typeChar() == 'W' || obj->typeChar() == 'T' || vision;
                if (visible) { ch = obj->renderChar(); color = obj->renderColor(); }
            }

            screen.setCharAt(x, y, ch);
            screen.setColorAt(x, y, color);
            screen.drawCell(x, y);
        }
    }
}


void Game::damagePlayer(Player& player, int amount)
{
    if (player.isWaitingAtDoor()) return; // לא פוגעים בשחקן "שנעלם"
    player.takeDamage(amount);
}

void Game::damagePlayersInManhattanRange(const Point& center, int range, int amount)
{
    auto dist = [&](const Player& pl) {
        return std::abs(pl.getPosition().getX() - center.getX()) +
               std::abs(pl.getPosition().getY() - center.getY());
    };

    if (!p1.isWaitingAtDoor() && dist(p1) <= range) damagePlayer(p1, amount);
    if (!p2.isWaitingAtDoor() && dist(p2) <= range) damagePlayer(p2, amount);
}






