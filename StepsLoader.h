#pragma once
#include <queue>
#include <string>
#include "InputAction.h"

struct TimedAction {
    int time;
    char key;
};

class StepsLoader {
public:
    StepsLoader(const std::string& filename);

    bool hasActionAt(int gameTime) const;
    InputAction getAction(int gameTime);

    unsigned int getRandomSeed() const;

private:
    std::queue<TimedAction> actions;
    unsigned int seed = 0;
};