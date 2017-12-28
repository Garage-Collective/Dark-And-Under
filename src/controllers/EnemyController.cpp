#include "../Arduboy/Arduboy2.h"
#include "EnemyController.h"
#include "../../Enums.h"
#include "../levels/Level.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../utils/Utils.h"


EnemyController::EnemyController() : BaseController() {}


void EnemyController::move(Enemy *thisEnemy, Enemy *allEnemies, Player *player, Level *level) {
    
  if (!EnemyController::moveTowardsPlayer(thisEnemy, allEnemies, player, level)) {

    for (int i = 0; i < 20; i++) {

      if (EnemyController::moveToVacantSquare(thisEnemy, allEnemies, level, (Direction)random((uint8_t)Direction::North, (uint8_t)Direction::Count))) { break; }

    }

  }

}
    
bool EnemyController::moveTowardsPlayer(Enemy *thisEnemy, Enemy *allEnemies, Player *player, Level *level) {


  // If the enemy is only one cell away from the player, do not move ..

  int16_t deltaX = player->getX() - thisEnemy->getX();
  int16_t deltaY = player->getY() - thisEnemy->getY();
  
  if ((deltaY == 0 && absT(deltaX) == 1) ^ (deltaX == 0 && absT(deltaY) == 1)) { return true; } 


  // Otherwise attempt to move close to the player ..

  else if (player->getY() < thisEnemy->getY() && EnemyController::moveLegal(allEnemies, nullptr, level, thisEnemy->getX(), thisEnemy->getY() - 1))          { thisEnemy->setY(thisEnemy->getY() - 1); } 
  else if (player->getY() > thisEnemy->getY() && EnemyController::moveLegal(allEnemies, nullptr, level, thisEnemy->getX(), thisEnemy->getY() + 1))          { thisEnemy->setY(thisEnemy->getY() + 1); } 
  else if (player->getX() > thisEnemy->getX() && EnemyController::moveLegal(allEnemies, nullptr, level, thisEnemy->getX() + 1, thisEnemy->getY()))          { thisEnemy->setX(thisEnemy->getX() + 1); }  
  else if (player->getX() < thisEnemy->getX() && EnemyController::moveLegal(allEnemies, nullptr, level, thisEnemy->getX() - 1, thisEnemy->getY()))          { thisEnemy->setX(thisEnemy->getX() - 1); } 
  else {
    
    return false;

  }

  return true;

}
  
bool EnemyController::moveToVacantSquare(Enemy *thisEnemy, Enemy *allEnemies, Level *level, Direction playerDirection) {
  
  if (playerDirection == Direction::North && EnemyController::moveLegal(allEnemies, nullptr, level, thisEnemy->getX(), thisEnemy->getY() - 1))              { thisEnemy->setY(thisEnemy->getY() - 1); } 
  else if (playerDirection == Direction::South && EnemyController::moveLegal(allEnemies, nullptr, level, thisEnemy->getX(), thisEnemy->getY() + 1))         { thisEnemy->setY(thisEnemy->getY() + 1); } 
  else if (playerDirection == Direction::East && EnemyController::moveLegal(allEnemies, nullptr, level, thisEnemy->getX() + 1, thisEnemy->getY()))          { thisEnemy->setX(thisEnemy->getX() + 1); } 
  else if (playerDirection == Direction::West && EnemyController::moveLegal(allEnemies, nullptr, level, thisEnemy->getX() - 1, thisEnemy->getY()))          { thisEnemy->setX(thisEnemy->getX() - 1); } 
  else { return false; }  

  return true;
  
}
