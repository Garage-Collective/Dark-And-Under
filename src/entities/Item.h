#pragma once

#include "../ArduboyExt/Arduboy2.h"
#include "Base.h"
#include "../../Enums.h"

class Item : public Base {

  public:

    Item();

    ItemType getItemType() const;
    bool getEnabled() const;

    void setItemType(const ItemType value);
    void setEnabled(const bool value);

  private:
    
    uint8_t _flags;
    
};
