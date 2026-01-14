#pragma once
#include <vector>

class Room;
class Player;

class VisionSystem {
public:
    enum class Cell {
        Hidden,
        Visible
    };

    using Grid = std::vector<std::vector<Cell>>;

    Grid compute(
        const Room& room,
        const Player& p1,
        const Player& p2
    );

private:
    void revealCircle(Grid& grid, int cx, int cy, int radius);
};