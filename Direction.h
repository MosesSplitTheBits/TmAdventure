#pragma once      


class Direction {
private:
    int dirx;
    int diry;
public:
    Direction(int dx = 0, int dy = 0) : dirx(dx), diry(dy) {}
    int dx() const { return dirx; }   // getter
    int dy() const { return diry; }   // getter

    enum DirectionType {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        STAY, NUM_DIRECTIONS    
    };
    static const Direction directions[NUM_DIRECTIONS];
};