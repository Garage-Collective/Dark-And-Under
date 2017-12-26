#pragma once

#include "../Arduboy/Arduboy2.h"

#define DEFAULT_VERT_SPACING 12

class Arduboy2Ext : public Arduboy2Base {

   public:
     Arduboy2Ext();

     uint8_t justPressedButtons() const;

};

