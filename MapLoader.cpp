#include "MapLoader.h"
#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace {
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
}

std::vector<std::string> MapLoader::load(const std::string& path) {
    std::filesystem::path p(path);

    // Try a few likely candidates (relative to CWD) before failing.
    std::ifstream file = TryOpen(p);
    if (!file) {
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

        // 3) map1.txt -> map_1.txt variants
        const std::string filename = p.filename().string();
        const std::string underscored = UnderscoreMapFilenameIfNeeded(filename);
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
                break;
            }
        }

        if (!file) {
            throw std::runtime_error("Could not open map file: " + path);
        }
    }

    std::vector<std::string> map;
    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back(); // CRLF fix

        // אם אתה רוצה לאפשר שורות ריקות - אל תדלג
        // כאן: מדלגים על שורה ריקה
        if (line.empty()) continue;

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
