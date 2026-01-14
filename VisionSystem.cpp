#include "VisionSystem.h"
#include "Room.h"
#include "Player.h"
#include "Point.h"


VisionSystem::Grid VisionSystem::compute(
    const Room& room,
    const Player& p1,
    const Player& p2
) {
    int width  = room.getMapData()[0].size();
    int height = room.getMapData().size();

    Grid grid(height, std::vector<Cell>(width, Cell::Hidden));

    // Rule 1: If the room is not dark, everything is visible
    if (!room.isDark()) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                grid[y][x] = Cell::Visible;
            }
        }
        return grid;
    }

    // ---- Dark room logic ----

    // Player 1
    {
        Point pos = p1.getPosition();
        int radius = (p1.hasTorch()) ? 4 : 1;
        revealCircle(grid, pos.getX(), pos.getY(), radius);
    }

    // Player 2
    {
        Point pos = p2.getPosition();
        int radius = (p2.hasTorch()) ? 4 : 1;
        revealCircle(grid, pos.getX(), pos.getY(), radius);
    }

    return grid;
}

void VisionSystem::revealCircle(Grid& grid, int cx, int cy, int radius)
{
    int height = grid.size();
    int width  = grid[0].size();

    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {

            if (dx * dx + dy * dy > radius * radius)
                continue;

            int x = cx + dx;
            int y = cy + dy;

            if (x < 0 || x >= width || y < 0 || y >= height)
                continue;

            grid[y][x] = Cell::Visible;
        }
    }
}