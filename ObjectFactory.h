#pragma once
#include <memory>


class GameObject;
class Game;

namespace ObjectFactory
{
    std::unique_ptr<GameObject> createFromTile(Game& game, int x, int y, char tile);
}
