#include "Key.h"
#include "Game.h"   
#include "Player.h"
#include "Screen.h"
#include "Sound.h"


bool Key::interact(Game& game, Player& player) 
{
    if (!isCollected()) {
        Sound::PickupKey();
        collect();
        player.pickKey(getId());
        game.removeObjectAt(getPosition().getX(), getPosition().getY());
        game.getScreen().setCharAt(getPosition().getX(), getPosition().getY(), ' ');
        player.draw();
    }
    return true;
}

