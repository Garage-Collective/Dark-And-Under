#pragma once
#include "src/levels/MapData.h"

// ----  Memory saving and other decision switches ----------------------------------------------------------------------------------

        // Should a level up include HP
#define LEVEL_UP_INC_HP 
#define LEVEL_UP_SELECT_PRIZE

// ----------------------------------------------------------------------------------------------------------------------------------


#define UP_BUTTON_MASK                    UP_BUTTON
#define DOWN_BUTTON_MASK                  DOWN_BUTTON
#define LEFT_BUTTON_MASK                  LEFT_BUTTON
#define RIGHT_BUTTON_MASK                 RIGHT_BUTTON
#define A_BUTTON_MASK                     A_BUTTON
#define B_BUTTON_MASK                     B_BUTTON

#define SELECT_BUTTON_MASK                A_BUTTON_MASK
#define BACK_BUTTON_MASK                  B_BUTTON_MASK

#define MAP_TILE_WIDTH                    15
#define MAP_TILE_HEIGHT                   15
#define MAP_TILE_PHYSICAL_WIDTH           15
#define MAP_TILE_PHYSICAL_HEIGHT          15

#define MAP_TILE_SIZE                     4
#define MAP_TILE_OFFSET                   5

#define NUMBER_OF_ENEMIES                 30
#define NUMBER_OF_DOORS                   8
#define NUMBER_OF_ITEMS                   NUMBER_OF_ENEMIES

#define MAP_IMAGE_BACK                    0
#define MAP_IMAGE_CLOSE_FRONT             1
#define MAP_IMAGE_CLOSE_GATE_LOCKED       2
#define MAP_IMAGE_CLOSE_DOOR_LOCKED       3
#define MAP_IMAGE_CLOSE_DOOR_UNLOCKED     4
#define MAP_IMAGE_CLOSE_LEFT              5
#define MAP_IMAGE_CLOSE_RIGHT             6
#define MAP_IMAGE_CLOSE_GATE_LEFT         7
#define MAP_IMAGE_CLOSE_GATE_RIGHT        8

#define MAP_IMAGE_MID_FRONT               9
#define MAP_IMAGE_MID_GATE_LOCKED         10
#define MAP_IMAGE_MID_DOOR_LOCKED         11
#define MAP_IMAGE_MID_DOOR_UNLOCKED       12
#define MAP_IMAGE_MID_LEFT                13
#define MAP_IMAGE_MID_RIGHT               14
#define MAP_IMAGE_MID_GATE_LEFT           15
#define MAP_IMAGE_MID_GATE_RIGHT          16

#define MAP_IMAGE_FAR_GATE_LOCKED         17
#define MAP_IMAGE_FAR_DOOR_LOCKED         18
#define MAP_IMAGE_FAR_DOOR_UNLOCKED       19
#define MAP_IMAGE_FAR_LEFT                20
#define MAP_IMAGE_FAR_RIGHT               21

#define FIGHT_DELAY                       1500
#define LEVEL_UP_DELAY                    3000

#define ENEMY_MAX_ATTACK                  4
#define ENEMY_MAX_HITPOINTS               10

#define HUMAN_MAX_ATTACK                  5
#define PLAYER_CASTS_SPELL                5

#define DIRECTION_X_OFFSET                66
#define DIRECTION_Y_OFFSET                43

#define INVENTORY_ACTION_USE              0
#define INVENTORY_ACTION_DROP             1
#define INVENTORY_POSITION_1              Point{11, 8}
#define INVENTORY_POSITION_2              Point{28, 8}
#define INVENTORY_POSITION_3              Point{45, 8}
#define INVENTORY_POSITION_4              Point{28, 35}
#define INVENTORY_POSITION_5              Point{45, 35}
#define INVENTORY_POTION_HP_INC           7

#define ITEM_DELAY                        1500
#define ITEM_ACTION_USE                   0
#define ITEM_ACTION_DROP                  1

#define EEPROM_SEQ_START                  EEPROM_STORAGE_SPACE_START + 98

enum class GameState : uint8_t {
  Splash,   
  About,
  InitGame,
  InitLevel,
  Move,
  InventorySelect,
  InventoryAction,
  ItemSelect,
  ItemIgnore,
  Battle_EnemyAttacks_Init,
  Battle_EnemyAttacks,          
  Battle_EnemyDies,
  Battle_PlayerDecides,
  Battle_PlayerAttacks,
  Battle_PlayerDefends,
  Battle_PlayerCastsSpell,
  Battle_PlayerDies,
  LevelUp,
  NextLevel,
  EndOfGame,
  #ifdef ALTERNATIVE_ENDING 
  InvalidSeq,
  #endif
  #ifdef USE_LARGE_MAP
  DisplayLargeMap
  #endif
};

#ifdef LEVEL_UP_SELECT_PRIZE
enum class LevelUpButtons : uint8_t {
  None,
  AP,
  DF
};
#endif

enum class SplashButtons : uint8_t {
  Play,
  About
};

enum class FightButtons : uint8_t {
  Attack,
  Shield,
  Magic,
  Potion,
  Count
};

enum class Button : uint8_t {
  Up,
  Left,
  Down,
  Right
};

enum class MapElement : uint8_t {
  Floor,
  Wall,
  LockedGate = 100,
  LockedDoor,
  UnlockedDoor,
  SelfLockingDoor
};

enum class EnemyType : uint8_t {
  Occular,
  Skeleton,
  Sparkat,
  Wraith,
  Dragon,
  Rat,
  Slime
};

enum class ItemType : uint8_t {
  None, 
  Key,
  Potion,
  Scroll,
  Shield,
  Sword,
  LockedGate = 100,
  LockedDoor,
  UnlockedDoor,
  SelfLockingDoor
};

enum class Direction : uint8_t {
  North,
  East,
  South,
  West,
  Count
};

inline Direction &operator++( Direction &c ) { 
  
  c = static_cast<Direction>( (static_cast<uint8_t>(c) + 1) & 0x03 );
  return c;

}

inline Direction operator++( Direction &c, int ) {

  Direction result = c;
  ++c;
  return result;

}

inline Direction &operator--( Direction & c ) {

  c = static_cast<Direction>( (static_cast<uint8_t>(c) - 1) & 0x03 );
  return c;

}

inline Direction operator--( Direction & c, int ) {

  Direction result = c;
  --c;
  return result;

}
