#pragma once

#include "Base.h"
#include "../../Enums.h"

class Player : public Base {

  public:

    Player();

    uint8_t getHitPoints();
    uint8_t getDefence();
    uint8_t getAttackPower();
    uint8_t getExperiencePoints();

    Direction getDirection();
    ItemType getInventory(uint8_t slot);
    uint8_t getConsumableSlot();
    uint8_t getSlotNumber(const ItemType item);
    uint8_t getInventoryCount(const ItemType item);
    
    void setHitPoints(const uint8_t value);   
    void setDefence(const uint8_t value);   
    void setAttackPower(const uint8_t value);   
    void setExperiencePoints(const uint8_t value);   
    
    void setInventory(const int8_t slot, const ItemType item);   
    void setDirection(const Direction value);
     
  private:

    void shuffleInventory();

    Direction _direction;
    uint8_t _hitPoints;
    uint8_t _defence;
    uint8_t _attackPower;
    uint8_t _experiencePoints;
    ItemType _inventory[5];
    
};
