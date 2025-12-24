#include "Riddle.h"

std::vector<std::unique_ptr<Riddle>> Riddle::createFromMap(const std::vector<std::string>& mapData, int roomId) {
    std::vector<std::unique_ptr<Riddle>> riddles;
    for (int y = 0; y < (int)mapData.size(); ++y) {
        for (int x = 0; x < (int)mapData[y].size(); ++x) {
            if (mapData[y][x] == '?') {
                riddles.push_back(std::make_unique<Riddle>(x, y, roomId));
            }
        }
    }
    return riddles;
}