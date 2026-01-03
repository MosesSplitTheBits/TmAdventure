#include "Spring.h"
#include "Game.h"
#include "Player.h"



bool Spring::interact(Game& game, Player& player) 
{
    if(compressed) return true; // Already compressed = invisible
    // Get the direction the player is currently moving
    Direction playerDir = player.getPosition().getDir(); 

    // Get the spring's release direction (always RIGHT = 3)
    Direction springReleaseDir = Direction::directions[releaseDirection];


    // Check if player is moving LEFT (opposite of spring's RIGHT direction)
    // Spring releases RIGHT (1,0), so player must be moving LEFT (-1,0)
    bool isPushing = (playerDir.dx() == -springReleaseDir.dx()) && 
                     (playerDir.dy() == -springReleaseDir.dy());
    

    if (isPushing && !player.springState.active) {
        player.compressedSprings++;
        player.springState.launchDir = springReleaseDir;
        compressed = true; //Mark it as compressed
        // visually remove it immediately
        game.getScreen().setCharAt(
            getPosition().getX(),
            getPosition().getY(),
            ' '
        );
        game.getScreen().drawCell(
            getPosition().getX(),
            getPosition().getY()
        );
    }
    
    return true; 
}
