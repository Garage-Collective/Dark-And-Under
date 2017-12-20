#pragma once

#include <Arduboy2.h>
#include "../../Enums.h"
#include "../levels/Level.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../entities/Item.h"
#include "../levels/MapData.h"

class BaseController {

  public:

    BaseController();

    static boolean moveLegal(Enemy *allEnemies, Player *player, Level *level, uint16_t x, uint16_t y);

};
