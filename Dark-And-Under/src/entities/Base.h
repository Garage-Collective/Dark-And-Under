#pragma once

#include <stdint.h>

class Base {

  public:

    Base();

    uint8_t getX() const;
    uint8_t getY() const;
    
    void setX(const uint8_t value);
    void setY(const uint8_t value);

  private:

    uint8_t _x;
    uint8_t _y;
    
};

