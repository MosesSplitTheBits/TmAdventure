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
#include "Puzzle.h"
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
    statusBar.resetRunTime();
    const char p1_drop_key = 'e';
    const char p2_drop_key = 'o';

    while (true)
    {
        int roomId = currentRoom ? currentRoom->getID() : 0;
        
        // 1. שליפת כמות הטעויות מהחידות
        int puzzleFails = puzzles.getTotalWrongAttempts();

        statusBar.draw(roomId, puzzleFails);

        if (_kbhit())
        {
            char ch = _getch();

            if (ch == 'h' || ch == 'H') {
                if (pause()) break;
            }

            if (ch == p1_drop_key || ch == toupper(p1_drop_key)) p1.tryDropKey(*this);
            if (ch == p2_drop_key || ch == toupper(p2_drop_key)) p2.tryDropKey(*this);

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
    Switch::updateAllSwitches(*this);
    Door::updateProximityDoors(*this);

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
                    Door::makeDoorHollow(id, getDoors(), screen);
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
    } 
    else 
    {
        statusBar.resetRunTime();
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




