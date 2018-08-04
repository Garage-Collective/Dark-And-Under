#pragma once

#include "BaseController.h"
#include "../../Enums.h"

class Enemy;
class Player;
class Level;

class EnemyController : public BaseController {

  public:

    EnemyController();

    static void move(Enemy *thisEnemy, Enemy *allEnemies, Player *player, Level *level); 
    static bool moveTowardsPlayer(Enemy *thisEnemy, Enemy *allEnemies, Player *player, Level *level);
    static bool moveToVacantSquare(Enemy *thisEnemy, Enemy *allEnemies, Level *level, Direction playerDirection);

};