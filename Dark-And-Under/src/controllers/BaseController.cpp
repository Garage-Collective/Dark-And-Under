#include "EnemyController.h"
#include "../../Enums.h"
#include "../levels/Level.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../utils/Utils.h"


BaseController::BaseController() {}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Is the move legal?
 * 
 *  Checks to see if the position X / Y is currently empty.  
 * -----------------------------------------------------------------------------------------------------------------------------
 */
bool BaseController::moveLegal(Enemy *allEnemies, Player *player, Level *level, uint16_t x, uint16_t y) {

  if ((level->getMapElement(x, y, false) == MapElement::Floor) ||
      (player != nullptr && level->getMapElement(x, y, false) == MapElement::UnlockedDoor)) {
       

    // Is there a locked door in the way ?

    for (uint8_t i = 0; i < NUMBER_OF_DOORS; ++i) {
      
      Item door = level->getDoors()[i];

      if (door.getEnabled() && (door.getItemType() == ItemType::LockedDoor || door.getItemType() == ItemType::LockedGate) && door.getX() == x && door.getY() == y)     { return false; }
  
    }


    // Is another enemy in the same position ?

    
    for (uint8_t i = 0; i < NUMBER_OF_ENEMIES; ++i) {

      Enemy enemy = allEnemies[i];

      if (enemy.getEnabled() && enemy.getX() == x && enemy.getY() == y)      { return false; }

    }
    
  
    // Position is already occupied by the player ?

    if (player != nullptr && player->getX() == x && player->getY() == y)      { return false; }

    return true;

  }

  return false;

} 

 
