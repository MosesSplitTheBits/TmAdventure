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
#include "VisionSystem.h"
#include "InputAction.h"
#include "ResultsRecorder.h"
#include "StepsRecorder.h"
#include "StepsLoader.h"




Game::Game(Screen& s, Player& p1_ref, Player& p2_ref, Room* startRoom, GameMode mode)
    : screen(s), p1(p1_ref), p2(p2_ref), currentRoom(nullptr), mode(mode),inputManager(mode, nullptr)
{
    puzzles.resetCounters(); 
    loadLevel(startRoom);
    visionDirty = true;

    // CHECK MODE
    if (mode == GameMode::Save) {
        stepsRecorder = std::make_unique<StepsRecorder>("adv-world.steps");
        resultsRecorder = std::make_unique<ResultsRecorder>("adv-world.result");
        unsigned int seed = static_cast<unsigned int>(std::time(nullptr));
        std::srand(seed);
        stepsRecorder->setRandomSeed(seed);
    }
    if (mode == GameMode::Load) {
    stepsLoader = std::make_unique<StepsLoader>("adv-world.steps");
    std::srand(stepsLoader->getRandomSeed());
}
    // Initialize InputManager with StepsLoader if in Load mode
    inputManager = InputManager(mode, stepsLoader.get());
}

void startGame(GameMode mode, bool silent)
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
        //set first room to dark
        room1->setDark(true);
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

    Game game(the_screen, player1, player2, room1, mode);
    game.run();

    delete room1;
    delete room2;
    delete room3;
}

void Game::run()
{
    statusBar.resetRunTime();
    const char p1_drop_key = 'e';
    const char p2_drop_key = 'o';

    // Initial render
    renderFrame();
    
    gameTime = 0; //record time played
    while (true)
    {
        InputAction action = inputManager.getAction(gameTime);
        // RECORD STEP IF IN SAVE MODE
        if (mode == GameMode::Save && stepsRecorder && action.type != ActionType::None) {
    stepsRecorder->recordStep(gameTime, action);
}
        gameTime++;

switch (action.type) // Handle input actions
{
case ActionType::Pause:
    if (pause()) return;
    break;

case ActionType::Quit:
    return;

case ActionType::DropItem:
    if (action.playerId == 0 && !p1.isWaitingAtDoor()) {
        p1.tryDropItem(*this);
    }
    else if (action.playerId == 1 && !p2.isWaitingAtDoor()) {
        p2.tryDropItem(*this);
    }
    break;

case ActionType::Move:
    if (!p1.isWaitingAtDoor()) {
        p1.keyPreesed(action.rawKey);
    }
    if (!p2.isWaitingAtDoor()) {
        p2.keyPreesed(action.rawKey);
    }
    break;

case ActionType::None:
default:
    break;
}

        if (!p1.isWaitingAtDoor()) p1.move(*this);
        if (!p2.isWaitingAtDoor()) p2.move(*this);

        if (!handleEvents()) {
            if (isGameWon()) {
                if (mode == GameMode::Save && resultsRecorder) {
                    resultsRecorder->recordGameEnd(gameTime, 0);
                }
                system("cls");
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
                if (mode == GameMode::Save && resultsRecorder) {
                    resultsRecorder->recordGameEnd(gameTime, 0);
                }
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

        // Render after all game logic completes
        renderFrame();

        Sleep(40);
    }
}

// --- FULL-FRAME RENDERING ENGINE ---
void Game::renderFrame()
{
    // 1. Clear buffer (reset to spaces and white)
    screen.clear();
    
    // 2. Load map tiles into buffer
    const auto& mapData = currentRoom->getMapData();
    screen.loadMap(mapData);

    // 3. Apply vision system (fog of war)
    auto vision = visionSystem.compute(*currentRoom, p1, p2, getTorches());
    screen.applyVision(mapData, vision);
    
    // 4. Render all objects to buffer
    for (const auto& obj : objects) {
        if (auto pb = dynamic_cast<PushableBlock*>(obj.get())) {
            for (const auto& [x, y] : pb->getOccupiedTiles()) {
                screen.setCharAt(x, y, pb->renderChar());
                screen.setColorAt(x, y, pb->renderColor());
            }
        } else {
            screen.setCharAt(obj->getPosition().getX(),
                             obj->getPosition().getY(),
                             obj->renderChar());
            screen.setColorAt(obj->getPosition().getX(),
                              obj->getPosition().getY(),
                              obj->renderColor());
        }
    }
    
    
    // 5. Render players to buffer
    if (!p1.isWaitingAtDoor()) {
        screen.setCharAt(p1.getPosition().getX(), p1.getPosition().getY(), p1.renderChar());
        screen.setColorAt(p1.getPosition().getX(), p1.getPosition().getY(), p1.renderColor());
    }
    
    if (!p2.isWaitingAtDoor()) {
        screen.setCharAt(p2.getPosition().getX(), p2.getPosition().getY(), p2.renderChar());
        screen.setColorAt(p2.getPosition().getX(), p2.getPosition().getY(), p2.renderColor());
    }
    
    // 6. Render StatusBar to buffer
    statusBar.draw(screen, currentRoom->getID(), 
                   p1.getHP(), p1.getMaxHP(), 
                   p2.getHP(), p2.getMaxHP(), 
                   p1.getHeldItemChar(), p2.getHeldItemChar());
    
    // 7. Single console output (actual rendering)
    screen.draw();
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

    rebuildObjectGrid();
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
    
    if (mode == GameMode::Save && resultsRecorder) {
        resultsRecorder->recordScreenChange(gameTime, currentRoom->getID());
    }
    
    // 3. Load Map Background (Walls and Floor)
    const auto& mapData = currentRoom->getMapData();
    visionDirty = true;
    
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
        
        // Clear object markers from mapData (objects now in objects vector)
        for (const auto& obj : objects) {
            int x = obj->getPosition().getX();
            int y = obj->getPosition().getY();
            char typeChar = obj->typeChar();
            // Clear pickupable objects and switches from map
            if (typeChar == 'T' || typeChar == 'K' || typeChar == 'B' || 
                typeChar == '!' || typeChar == 'S' || typeChar == 'P' || 
                typeChar == '/' || typeChar == '?' || typeChar == 'X') {
                currentRoom->setCell(x, y, ' ');
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


    // SECOND PASS: Force draw all PushableBlocks on top of everything else
    for (const auto& obj : objects) {
        if (auto pb = dynamic_cast<PushableBlock*>(obj.get())) {
            auto tiles = pb->getOccupiedTiles();
            for (const auto& [x, y] : tiles) {
                screen.setCharAt(x, y, pb->renderChar());
                screen.setColorAt(x, y, pb->renderColor());
            }
        }
    }
    
    // 6. Handle specific visual updates (like hollow doors)
    // This is cleaner than doing it inside the render loop
    Door::updateProximityDoors(*this);

    // 7. Position Players
    spawnPlayers(comingBack);

    // 8. Force full screen render to display new room
    renderFrame();

}

void Game::spawnPlayers(bool comingBack) {
    // Default fallback position
    int p1x = 4, p1y = 6;
    int p2x = 5, p2y = 6;
    bool foundSpawn = false;

    // Case 2: Coming Back (Find the door we just exited from)
     if (comingBack && lastUsedDoor) {
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



void Game::damagePlayer(Player& player, int amount)
{
    if (player.isWaitingAtDoor()) return; // לא פוגעים בשחקן "שנעלם"
    player.takeDamage(amount);
    
    if (player.isDead() && mode == GameMode::Save && resultsRecorder) {
        resultsRecorder->recordLifeLost(gameTime);
    }
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






