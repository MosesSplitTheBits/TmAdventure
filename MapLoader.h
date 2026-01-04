#pragma once
#include <string>
#include <vector>

class MapLoader {
public:
    static std::vector<std::string> load(const std::string& path);
};
