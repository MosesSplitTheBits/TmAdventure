#include "Torch.h"
#include "Game.h"
#include "Player.h"

bool Torch::interact(Game& game, Player& player) {
    if (!isCollected()) {
        collect();
        player.pickTorch();
        game.removeObjectAt(getPosition().getX(), getPosition().getY());
        game.getScreen().setCharAt(getPosition().getX(), getPosition().getY(), ' ');
        player.draw();
    }
    return true;
}