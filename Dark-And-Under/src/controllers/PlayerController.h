#pragma once

#include <Arduboy2.h>
#include "BaseController.h"
#include "../../Enums.h"
#include "../levels/Level.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../entities/Item.h"
#include "../levels/MapData.h"

class PlayerController : public BaseController {

  public:

    PlayerController();

    static bool move(Player *player, Enemy *allEnemies, Level *level, Button button);

};
