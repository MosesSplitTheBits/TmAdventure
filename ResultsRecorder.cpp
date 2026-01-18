#include "ResultsRecorder.h"

ResultsRecorder::ResultsRecorder(const std::string& filename) {
    file.open(filename);
}

void ResultsRecorder::recordScreenChange(int time, int screenId) {
    file << time << " SCREEN " << screenId << "\n";
    file.flush();
}

void ResultsRecorder::recordLifeLost(int time) {
    file << time << " LIFE_LOST\n";
    file.flush();
}

void ResultsRecorder::recordRiddle(int time,
                                   const std::string& riddle,
                                   const std::string& answer,
                                   bool correct) {
    file << time << " RIDDLE |"
         << riddle << "|"
         << answer << "|"
         << (correct ? "CORRECT" : "WRONG") << "\n";
    file.flush();
}

void ResultsRecorder::recordGameEnd(int time, int score) {
    file << time << " GAME_END " << score << "\n";
    file.flush();
}

void ResultsRecorder::close() {
    if (file.is_open())
        file.close();
}