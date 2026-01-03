#include "Torch.h"
#include "Game.h"
#include "Player.h"
#include "Sound.h"

bool Torch::interact(Game& game, Player& player) {
    if (!isCollected()) {
        Sound::PickupTorch();
        collect();
        player.pickTorch();
        game.removeObjectAt(getPosition().getX(), getPosition().getY());
        game.getScreen().setCharAt(getPosition().getX(), getPosition().getY(), ' ');
        player.draw();
    }
    return true;
}