#include "Spring.h"
#include "Game.h"
#include "Player.h"




bool Spring::interact(Game& game, Player& player) 
{
    // 1. Get the direction the player is currently moving
    Direction playerDirVec = player.getPosition().getDir(); 

    // 2. Get the spring's force vector using the stored index
    Direction springReleaseVec = Direction::directions[releaseDirection];

    // 3. Check if player is moving OPPOSITE to the release direction
    // (i.e., pushing into the spring towards the wall)
    bool isPushing = (playerDirVec.getX() == -springReleaseVec.getX()) && 
                     (playerDirVec.getY() == -springReleaseVec.getY());

    if (isPushing) {
        player.compressedSprings++;
    }
    
    // Return true so the player actually moves onto the tile
    return true; 
}