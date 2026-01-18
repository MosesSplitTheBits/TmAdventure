#pragma once
#include "InputAction.h"
#include "GameMode.h"


class StepsLoader; // Forward declaration

class InputManager {
public:
    InputManager(GameMode mode, StepsLoader* loader = nullptr);
    InputAction getAction(int gameTime);

private:
    GameMode mode;
    StepsLoader* stepsLoader;
};