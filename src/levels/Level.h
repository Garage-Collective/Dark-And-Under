#pragma once

#include "../Arduboy/Arduboy2.h"
#include "../../Enums.h"
#include "MapData.h"
#include "../entities/Item.h"

class Level {

  public:

    Level();
    MapElement getMapElement(uint8_t x, uint8_t y, bool returnInactive) const;

    uint32_t getStartPos() const;
    uint8_t getWidth() const;
    uint8_t getHeight() const;

    Item * getDoors();
    const Item * getDoors() const;
    const uint8_t * getLevel() const;

    char * getTitleLine1();
    const char * getTitleLine1() const;
    char * getTitleLine2();
    const char * getTitleLine2() const;
    
    void setDoors(Item *value);
    void setStartPos(const uint32_t value);
    void setWidth(const uint8_t value);
    void setHeight(const uint8_t value);

    void setLevel(const uint8_t *value);
    void setMapTiles(const uint8_t * const *value);
    
  private:
    
    uint32_t _startPos;
    uint8_t _width;
    uint8_t _height;

    char _titleLine1[12];
    char _titleLine2[12];
    
    Item * _doors;
    const uint8_t * _level;
    const uint8_t * const * _map_tiles;

};
