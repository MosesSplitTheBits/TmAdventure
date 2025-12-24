#pragma once
#include "GameObject.h"
#include <vector>
#include <string>
#include <memory>

class Riddle : public GameObject {
private:
    bool solved = false;

public:
    // Constructor: x, y, puzzleId (mapped to room ID usually)
    Riddle(int x, int y, int id) 
        : GameObject(x, y, '?', id), solved(false) {}

    virtual char typeChar() const override { return '?'; }
    virtual bool interact(Game& game, Player& player) override;

    bool isSolved() const { return solved; }
    void setSolved(bool s) { solved = s; }
    virtual bool isPassable() const override { return true; }

    static std::vector<std::unique_ptr<Riddle>> createFromMap(const std::vector<std::string>& mapData, int roomId);
};