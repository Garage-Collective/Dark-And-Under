#include "Base.h"

Base::Base() {}

uint8_t Base::getX() const                      { return _x; }
uint8_t Base::getY() const                      { return _y; }

void Base::setX(const uint8_t value)            { _x = value; }
void Base::setY(const uint8_t value)            { _y = value; }

