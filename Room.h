#pragma once
#include <vector>
#include <string>
#include <set>
#include <utility> // for std::pair
#include "Obstacle.h"
#include "Switch.h"
#include "Key.h"


class Room {
    std::vector<std::string> mapData;   // <-- רק זה, בלי מצביעים ובלי m_map
    int roomId;
    Room* next = nullptr;
    Room* prev = nullptr;
    bool dark = false;

    bool visited = false;
    std::vector<Obstacle> obstacles;
    std::vector<Switch> switches;
    std::vector<Key> keysInGame;
    std::set<std::pair<int, int>> solvedRiddles;
    std::vector<int> openDoorIdsSaved;

public:
    Room(std::vector<std::string> data, int id)
        : mapData(std::move(data)), roomId(id) {}

    int getID() const { return roomId; }

    const std::vector<std::string>& getMapData() const { return mapData; }

    void setNext(Room* r) { next = r; }
    void setPrev(Room* r) { prev = r; }
    Room* getNext() const { return next; }
    Room* getPrev() const { return prev; }

    bool isVisited() const { return visited; }
    void setVisited(bool v) { visited = v; }

    bool isDark() const { return dark; }
    void setDark(bool d) { dark = d; }

    // saveState/loadState כמו שהיה אצלך
};
