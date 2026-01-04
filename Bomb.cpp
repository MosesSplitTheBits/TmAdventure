#include "Game.h"
#include "Player.h"
#include "Screen.h"
#include "Bomb.h"
#include "GameObject.h"
#include "Sound.h"


bool Bomb::interact(Game& game, Player& player) {
    if (!isCollected()) {
        if (player.hasItem()) return true; // Can't pick up if already holding something
        collect();
        player.pickBomb();
        game.removeObjectAt(getPosition().getX(), getPosition().getY());
        game.getScreen().setCharAt(getPosition().getX(), getPosition().getY(), ' ');
        player.draw();
    }
    return true;
}


// BOMB HANDLERING
void Bomb::handleBombExplosions(Game& game) {
    auto bombs = game.getBombs();
    
    for (auto bomb : bombs) {
        if (!bomb || bomb->isCollected()) continue;
        
        // Tick planted bombs
        if (bomb->isPlanted()) {
            bomb->tick();
            
            if (bomb->hasExploded()) {
                Sound::BombExplode();
                int bx = bomb->getPosition().getX();
                int by = bomb->getPosition().getY();
                
                // Destroy everything in radius 3
                for (int y = 0; y <= Screen::MAX_Y; ++y) {
                    if (y >= 21) continue; // Preserve HUD
                    for (int x = 0; x <= Screen::MAX_X; ++x) {
                        int dist = std::abs(x - bx) + std::abs(y - by);
                        if (dist <= 3) {
                            GameObject* obj = game.objectAt(x, y);
                            if (obj && obj->typeChar() != 'B') { // Don't remove bomb itself yet
                                game.removeObjectAt(x, y);
                            }
                            
                            game.getScreen().setCharAt(x, y, ' ');
                            game.getScreen().setColorAt(x, y, 7);
                            game.getScreen().drawCell(x, y);
                            
                            // Kill players in range
                            if (game.getp1().getPosition().getX() == x && game.getp1().getPosition().getY() == y) {
                                game.getp1().setPosition(3, 2);
                                game.getp1().draw();
                            }
                            if (game.getp2().getPosition().getX() == x && game.getp2().getPosition().getY() == y) {
                                game.getp2().setPosition(75, 2);
                                game.getp2().draw();
                            }
                        }
                    }
                }
                
                // Now remove the bomb
                game.removeObjectAt(bx, by);
            }
        }
    }
}
