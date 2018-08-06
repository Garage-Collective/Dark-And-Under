#include "src/Arduboy/Arduboy2.h"

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Initialise Game Play.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void initialiseGame() {

  #ifdef ALTERNATIVE_ENDING

    if (ALTERNATIVE_ENDING_SEQ != 1) {

      byte game = EEPROM.read(EEPROM_SEQ_START);
      byte seq = EEPROM.read(EEPROM_SEQ_START + 1);

      if (game != ALTERNATIVE_ENDING_PREFIX || seq != (ALTERNATIVE_ENDING_SEQ - 1)) {

        gameState = GameState::InvalidSeq;
        
      }

    }

  #endif

  myHero.setHitPoints(START_HP);
  myHero.setDefence(START_DF);
  myHero.setAttackPower(START_AP);

  myHero.setInventory(0, ItemType::None);
  myHero.setInventory(1, ItemType::None);
  myHero.setInventory(2, ItemType::None);
  myHero.setInventory(3, ItemType::Shield);
  myHero.setInventory(4, ItemType::Sword);

  playerLevel = 1;
  level = 0;

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Initialise Level.
 *
 *  Initialise the player, level and enemy details based on the current level.
 *
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void initialiseLevel(Player *myHero, Level *myLevel, const uint8_t *level) {

  uint8_t idx = 0;


  // Read level title ..

  memcpy_P(myLevel->getTitleLine1(), &level[idx], sizeof(char) * 11);
  idx += 11;
  memcpy_P(myLevel->getTitleLine2(), &level[idx], sizeof(char) * 11);
  idx += 11;

  myHero->setX(pgm_read_byte(&level[idx++]));
  myHero->setY(pgm_read_byte(&level[idx++]));
  myHero->setDirection((Direction)pgm_read_byte(&level[idx++]));

  myLevel->setWidth(pgm_read_byte(&level[idx++]));
  myLevel->setHeight(pgm_read_byte(&level[idx++]));


  // Create all enemies ..

  idx = loadEnemies(level, enemies, idx, NUMBER_OF_ENEMIES);


  // Create all items and doors ..

  idx = loadItems(level, items, idx, NUMBER_OF_ITEMS);
  idx = loadItems(level, doors, idx, NUMBER_OF_DOORS);

  myLevel->setLevel(level);
  myLevel->setDoors(doors);
  myLevel->setStartPos(idx);

  gameState = GameState::Move;

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Initialise Level - load items.
 *
 *  Initialise the item and door items from progmem using the 'idx' variable as an index reference.
 *
 *  Returns the 'idx' index reference so the next process can continue reading from that location onwards.
 *
 * -----------------------------------------------------------------------------------------------------------------------------
 */
uint8_t loadItems(const uint8_t *level, Item * items, uint8_t idx, uint8_t max) {

  uint8_t numberOfItems = pgm_read_byte(&level[idx++]);

  for (uint8_t i = 0; i < max; ++i) {

    items[i].setEnabled(false);

    if (i < numberOfItems) {
      items[i].setItemType((ItemType)pgm_read_byte(&level[idx++]));
      items[i].setX(pgm_read_byte(&level[idx++]));
      items[i].setY(pgm_read_byte(&level[idx++]));
      items[i].setEnabled(true);
    }

  }

  return idx;

}



/* -----------------------------------------------------------------------------------------------------------------------------
 *  Initialise Level - load enemies.
 *
 *  Initialise the enemies items from progmem using the 'idx' variable as an index reference.
 *
 *  Returns the 'idx' index reference so the next process can continue reading from that location onwards.
 *
 * -----------------------------------------------------------------------------------------------------------------------------
 */
uint8_t loadEnemies(const uint8_t * level, Enemy * enemies, uint8_t idx, uint8_t max) {

  struct StatSet { uint8_t hp, ap, xp; bool moving; };

  static const StatSet loadEnemiesStats[] PROGMEM {
    { ENEMY_OCCULAR_HP, ENEMY_OCCULAR_AP, ENEMY_OCCULAR_XP, ENEMY_OCCULAR_MV },
    { ENEMY_SKELETON_HP, ENEMY_SKELETON_AP, ENEMY_SKELETON_XP, ENEMY_SKELETON_MV },
    { ENEMY_SPARKAT_HP, ENEMY_SPARKAT_AP, ENEMY_SPARKAT_XP, ENEMY_SPARKAT_MV },
    { ENEMY_WRAITH_HP, ENEMY_WRAITH_AP, ENEMY_WRAITH_XP,  ENEMY_WRAITH_MV },
    { ENEMY_DRAGON_HP, ENEMY_DRAGON_AP, ENEMY_DRAGON_XP, ENEMY_DRAGON_MV },
    { ENEMY_RAT_HP, ENEMY_RAT_AP, ENEMY_RAT_XP, ENEMY_RAT_MV, },
    { ENEMY_SLIME_HP, ENEMY_SLIME_AP, ENEMY_SLIME_XP, ENEMY_SLIME_MV }
  };

  uint8_t numberOfEnemies = pgm_read_byte(&level[idx++]);

  for (uint8_t i = 0; i < max; ++i) {

    enemies[i].setEnabled(i < numberOfEnemies);

    if (enemies[i].getEnabled()) {

      const EnemyType enemyType = (EnemyType)pgm_read_byte(&level[idx++]);

      enemies[i].setEnemyType(enemyType);
      enemies[i].setX(pgm_read_byte(&level[idx++]));
      enemies[i].setY(pgm_read_byte(&level[idx++]));

      const uint8_t hp = pgm_read_byte(&loadEnemiesStats[static_cast<uint8_t>(enemyType)].hp);
      const uint8_t ap = pgm_read_byte(&loadEnemiesStats[static_cast<uint8_t>(enemyType)].ap);
      const uint8_t xp = pgm_read_byte(&loadEnemiesStats[static_cast<uint8_t>(enemyType)].xp);
      const bool moving = pgm_read_byte(&loadEnemiesStats[static_cast<uint8_t>(enemyType)].moving) != 0;

      enemies[i].setHitPoints(hp);
      enemies[i].setHitPointsMax(hp);
      enemies[i].setAttackPower(ap);
      enemies[i].setExperiencePoints(xp);
      enemies[i].setMoving(moving);

    }

  }

 return idx;

}

#ifdef SAVE_GAME

#define EEPROM_START            EEPROM_STORAGE_SPACE_START + 100
#define EEPROM_START_C1         EEPROM_START
#define EEPROM_START_C2         EEPROM_START + 1
#define EEPROM_LEVEL_NO         EEPROM_START + 2
#define EEPROM_PLAYER_LEVEL     EEPROM_START + 3
#define EEPROM_PLAYER_START     EEPROM_START + 4
#define EEPROM_PLAYER_LENGTH    12
#define EEPROM_ENEMY_START      EEPROM_PLAYER_START + EEPROM_PLAYER_LENGTH
#define EEPROM_ENEMY_LENGTH     7
#define EEPROM_ITEM_LENGTH      4
#define EEPROM_DOOR_LENGTH      4


/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised?
 *
 *   Looks for the characters 'D' and 'U' in the first two bytes of the EEPROM
 *   memory range starting from byte EEPROM_STORAGE_SPACE_START.  If not found,
 *   it resets the settings ..
 */
bool initEEPROM() {

  byte c1 = EEPROM.read(EEPROM_START_C1);
  byte c2 = EEPROM.read(EEPROM_START_C2);

  if (c1 != 68 || c2 != 85) { // RU 68 85

    EEPROM.update(EEPROM_START_C1, 68);
    EEPROM.update(EEPROM_START_C2, 85);
    EEPROM.update(EEPROM_LEVEL_NO, 255);

  }

}


/* -----------------------------------------------------------------------------
 *   Get the current level number. Should return 255 if no level has been saved.
 */
uint8_t getLevel() {

  return EEPROM.read(EEPROM_LEVEL_NO);

}


/* -----------------------------------------------------------------------------
 *   Save game state.
 */
void saveGame() {

  EEPROM.update(EEPROM_LEVEL_NO, level);
  EEPROM.update(EEPROM_PLAYER_LEVEL, playerLevel);
  EEPROM.put(EEPROM_PLAYER_START, myHero);

  uint16_t loc = EEPROM_ENEMY_START;

  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    EEPROM.put(loc, enemies[i]);
    loc = loc + EEPROM_ENEMY_LENGTH;
  }

  for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
    EEPROM.put(loc, items[i]);
    loc = loc + EEPROM_ITEM_LENGTH;
  }

  for (int i = 0; i < NUMBER_OF_DOORS; i++) {
    EEPROM.put(loc, doors[i]);
    loc = loc + EEPROM_DOOR_LENGTH;
  }

}


/* -----------------------------------------------------------------------------
 *   Restore game state.
 */
void restoreGame() {

  level = EEPROM.read(EEPROM_LEVEL_NO);
  playerLevel = EEPROM.read(EEPROM_PLAYER_LEVEL);

  initialiseLevel(&myHero, &myLevel, levels[level]);

  EEPROM.get(EEPROM_PLAYER_START, myHero);

  uint16_t loc = EEPROM_ENEMY_START;

  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    Enemy enemy;
    EEPROM.get(loc, enemy);
    enemies[i] = enemy;
    loc = loc + EEPROM_ENEMY_LENGTH;
  }

  for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
    Item item;
    EEPROM.get(loc, item);
    items[i] = item;
    loc = loc + EEPROM_ITEM_LENGTH;
  }

  for (int i = 0; i < NUMBER_OF_DOORS; i++) {
    Item door;
    EEPROM.get(loc, door);
    doors[i] = door;
    loc = loc + EEPROM_DOOR_LENGTH;
  }

}
#endif