#pragma once
#include <string>

enum class ResultType {
    ScreenChange,
    LifeLost,
    RiddleAnswered,
    GameEnd
};

struct ResultEvent {
    int time;
    ResultType type;

    // optional payload
    int screenId = -1;
    std::string riddle;
    std::string answer;
    bool correct = false;
    int score = 0;
};