#pragma once

#include "../../Enums.h"

class Enemy;
class Player;
class Level;

class BaseController {

  public:

    BaseController();

    static bool moveLegal(Enemy *allEnemies, Player *player, Level *level, uint16_t x, uint16_t y);

};
