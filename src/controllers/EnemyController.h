#pragma once

#include "../Arduboy/Arduboy2.h"
#include "BaseController.h"
#include "../../Enums.h"
#include "../levels/Level.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../entities/Item.h"
#include "../levels/MapData.h"

class EnemyController : public BaseController {

  public:

    EnemyController();

    static void move(Enemy *thisEnemy, Enemy *allEnemies, Player *player, Level *level); 
    static boolean moveTowardsPlayer(Enemy *thisEnemy, Enemy *allEnemies, Player *player, Level *level);
    static boolean moveToVacantSquare(Enemy *thisEnemy, Enemy *allEnemies, Level *level, Direction playerDirection);

};