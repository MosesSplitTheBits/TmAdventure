#include "GameObject.h"
#include "Door.h"
#include "Torch.h"
#include "Bomb.h"
#include "Switch.h"
#include "Spring.h"
#include "Obstacle.h"
#include "Riddle.h"
#include "Key.h"
#include "Game.h" 
#include <memory>
#include <vector>
#include "Room.h"
#include "Wall.h"
#include "PushableBlock.h"
#include <iostream>
#include "ObjectFactory.h"

std::unique_ptr<GameObject> ObjectFactory::createFromTile(Game& game, int x, int y, char tile) {

    int room = game.getCurrentRoom()->getID(); 

    // 2. הדפסה כדי לראות אם זה רץ
    // std::cout << "Processing tile: " << tile << " at " << x << "," << y << std::endl;

    switch (tile) 
    {
        case '4':
        {
            // בדיקה אם החדר הבא קיים
            Room* next = game.getCurrentRoom()->getNext();
            if (!next) std::cout << "WARNING: Door 4 has no next room!" << std::endl;
            return std::make_unique<Door>(x, y, room, tile, next);
        }

        case '3':
        {
            // בדיקה אם החדר הקודם קיים
            Room* prev = game.getCurrentRoom()->getPrev();
            if (!prev) std::cout << "WARNING: Door 3 has no prev room!" << std::endl;
            return std::make_unique<Door>(x, y, room, tile, prev, false);
        }
        
        case 'T':
            // Torch only takes (x, y)
            return std::make_unique<Torch>(x, y);

        case 'B':
            // Bomb only takes (x, y)
            return std::make_unique<Bomb>(x, y);

        case 'S':
            // Regular switch (player-activated)
            return std::make_unique<Switch>(x, y, room, false, false);
        
        case 'P':
            // Pressure plate (block-activated)
            return std::make_unique<Switch>(x, y, room, true, false);
        
        case '/':  // Add this case for toggle switches
            return std::make_unique<Switch>(x, y, game.getCurrentRoom()->getID(), false, true);

        case '#':
            // Springs always push RIGHT (direction 3)
            // Player moves LEFT into wall, spring launches them back RIGHT
            return std::make_unique<Spring>(x, y, 3); // 3 = RIGHT

        case '*':
            // Obstacle takes (x, y, id, type) - this one was actually correct
            return std::make_unique<Obstacle>(x, y, room, tile);

        case '?':
            // Riddle takes (x, y, id)
            return std::make_unique<Riddle>(x, y, room);

        case 'K':
            // Key takes (x, y, id)
            return std::make_unique<Key>(x, y, room);

        case 'W':
            return std::make_unique<Wall>(x, y);
        
        case 'X':
            // Create 3x2 PushableBlock (only once for top-left tile)
            if (!game.objectAt(x, y)) {
                return std::make_unique<PushableBlock>(x, y, 3, 2);
            }
            return nullptr; // Skip if already part of another block
        
        
        
            default:
            return nullptr;
    }
}
