#pragma once
#include <vector>
#include <string>
#include <set>
#include <utility> // for std::pair
#include "Obstacle.h"
#include "Switch.h"
#include "Key.h"


class Room {
    // Store a pointer to the map data (which is in LevelData.h)
    const std::vector<std::string>* mapData; 
    int roomId;
    Room* next = nullptr;
    Room* prev = nullptr;

    // State storage
    bool visited = false;  // MUST be false initially, not true
    std::vector<Obstacle> obstacles;
    std::vector<Switch> switches;
    std::vector<Key> keysInGame;
    std::set<std::pair<int, int>> solvedRiddles;
    std::vector<int> openDoorIdsSaved; // store opened door ids (unique ids)

public:
    // Constructor: Takes a reference to the vector, stores its address
    Room(const std::vector<std::string>& data, int id) 
        : mapData(&data), roomId(id) {}

    int getID() const { return roomId; }
    
    // Returns the map data in the format Screen::loadMap expects
    const std::vector<std::string>& getMapData() const { return *mapData; }

    void setNext(Room* r) { next = r; }
    void setPrev(Room* r) { prev = r; }
    Room* getNext() const { return next; }
    Room* getPrev() const { return prev; }

    // State management
    bool isVisited() const { return visited; }
    void setVisited(bool v) { visited = v; }

    // Save the current game state into this room
    void saveState(const std::vector<Obstacle>& obsIn,
                   const std::vector<Switch>& swIn,
                   const std::vector<Key>& keysIn,
                   const std::set<std::pair<int,int>>& riddlesIn,
                   const std::vector<int>& openDoorIdsIn)
    {
        obstacles = obsIn;
        switches = swIn;
        keysInGame = keysIn;
        solvedRiddles = riddlesIn;
        openDoorIdsSaved = openDoorIdsIn;
        visited = true;
    }

    // Load the saved state back into the game variables
    void loadState(std::vector<Obstacle>& obsOut,
                   std::vector<Switch>& swOut,
                   std::vector<Key>& keysOut,
                   std::set<std::pair<int,int>>& riddlesOut,
                   std::vector<int>& openDoorIdsOut) const
    {
        obsOut = obstacles;
        swOut = switches;
        keysOut = keysInGame;
        riddlesOut = solvedRiddles;
        openDoorIdsOut = openDoorIdsSaved;
    }
};