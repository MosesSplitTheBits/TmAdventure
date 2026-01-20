#include "MapLoader.h"
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <unordered_set>

namespace {
    std::string Trim(const std::string& s) {
        size_t start = 0;
        while (start < s.size() && (s[start] == ' ' || s[start] == '\t')) ++start;
        size_t end = s.size();
        while (end > start && (s[end - 1] == ' ' || s[end - 1] == '\t')) --end;
        return s.substr(start, end - start);
    }

    bool IsAllDigits(const std::string& s) {
        if (s.empty()) return false;
        for (char c : s) {
            if (c < '0' || c > '9') return false;
        }
        return true;
    }

    std::string UnderscoreMapFilenameIfNeeded(const std::string& filename) {
        // Converts map1.txt -> map_1.txt, map12.txt -> map_12.txt
        // Leaves other filenames unchanged.
        const std::string prefix = "map";
        const std::string suffix = ".txt";
        if (filename.size() <= prefix.size() + suffix.size()) return filename;
        if (filename.rfind(prefix, 0) != 0) return filename;
        if (filename.substr(filename.size() - suffix.size()) != suffix) return filename;

        // ensure middle is digits
        const std::string middle = filename.substr(prefix.size(), filename.size() - prefix.size() - suffix.size());
        if (middle.empty()) return filename;
        for (char c : middle) {
            if (c < '0' || c > '9') return filename;
        }

        return prefix + "_" + middle + suffix;
    }

    std::ifstream TryOpen(const std::filesystem::path& p) {
        std::ifstream file(p);
        return file;
    }

    std::ifstream OpenWithCandidatesOrThrow(std::filesystem::path& p, const std::string& originalPath, bool allowUnderscoreMapVariant) {
        // Try a few likely candidates (relative to CWD) before failing.
        std::ifstream file = TryOpen(p);
        if (file) return file;

        std::vector<std::filesystem::path> candidates;
        const auto cwd = std::filesystem::current_path();

        // 1) CWD + given path
        if (p.is_relative()) {
            candidates.push_back(cwd / p);
        }

        // 2) Fix folder casing: maps/ -> Maps/
        if (p.has_parent_path()) {
            auto parent = p.parent_path();
            if (parent == "maps") {
                candidates.push_back(std::filesystem::path("Maps") / p.filename());
            }
        }

        // 3) map1.txt -> map_1.txt variants (only for actual map files)
        const std::string filename = p.filename().string();
        const std::string underscored = allowUnderscoreMapVariant ? UnderscoreMapFilenameIfNeeded(filename) : filename;
        if (underscored != filename) {
            candidates.push_back(p.parent_path() / underscored);
            candidates.push_back(std::filesystem::path("Maps") / underscored);
            if (p.is_relative()) {
                candidates.push_back(cwd / (p.parent_path() / underscored));
                candidates.push_back(cwd / (std::filesystem::path("Maps") / underscored));
            }
        }

        // 4) If only filename provided, try under Maps/
        if (p.parent_path().empty()) {
            candidates.push_back(std::filesystem::path("Maps") / p);
            if (p.is_relative()) {
                candidates.push_back(cwd / (std::filesystem::path("Maps") / p));
            }
        }

        // 5) Try one level up (common when exe runs from build dir)
        if (p.is_relative()) {
            candidates.push_back(cwd.parent_path() / p);
            candidates.push_back(cwd.parent_path() / (std::filesystem::path("Maps") / p.filename()));
            if (underscored != filename) {
                candidates.push_back(cwd.parent_path() / (p.parent_path() / underscored));
                candidates.push_back(cwd.parent_path() / (std::filesystem::path("Maps") / underscored));
            }
        }

        for (const auto& cand : candidates) {
            file = TryOpen(cand);
            if (file) {
                p = cand;
                return file;
            }
        }

        throw std::runtime_error("Could not open map file: " + originalPath);
    }
}

std::vector<std::string> MapLoader::load(const std::string& path) {
    std::filesystem::path p(path);

    std::ifstream file = OpenWithCandidatesOrThrow(p, path, /*allowUnderscoreMapVariant=*/true);

    std::vector<std::string> map;
    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back(); // CRLF fix


        map.push_back(line);
    }

    if (map.empty()) {
        throw std::runtime_error("Map file is empty: " + path);
    }

    // validate: כל השורות אותו אורך
    const size_t w = map.front().size();
    for (size_t i = 0; i < map.size(); ++i) {
        if (map[i].size() != w) {
            throw std::runtime_error("Map '" + path + "' has inconsistent row width at row " + std::to_string(i));
        }
    }

    return map;
}

void MapLoader::forEachMapListEntry(const std::string& listPath,
                                   const std::function<void(const MapEntry&)>& onEntry) {
    std::filesystem::path p(listPath);
    std::ifstream file = OpenWithCandidatesOrThrow(p, listPath, /*allowUnderscoreMapVariant=*/false);

    std::unordered_set<int> usedRoomIds;
    std::string line;
    int nextAutoId = 1;
    bool any = false;

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        const std::string trimmed = Trim(line);
        if (trimmed.empty()) continue;
        if (trimmed.rfind("#", 0) == 0) continue;
        if (trimmed.rfind("//", 0) == 0) continue;

        int roomId = -1;
        std::string mapPath;

        const size_t bar = trimmed.find('|');
        if (bar != std::string::npos) {
            const std::string left = Trim(trimmed.substr(0, bar));
            const std::string right = Trim(trimmed.substr(bar + 1));
            if (IsAllDigits(left)) {
                roomId = std::stoi(left);
                mapPath = right;
            } else {
                mapPath = trimmed;
            }
        } else {
            mapPath = trimmed;
        }

        if (mapPath.empty()) {
            throw std::runtime_error("Map list contains an empty path: " + listPath);
        }

        if (roomId < 0) {
            roomId = nextAutoId++;
        }

        if (!usedRoomIds.insert(roomId).second) {
            throw std::runtime_error("Duplicate roomId " + std::to_string(roomId) + " in map list: " + listPath);
        }

        any = true;
        onEntry(MapEntry{ roomId, mapPath });
    }

    if (!any) {
        throw std::runtime_error("Map list is empty: " + listPath);
    }
}
