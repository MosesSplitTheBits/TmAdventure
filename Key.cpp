#include "Key.h"
#include "Game.h"   
#include "Player.h"
#include "Screen.h"
#include "Sound.h"


bool Key::interact(Game& game, Player& player) 
{
    if (!isCollected()) {
        if (player.hasItem()) return true; // Can't pick up if already holding something
        Sound::PickupKey();
        collect();
        player.pickKey(getId());
        game.removeObjectAt(getPosition().getX(), getPosition().getY());
        game.getScreen().setCharAt(getPosition().getX(), getPosition().getY(), ' ');
        player.draw();
    }
    return true;
}

