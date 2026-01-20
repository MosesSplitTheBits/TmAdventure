#pragma once
#include <string>
#include <vector>
#include <functional>

class MapLoader {
public:
    struct MapEntry {
        int roomId;
        std::string path;
    };

    static std::vector<std::string> load(const std::string& path);

    // Reads an ordered list of maps from a text file.
    // Format (one per line):
    //   - map_1.txt
    //   - Maps/map_1.txt
    //   - 10|Maps/map_10.txt   (explicit roomId)
    // Empty lines and lines starting with # or // are ignored.
    // Streaming variant: reads list file line-by-line and calls onEntry for each parsed entry.
    static void forEachMapListEntry(const std::string& listPath,
                                   const std::function<void(const MapEntry&)>& onEntry);
};
