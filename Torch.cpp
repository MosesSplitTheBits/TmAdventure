#include "Torch.h"
#include "Game.h"
#include "Player.h"
#include "Sound.h"

bool Torch::interact(Game& game, Player& player) {
    if (!isCollected()) {
        if (player.hasItem()) return true; // Can't pick up if already holding something
        Sound::PickupTorch();
        collect();
        player.pickTorch();
        game.removeObjectAt(getPosition().getX(), getPosition().getY());
    }
    return true;
}