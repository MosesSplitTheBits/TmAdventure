#include "Switch.h"

// ...existing code...

std::vector<std::unique_ptr<Switch>> Switch::createFromMap(const std::vector<std::string>& mapData, int roomId) {
    std::vector<std::unique_ptr<Switch>> switches;
    for (int y = 0; y < (int)mapData.size(); ++y) {
        for (int x = 0; x < (int)mapData[y].size(); ++x) {
            if (mapData[y][x] == 'S') {
                switches.push_back(std::make_unique<Switch>(x, y, roomId));
            }
        }
    }
    return switches;
}