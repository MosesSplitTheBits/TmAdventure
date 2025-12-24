#pragma once
#include <vector>
#include <string>
#include <memory>
#include <map> // Required for savedRoomObjects
#include "Screen.h"
#include "Player.h"
#include "LevelData.h"
#include "Room.h"
#include "GameObject.h"
#include "Door.h"
#include "Key.h"
#include "Switch.h"
#include "Obstacle.h"
#include "Puzzle.h"
#include "Riddle.h" 

class Game {
private:
    Screen& screen;
    Player& p1;
    Player& p2;
    
    // The main list of objects in the current room
    std::vector<std::unique_ptr<GameObject>> objects;
    
    // Fast lookup grid
    std::vector<std::vector<GameObject*>> objectGrid;
    
    // Storage for saving room states when leaving
    std::map<Room*, std::vector<std::unique_ptr<GameObject>>> savedRoomObjects;

    Room* currentRoom;
    PuzzleManager puzzles;

public:
    Game(Screen& s, Player& p1, Player& p2, Room* startRoom);
    void run();
    bool handleEvents();
    bool pause();
    bool isGameWon() const;
    void loadLevel(Room* nextRoom, bool comingBack = false);

    // Object Management
    void addObject(std::unique_ptr<GameObject> obj);
    void removeObjectAt(int x, int y);
    GameObject* objectAt(int x, int y);
    void rebuildObjectGrid();

    // Initialization (Implemented in Init.cpp)
    void initSwitches();
    void initObstacles();
    void initDoors();
    void initRiddles();

    // Helpers
    std::vector<Door*> getDoors();
    std::vector<Key*> getKeys();
    std::vector<Switch*> getSwitches();
    std::vector<Obstacle*> getObstacles();
    std::vector<Riddle*> getRiddles(); 

    // Getters for Interactions
    PuzzleManager& getPuzzles() { return puzzles; }
    Screen& getScreen() { return screen; }

    // Game Logic
    void updateProximityDoors();
    bool withinRadius2(const Point& a, const Point& b);
    void animateDoorOpening(int id);
    void makeDoorAreaHollow(int id);
    
    
    // Generic Interaction Handler
    bool handlePlayerInteraction(Player& player);

    void handleDrop(Player& player);
    void checkKeyPickup(Player& player); 
    void updateSwitches();
};

// --- IMPORTANT: This must be OUTSIDE the class ---
void startGame();
