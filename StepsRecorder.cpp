#include "StepsRecorder.h"

StepsRecorder::StepsRecorder(const std::string& filename) {
    file.open(filename);
}

void StepsRecorder::setRandomSeed(unsigned int seed) {
    file << "SEED " << seed << "\n";
}

void StepsRecorder::recordStep(int gameTime, const InputAction& action) {
    if (action.type == ActionType::None)
        return;

    file << gameTime << " " << action.rawKey << "\n";
    file.flush();
}

void StepsRecorder::close() {
    file.close();
}