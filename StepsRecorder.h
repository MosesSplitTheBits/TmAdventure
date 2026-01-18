#pragma once
#include <fstream>
#include "InputAction.h"

class StepsRecorder {
public:
    StepsRecorder(const std::string& filename);

    void setRandomSeed(unsigned int seed);
    void recordStep(int gameTime, const InputAction& action);
    void close();

private:
    std::ofstream file;
};