#include "PlayerController.h"
#include <Arduboy2.h>
#include "../../Enums.h"
#include "../levels/Level.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"


PlayerController::PlayerController() : BaseController() {}
    
bool PlayerController::move(Player *player, Enemy *allEnemies, Level *level, Button button) {

  if ((button == Button::Up) || (button == Button::Down)) {

    int8_t deltaX = 0;
    int8_t deltaY = 0;
    
    if (button == Button::Up) {
  
      switch (player->getDirection()) {
        
        case Direction::North:      deltaY = -1;    break;
        case Direction::East:       deltaX = 1;     break;
        case Direction::South:      deltaY = 1;     break;
        case Direction::West:       deltaX = -1;    break;
        
      }

    }

    if (button == Button::Down) {
      
      switch (player->getDirection()) {
        
        case Direction::North:      deltaY = 1;     break;
        case Direction::East:       deltaX = -1;    break;
        case Direction::South:      deltaY = -1;    break;
        case Direction::West:       deltaX = 1;     break;
        
      }

    }

    if (PlayerController::moveLegal(allEnemies, player, level, player->getX() + deltaX, player->getY() + deltaY))    { 
    
      player->setX(player->getX() + deltaX); 
      player->setY(player->getY() + deltaY); 
      return true; 
    
    };

  }
  else {

    Direction curDirection = player->getDirection();

    switch (button) {

      case Button::Left:   --curDirection;                      break;
      case Button::Right:  ++curDirection;                      break;
      default:                                                  break;
      
    }

    player->setDirection(curDirection);

  }

  return false;

}
