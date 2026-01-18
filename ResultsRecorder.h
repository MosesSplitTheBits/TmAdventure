#pragma once
#include <fstream>
#include "ResultEvent.h"

class ResultsRecorder {
public:
    ResultsRecorder(const std::string& filename);

    void recordScreenChange(int time, int screenId);
    void recordLifeLost(int time);
    void recordRiddle(int time,
                      const std::string& riddle,
                      const std::string& answer,
                      bool correct);
    void recordGameEnd(int time, int score);

    void close();

private:
    std::ofstream file;
};