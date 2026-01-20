#pragma once
#include <vector>
#include <string>
#include "Puzzle.h"

// Static utility class for loading riddles from file
// Follows MapLoader pattern
class RiddleLoader {
public:
    // Load riddles from file with format: question|answer1|answer2|answer3|answer4|correctIndex
    // Returns vector of Puzzle structs
    // Throws std::runtime_error if file cannot be opened or format is invalid
    static std::vector<Puzzle> load(const std::string& filename);
};
