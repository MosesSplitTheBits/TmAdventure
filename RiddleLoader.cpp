#include "RiddleLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<Puzzle> RiddleLoader::load(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("RiddleLoader: Could not open file: " + filename);
    }
    
    std::vector<Puzzle> riddles;
    std::string line;
    int lineNumber = 0;
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        // Parse pipe-delimited format: roomId|question|ans1|ans2|ans3|ans4|correctIndex
        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string token;
        
        while (std::getline(ss, token, '|')) {
            tokens.push_back(token);
        }
        
        // Validate format: must have exactly 7 tokens
        if (tokens.size() != 7) {
            throw std::runtime_error(
                "RiddleLoader: Invalid format at line " + std::to_string(lineNumber) + 
                ". Expected 7 tokens (roomId|question|ans1|ans2|ans3|ans4|index), got " + 
                std::to_string(tokens.size())
            );
        }
        
        // Create puzzle
        Puzzle puzzle;
        
        // Parse roomId
        try {
            puzzle.roomId = std::stoi(tokens[0]);
        } catch (const std::exception&) {
            throw std::runtime_error(
                "RiddleLoader: Invalid roomId at line " + std::to_string(lineNumber) + 
                ". Must be a number."
            );
        }
        
        puzzle.question = tokens[1];
        puzzle.answers[0] = tokens[2];
        puzzle.answers[1] = tokens[3];
        puzzle.answers[2] = tokens[4];
        puzzle.answers[3] = tokens[5];
        
        // Parse correctIndex
        try {
            puzzle.correctIndex = std::stoi(tokens[6]);
        } catch (const std::exception&) {
            throw std::runtime_error(
                "RiddleLoader: Invalid correctIndex at line " + std::to_string(lineNumber) + 
                ". Must be a number."
            );
        }
        
        // Validate correctIndex range (0-3)
        if (puzzle.correctIndex < 0 || puzzle.correctIndex > 3) {
            throw std::runtime_error(
                "RiddleLoader: correctIndex out of range at line " + std::to_string(lineNumber) + 
                ". Must be 0-3, got " + std::to_string(puzzle.correctIndex)
            );
        }
        
        riddles.push_back(puzzle);
    }
    
    file.close();
    
    // Ensure at least one riddle was loaded
    if (riddles.empty()) {
        throw std::runtime_error("RiddleLoader: No riddles found in file: " + filename);
    }
    
    return riddles;
}
