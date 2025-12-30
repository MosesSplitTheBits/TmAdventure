#include "Game.h"
#include "GameObject.h"
#include "Door.h"
#include "Key.h"
#include "Switch.h"
#include "Obstacle.h"
#include "Riddle.h"
#include "Screen.h"
#include "Spring.h" // Ensure this is included
#include <algorithm>
#include <vector>
#include <memory>

// --- CORE OBJECT MANAGEMENT ---

void Game::addObject(std::unique_ptr<GameObject> obj) {
    if (!obj) return;
    objects.push_back(std::move(obj));
    
    // Ensure grid is initialized
    if (objectGrid.empty()) {
        objectGrid.assign(Screen::MAX_Y + 1, std::vector<GameObject*>(Screen::MAX_X + 1, nullptr));
    }
    
    auto& p = objects.back()->getPosition();
    int x = p.getX(); 
    int y = p.getY();
    
    if (x >= 0 && x <= Screen::MAX_X && y >= 0 && y <= Screen::MAX_Y) {
        objectGrid[y][x] = objects.back().get();
    }
}

void Game::rebuildObjectGrid() {
    objectGrid.assign(Screen::MAX_Y + 1, std::vector<GameObject*>(Screen::MAX_X + 1, nullptr));
    for (auto& u : objects) {
        if (!u) continue;
        auto& p = u->getPosition();
        int x = p.getX(); 
        int y = p.getY();
        
        if (x >= 0 && x <= Screen::MAX_X && y >= 0 && y <= Screen::MAX_Y) {
            objectGrid[y][x] = u.get();
        }
    }
}

void Game::removeObjectAt(int x, int y) {
    if (x < 0 || x > Screen::MAX_X || y < 0 || y > Screen::MAX_Y) return;
    
    GameObject* ptr = objectGrid[y][x];
    if (!ptr) return;
    
    // Remove from vector
    auto it = std::remove_if(objects.begin(), objects.end(), 
        [&](const std::unique_ptr<GameObject>& u){ return u.get() == ptr; });
    
    if (it != objects.end()) {
        objects.erase(it, objects.end());
    }
    
    // Rebuild grid to ensure pointers are valid
    rebuildObjectGrid();
}

GameObject* Game::objectAt(int x, int y) {
    if (x < 0 || x > Screen::MAX_X || y < 0 || y > Screen::MAX_Y) return nullptr;
    if (objectGrid.empty()) return nullptr;
    return objectGrid[y][x];
}

// --- TYPE-SPECIFIC GETTERS ---

std::vector<Door*> Game::getDoors() {
    std::vector<Door*> out;
    for (auto& u : objects) {
        if (!u) continue;
        if (auto d = dynamic_cast<Door*>(u.get())) out.push_back(d);
    }
    return out;
}

std::vector<Key*> Game::getKeys() {
    std::vector<Key*> out;
    for (auto& u : objects) {
        if (!u) continue;
        if (auto k = dynamic_cast<Key*>(u.get())) out.push_back(k);
    }
    return out;
}

std::vector<Switch*> Game::getSwitches() {
    std::vector<Switch*> out;
    for (auto& u : objects) {
        if (!u) continue;
        if (auto s = dynamic_cast<Switch*>(u.get())) out.push_back(s);
    }
    return out;
}

std::vector<Obstacle*> Game::getObstacles() {
    std::vector<Obstacle*> out;
    for (auto& u : objects) {
        if (!u) continue;
        if (auto o = dynamic_cast<Obstacle*>(u.get())) out.push_back(o);
    }
    return out;
}

std::vector<Riddle*> Game::getRiddles() {
    std::vector<Riddle*> out;
    for (auto& u : objects) {
        if (!u) continue;
        if (auto r = dynamic_cast<Riddle*>(u.get())) out.push_back(r);
    }
    return out;
}

std::vector<Torch*> Game::getTorches() {
    std::vector<Torch*> out;
    for (auto& u : objects) {
        if (!u) continue;
        if (auto t = dynamic_cast<Torch*>(u.get())) out.push_back(t);
    }
    return out;
}

std::vector<Bomb*> Game::getBombs() {
    std::vector<Bomb*> out;
    for (auto& u : objects) {
        if (!u) continue;
        if (auto b = dynamic_cast<Bomb*>(u.get())) out.push_back(b);
    }
    return out;
}

//for torches
bool Game::isDarkened() const {
    auto torches = const_cast<Game*>(this)->getTorches();
    return !torches.empty() && !torches[0]->isCollected();
}

int Game::getVisionRadius(int px, int py) const {
    auto p1_has = const_cast<Player&>(p1).hasTorch();
    auto p2_has = const_cast<Player&>(p2).hasTorch();
    return (p1_has || p2_has) ? 10 : 2;
}

std::vector<Spring*> Game::getSprings() {
    std::vector<Spring*> springs;
    for (auto& obj : objects) {
        // Check if the object is actually a Spring
        if (auto s = dynamic_cast<Spring*>(obj.get())) {
            springs.push_back(s);
        }
    }
    return springs;
}