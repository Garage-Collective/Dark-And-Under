
#include <WString.h>
#include "Utils.h"

const char OccularName[] PROGMEM = "AN OCCULAR";
const char SkeletonName[] PROGMEM = "A SKELETON";
const char SparkatName[] PROGMEM = "A SPARKAT";
const char WraithName[] PROGMEM = "A WRAITH";
const char DragonName[] PROGMEM = "A DRAGON";
const char RatName[] PROGMEM = "A RAT";
const char SlimeName[] PROGMEM = "A SLIME";

const __FlashStringHelper * EnemyNames[] = {
    FlashString(OccularName),
    FlashString(SkeletonName),
    FlashString(SparkatName),
    FlashString(WraithName),
    FlashString(DragonName),
    FlashString(RatName),
    FlashString(SlimeName),    
};
