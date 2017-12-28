#pragma once

#include "../Arduboy/Arduboy2.h"
#include "Base.h"
#include "../../Enums.h"

class Enemy : public Base {

  public:

    Enemy();

    EnemyType getEnemyType();
    bool getEnabled();
    uint8_t getHitPoints();
    uint8_t getHitPointsMax();
    uint8_t getAttackPower();
    uint8_t getExperiencePoints();
    bool getMoving();
    
    void setEnemyType(const EnemyType value);
    void setEnabled(const bool value);    
    void setHitPoints(const uint8_t value);   
    void setHitPointsMax(const uint8_t value);   
    void setAttackPower(const uint8_t value);   
    void setMoving(const bool value);    
    void setExperiencePoints(const uint8_t value);  
    
  private:

    // EnemyType _enemyType;
    uint8_t _hitPoints;
    uint8_t _hitPointsMax;
    uint8_t _attackPower;    
    uint8_t _experiencePoints;
    uint8_t _flags;

  };
