#pragma once

enum class ActionType {
    None,
    Pause,
    Quit,
    DropItem,
    Move
};

struct InputAction {
    ActionType type = ActionType::None;
    int playerId = -1;   // 0 = p1, 1 = p2
    char rawKey = 0;     // original key 
};