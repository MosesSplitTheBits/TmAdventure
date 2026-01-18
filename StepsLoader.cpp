#include "StepsLoader.h"
#include <fstream>
#include <sstream>

StepsLoader::StepsLoader(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);

        if (line.rfind("SEED", 0) == 0) {
            iss.ignore(5);
            iss >> seed;
        }
        else {
            TimedAction a;
            iss >> a.time >> a.key;
            actions.push(a);
        }
    }
}

bool StepsLoader::hasActionAt(int gameTime) const {
    return !actions.empty() && actions.front().time == gameTime;
}

InputAction StepsLoader::getAction(int gameTime)
{
    InputAction action;

    if (hasActionAt(gameTime)) {
        action.rawKey = actions.front().key;
        action.type = ActionType::Move; // reuse existing logic
        actions.pop();
    }

    return action;
}

unsigned int StepsLoader::getRandomSeed() const {
    return seed;
}