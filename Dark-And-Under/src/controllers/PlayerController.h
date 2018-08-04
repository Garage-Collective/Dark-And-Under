#pragma once

#include "BaseController.h"
#include "../../Enums.h"

class Level;
class Enemy;
class Player;

class PlayerController : public BaseController {

  public:

    PlayerController();

    static bool move(Player *player, Enemy *allEnemies, Level *level, Button button);

};
