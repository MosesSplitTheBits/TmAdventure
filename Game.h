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
#include "StatusBar.h"
#include "Torch.h"
#include "Bomb.h"
#include "Spring.h" 

class Game {
private:
    Screen& screen;
    Player& p1;
    Player& p2;
    Door* lastUsedDoor = nullptr;
    
    // The main list of objects in the current room
    std::vector<std::unique_ptr<GameObject>> objects;
    
    // Fast lookup grid
    std::vector<std::vector<GameObject*>> objectGrid;
    
    // Storage for saving room states when leaving
    std::map<Room*, std::vector<std::unique_ptr<GameObject>>> savedRoomObjects;

    Room* currentRoom;
    PuzzleManager puzzles;
    
    StatusBar statusBar;

public:
    Game(Screen& s, Player& p1, Player& p2, Room* startRoom);
    void run();
    bool handleEvents();
    bool pause();
    bool isGameWon() const;
    void loadLevel(Room* nextRoom, bool comingBack = false);
    void spawnPlayers(bool comingBack);
    


    // Object Management
    void addObject(std::unique_ptr<GameObject> obj);
    void removeObjectAt(int x, int y);
    GameObject* objectAt(int x, int y);
    void rebuildObjectGrid();

    // Helpers
    std::vector<Door*> getDoors();
    std::vector<Key*> getKeys();
    std::vector<Switch*> getSwitches();
    std::vector<Obstacle*> getObstacles();
    std::vector<Riddle*> getRiddles(); 
    std::vector<Torch*> getTorches();
    std::vector<Bomb*> getBombs();
    std::vector<Spring*> getSprings();

    // Getters for Interactions
    PuzzleManager& getPuzzles() { return puzzles; }
    Screen& getScreen() { return screen; }
    Player& getp1() { return p1; }
    Player& getp2() { return p2; }
    Door* getLastUsedDoor() { return lastUsedDoor; }
    StatusBar& getStatusBar() { return statusBar; }
    Room* getCurrentRoom() const { return currentRoom; }

    //setter for lastUsedDoor
    void setLastUsedDoor(Door* door) { lastUsedDoor = door; }
    // Game Logic    
    
    // Generic Interaction Handler
    bool handlePlayerInteraction(Player& player);
    void checkKeyPickup(Player& player); 
    void handleBombExplosions();

    //For Torches
    bool isDarkened() const;
    int getVisionRadius(int px, int py) const;
    void refreshVision();

};

// --- IMPORTANT: This must be OUTSIDE the class ---
void startGame();
