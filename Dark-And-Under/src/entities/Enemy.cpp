#include "Enemy.h"
#include "../../Enums.h"

Enemy::Enemy() : Base() { }

EnemyType Enemy::getEnemyType()                           { return (EnemyType)(_flags & 0x3F); }
uint8_t Enemy::getHitPoints()                             { return _hitPoints; }
uint8_t Enemy::getHitPointsMax()                          { return _hitPointsMax; }
uint8_t Enemy::getAttackPower()                           { return _attackPower; }
uint8_t Enemy::getExperiencePoints()                      { return _experiencePoints; }
bool Enemy::getEnabled()                                  { return ((_flags & 0x80) > 0); }
bool Enemy::getMoving()                                   { return ((_flags & 0x40) > 0); }

void Enemy::setEnemyType(const EnemyType value)           { _flags = (_flags & 0xC0) | ((uint8_t)value); }
void Enemy::setHitPoints(const uint8_t value)             { _hitPoints = value; }
void Enemy::setHitPointsMax(const uint8_t value)          { _hitPointsMax = value; }
void Enemy::setAttackPower(const uint8_t value)           { _attackPower = value; }
void Enemy::setExperiencePoints(const uint8_t value)      { _experiencePoints = value; }
void Enemy::setEnabled(const bool value)                  { _flags = (_flags & 0x7F) | (value ? 0x80 : 00); }
void Enemy::setMoving(const bool value)                   { _flags = (_flags & 0xBF) | (value ? 0x40 : 00); }

