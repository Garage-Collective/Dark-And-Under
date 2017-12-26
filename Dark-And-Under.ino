#include "src/ArduboyExt/Arduboy2.h"
#include "Enums.h"
#include "src/levels/Level.h"
#include "images/Images.h"
#include "src/entities/Player.h"
#include "src/entities/Enemy.h"
#include "src/entities/Item.h"
#include "src/levels/MapData.h"
#include "src/controllers/PlayerController.h"
#include "src/controllers/EnemyController.h"
#include "src/fonts/Font3x5.h"
#include "src/utils/Utils.h"
#include "src/utils/EnemyNames.h"


Arduboy2 arduboy;
Font3x5 font3x5 = Font3x5();

Item items[NUMBER_OF_ITEMS];
Item doors[NUMBER_OF_DOORS];
Enemy enemies[NUMBER_OF_ENEMIES];

uint8_t attackingEnemyIdx = 0;

const uint8_t *levels[] =    { level_00, level_01, level_02, level_03, level_04, level_05, level_06, level_07, level_08, level_09 };
const uint8_t *map_tiles[] = { tile_00, tile_01, tile_02, tile_03, tile_04, tile_05, tile_06, tile_07, tile_08, tile_09,
                               tile_10, tile_11, tile_12, tile_13, tile_14, tile_15, tile_16, tile_17, tile_18, tile_19 };
const uint8_t *map_images[] = { visionBack, closeWallFront, closeGateLocked, closeDoorLocked, closeDoorUnlocked, closeWallLeft, closeWallRight, closeGateLeft, closeGateRight,
                                midWallFront, midDoorLocked, midDoorLevelLocked, midDoorLevelUnlocked, midWallLeft, midWallRight, midGateLeft, midGateRight,
                                farGateLocked, farDoorLocked, farDoorUnlocked, farWallLeft, farWallRight };

const uint8_t *direction_images[] = { directionN, directionE, directionS, directionW };


// Inventory settings ..

const uint8_t *inventory_images[] = { NULL, inv_key, inv_potion, inv_scroll, inv_shield, inv_sword };
const Point inventory_Coords[] = { INVENTORY_POSITION_1, INVENTORY_POSITION_2, INVENTORY_POSITION_3, INVENTORY_POSITION_4, INVENTORY_POSITION_5 };
uint8_t inventory_selection = 0;
uint8_t inventory_action = 0;


// Enemy details ..

const uint8_t *enemy_images[] = { enemy_occular, enemy_skeleton, enemy_sparkat, enemy_wraith, enemy_dragon, enemy_rat, enemy_slime };
const uint8_t *enemy_masks[] = { enemy_occular_Mask, enemy_skeleton_Mask, enemy_sparkat_Mask, enemy_wraith_Mask, enemy_dragon_Mask, enemy_rat_Mask, enemy_slime_Mask };


// Item details ..

const uint8_t *item_images[] = { NULL, item_key, item_potion, item_mascroll, item_shield, item_sword };
uint8_t itemAction = 0;
uint8_t savedItem = 0;

GameState gameState = GameState::Splash;
GameState savedState = GameState::Splash;

Level myLevel;
Player myHero;

SplashButtons splashStatus = SplashButtons::Play;
FightButtons fightButton = FightButtons::Attack;
#ifdef LEVEL_UP_SELECT_PRIZE
LevelUpButtons levelUpButton = LevelUpButtons::None;
#endif

uint8_t level = 0;          // Current map
uint8_t playerLevel = 1;    // Levelup level


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Setup ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void setup() {

  arduboy.boot();
  arduboy.flashlight();
  arduboy.setFrameRate(30);

  #ifdef USE_SOUNDS
  arduboy.audio.begin();
  #endif

  arduboy.initRandomSeed();
  myLevel.setMapTiles(map_tiles);

  #ifdef SAVE_GAME
  initEEPROM();
  #endif

  initialiseGame();

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Main loop ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void loop() {

  uint16_t delayLength = 0;
  if (!(arduboy.nextFrame())) return;

  arduboy.pollButtons();
  arduboy.clear();

  switch (gameState) {

    case GameState::InitGame:
      level = 0;
      gameState = GameState::InitLevel;
      break;

    case GameState::InitLevel:
      initialiseLevel(&myHero, &myLevel, levels[level]);
      break;

    case GameState::Move:
    case GameState::ItemIgnore:
      delayLength = playLoop();
      break;

    case GameState::InventorySelect:
    case GameState::InventoryAction:
      delayLength = inventoryLoop();
      break;

    case GameState::ItemSelect:
      delayLength = itemLoop();
      break;

    case GameState::Splash:
      displaySplash();
      break;

    case GameState::About:
      displayLogo();
      break;

    case GameState::Battle_EnemyAttacks_Init:
    case GameState::Battle_EnemyAttacks:
    case GameState::Battle_EnemyDies:
    case GameState::Battle_PlayerDecides:
    case GameState::Battle_PlayerAttacks:
    case GameState::Battle_PlayerDefends:
    case GameState::Battle_PlayerCastsSpell:
      delayLength = battleLoop();
      break;

    case GameState::Battle_PlayerDies:
      displayEndOfGame(true);
      break;

    case GameState::LevelUp:
      delayLength = displayLevelUp();
      break;

    case GameState::NextLevel:
      displayNextLevel();
      break;

    case GameState::EndOfGame:
      displayEndOfGame(false);
      break;

    #ifdef USE_LARGE_MAP
    case GameState::DisplayLargeMap:
      displayLargeMap();
      break;
    #endif

    default:
      break;

  }

  arduboy.display();
  delay(delayLength);

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Level Up Handler
 *
 *  Display the 'Level Up' graphic and award a point.
 *
 * -----------------------------------------------------------------------------------------------------------------------------
 */
uint16_t displayLevelUp() {

  arduboy.drawCompressed(0, 0, frames_outside, WHITE);
  arduboy.drawCompressed(66, 4, frames_inside, WHITE);

  #ifdef USE_LARGE_MAP
  drawMapAndStatistics(&myHero, &myLevel, true);
  #endif
  #ifndef USE_LARGE_MAP
  drawMapAndStatistics(&myHero, &myLevel);
  #endif
  drawDirectionIndicator(&myHero);
  drawLevelDescription(&myLevel);

  arduboy.drawCompressed(23, 5, levelUp, WHITE);

  #ifndef LEVEL_UP_SELECT_PRIZE

    font3x5.setCursor(20, 40);
    font3x5.print(F("LEVEL "));
    font3x5.print(playerLevel);
    playerLevel++;

    font3x5.setCursor(18, 47);
    font3x5.print(F("YOU GAIN"));
    font3x5.setCursor(26, 54);

    #ifdef LEVEL_UP_INC_HP
      switch (random(0, 3)) {

        case 0:
          font3x5.print(F("1 HP"));
          myHero.setHitPoints(myHero.getHitPoints() + 1);
          break;

        case 1:
          font3x5.print(F("1 AP"));
          myHero.setAttackPower(myHero.getAttackPower() + 1);
          break;

        case 2:
          font3x5.print(F("1 DF"));
          myHero.setDefence(myHero.getDefence() + 1);
          break;


      }
    #endif

    #ifndef LEVEL_UP_INC_HP
      switch (random(0, 2)) {

        case 0:
          font3x5.print(F("1 AP"));
          myHero.setAttackPower(myHero.getAttackPower() + 1);
          break;

        case 1:
          font3x5.print(F("1 DF"));
          myHero.setDefence(myHero.getDefence() + 1);
          break;


      }
    #endif

    gameState = GameState::Move;
    return LEVEL_UP_DELAY;

  #endif

  #ifdef LEVEL_UP_SELECT_PRIZE

    uint8_t buttons = arduboy.justPressedButtons();

    switch (levelUpButton) {

      case LevelUpButtons::None:

        font3x5.setCursor(20, 42);
        font3x5.print(F("LEVEL "));
        font3x5.print(playerLevel);
        font3x5.setCursor(9, 50);
        font3x5.print(F("CHOOSE BONUS"));
        playerLevel++;

        levelUpButton = LevelUpButtons::AP;
        return LEVEL_UP_DELAY;

      default:
        font3x5.setCursor(23, 42);
        font3x5.print(F("ATTACK"));
        font3x5.setCursor(23, 50);
        font3x5.print(F("DEFENCE"));
        break;

    }

    if (levelUpButton == LevelUpButtons::AP) {
      Sprites::drawSelfMasked(17, 43, hMarker, 0);
    }
    else {
      Sprites::drawSelfMasked(17, 51, hMarker, 0);
    }

    if ((buttons & UP_BUTTON_MASK) && levelUpButton == LevelUpButtons::DF)             { levelUpButton = LevelUpButtons::AP; }
    else if ((buttons & DOWN_BUTTON_MASK) && levelUpButton == LevelUpButtons::AP)      { levelUpButton = LevelUpButtons::DF; }
    else if ((buttons & A_BUTTON_MASK) && levelUpButton == LevelUpButtons::AP)         { myHero.setAttackPower(myHero.getAttackPower() + 1); levelUpButton = LevelUpButtons::None, gameState = GameState::Move; }
    else if ((buttons & A_BUTTON_MASK) && levelUpButton == LevelUpButtons::DF)         { myHero.setDefence(myHero.getDefence() + 1); levelUpButton = LevelUpButtons::None, gameState = GameState::Move; }

  #endif

  return 0;

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Next Level Handler
 *
 *  Display the 'Next Level' graphic and initialise the next level ready for play.
 *
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void displayNextLevel() {

  arduboy.drawCompressed(0, 0, frames_outside, WHITE);
  arduboy.drawCompressed(64, 4, endOfLevel, WHITE);

  font3x5.setHeight(7);
  font3x5.setCursor(10, 7);
  font3x5.print(F("YOU CAN HEAR\nA DISTANT\nYET CLOSER\nDRAGON ROAR\nAS YOU STEP\nTHROUGH THE\nDOOR..."));
  font3x5.setHeight(8);

  uint8_t buttons = arduboy.justPressedButtons();

  if (buttons) {

    level++;
    gameState = GameState::Move;
    initialiseLevel(&myHero, &myLevel, levels[level]);

  }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  End of Game Handler
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void displayEndOfGame(bool playerDead) {

  arduboy.drawCompressed(0, 0, frames_outside, WHITE);

  if (playerDead) {

    arduboy.drawCompressed(66, 4, frames_inside, WHITE);
    Sprites::drawSelfMasked(DIRECTION_X_OFFSET, DIRECTION_Y_OFFSET, fight_icon, 0);
    #ifdef USE_LARGE_MAP
    drawMapAndStatistics(&myHero, &myLevel, true);
    #endif
    #ifndef USE_LARGE_MAP
    drawMapAndStatistics(&myHero, &myLevel);
    #endif

    arduboy.drawCompressed(16, 6, gameOver, WHITE);

  }
  else {

    arduboy.drawCompressed(43, 4, victory, WHITE);
    font3x5.setCursor(9, 8);
    font3x5.print(F("WELL DONE!\nTHE RICHES\nUNDER THE\nMOUNTAIN\nARE YOURS\nNOW!"));

  }

  uint8_t buttons = arduboy.justPressedButtons();

  if (buttons & SELECT_BUTTON_MASK) {

    initialiseGame();
    gameState = GameState::Splash;

  }

}
