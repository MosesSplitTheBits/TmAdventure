#include "InputManager.h"
#include "StepsLoader.h"
#include <conio.h>

constexpr char ESC = 27;

InputManager::InputManager(GameMode mode, StepsLoader* loader)
    : mode(mode), stepsLoader(loader) {}

InputAction InputManager::getAction(int gameTime)
{
    
    if (mode == GameMode::Load) {
        if (stepsLoader && stepsLoader->hasActionAt(gameTime)) {
            return stepsLoader->getAction(gameTime);
        }
        return InputAction{};
    }

    // NORMAL / SAVE MODE: KEYBOARD ONLY
    if (!_kbhit())
        return InputAction{};

    char ch = _getch();
    InputAction action;
    action.rawKey = ch;

    if (ch == 'h' || ch == 'H') {
        action.type = ActionType::Pause;
        return action;
    }

    if (ch == 27) { // ESC
        action.type = ActionType::Quit;
        return action;
    }

    if (ch == 'e' || ch == 'E') {
        action.type = ActionType::DropItem;
        action.playerId = 0;
        return action;
    }

    if (ch == 'o' || ch == 'O') {
        action.type = ActionType::DropItem;
        action.playerId = 1;
        return action;
    }
    // VALID MOVEMENT KEYS ONLY
if (ch == 'w' || ch == 'a' || ch == 's' || ch == 'd' || ch == 'x'   // Player 1
    || ch == 'i' || ch == 'j' || ch == 'k' || ch == 'l' || ch == 'm' )  // Player 2
{
    action.type = ActionType::Move;
    return action;
}


    return action;
}