#include <Arduboy2.h>
#include "Level.h"
#include "../../Enums.h"
#include "MapData.h"

Level::Level() {  }

uint32_t Level::getStartPos() const                         { return _startPos; }
uint8_t Level::getWidth() const                             { return _width; }
uint8_t Level::getHeight() const                            { return _height; }
const Item * Level::getDoors() const                        { return _doors; }
Item * Level::getDoors()                                    { return _doors; }

const uint8_t * Level::getLevel() const                     { return _level; }

char * Level::getTitleLine1()                               { return _titleLine1; }
const char * Level::getTitleLine1() const                   { return _titleLine1; }
char * Level::getTitleLine2()                               { return _titleLine2; }
const char * Level::getTitleLine2() const                   { return _titleLine2; }

void Level::setDoors(Item *value)                           { _doors = value; }
void Level::setStartPos(const uint32_t value)               { _startPos = value; }
void Level::setWidth(const uint8_t value)                   { _width = value; }
void Level::setHeight(const uint8_t value)                  { _height = value; }

void Level::setLevel(const uint8_t *value)                  { _level = value; }
void Level::setMapTiles(const uint8_t * const *value)       { _map_tiles = value; }
  
MapElement Level::getMapElement(uint8_t x, uint8_t y, bool returnInactive) const {


  // Is there a door in this location?

  for (uint8_t i = 0; i < NUMBER_OF_DOORS; ++i) {

    Item door = _doors[i];

    if ((returnInactive ? true: door.getEnabled()) && door.getX() == x && door.getY() == y) {

      return (MapElement)((uint8_t)door.getItemType());

    }

  }


  // If the coordinates are for the outside bounday of the map then it has to be a wall ..

  if (x == 0 || y == 0 || x == (_width * MAP_TILE_WIDTH) - 1 || y == (_height * MAP_TILE_HEIGHT) - 1)  { return MapElement::Wall; }


  // Otherwise, work it out from the map ..
  
  uint8_t tileNumber = pgm_read_byte(&_level[_startPos + (x / MAP_TILE_WIDTH) + ((y / MAP_TILE_HEIGHT) * _width)]);
  const uint8_t *tile = &tiles[tileNumber * MAP_TILE_WIDTH * 2];
  uint8_t mapElement = pgm_read_byte(&tile[(x % MAP_TILE_WIDTH) + (((y % MAP_TILE_HEIGHT) / 8) * MAP_TILE_PHYSICAL_WIDTH)]) & (1 << (y % MAP_TILE_HEIGHT % 8));
  
  return (mapElement > 0 ? MapElement::Wall : MapElement::Floor);

}
