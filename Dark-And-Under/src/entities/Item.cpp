#include "Item.h"

Item::Item() : Base() {}

ItemType Item::getItemType() const                { return (ItemType)(_flags & 0x7F); }
bool Item::getEnabled() const                     { return ((_flags & 0x80) > 0); }

void Item::setItemType(const ItemType value)      { _flags = (_flags & 0x80) | ((uint8_t)value); }
void Item::setEnabled(const bool value)           { _flags = (_flags & 0x7F) | (value ? 0x80 : 00); }