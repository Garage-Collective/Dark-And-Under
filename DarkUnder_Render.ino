#include <Arduboy2.h>

#define VISION_X_OFFSET   3
#define VISION_Y_OFFSET   4

const int8_t offsetXTable[] = { 0, 1, 0, -1 };
const int8_t offsetYTable[] = { -1, 0, 1, 0 };


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Draw Player Vision.
 *
 *  Renders the dungeon with any doors, enemies and items that are in view.
 *
 *  To save space elsewhere, it automatically calls the drawMapAndStatistics() function to render the map and statistics panel
 *  as these are always rendered when the dungeon is rendered.
 *
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void drawPlayerVision(Player *myHero, Level *myLevel) {

  bool horizon3Plus = false;
  bool horizon2Plus = false;

  int8_t farLeftX = 0;
  int8_t farFrontX = 0;
  int8_t farRightX = 0;

  int8_t farLeftY = 0;
  int8_t farFrontY = 0;
  int8_t farRightY = 0;

  int8_t middleLeftX = 0;
  int8_t middleFrontX = 0;
  int8_t middleRightX = 0;

  int8_t middleLeftY = 0;
  int8_t middleFrontY = 0;
  int8_t middleRightY = 0;

  int8_t closeLeftX = 0;
  int8_t closeFrontX = 0;
  int8_t closeRightX = 0;

  int8_t closeLeftY = 0;
  int8_t closeFrontY = 0;
  int8_t closeRightY = 0;

  uint8_t playerX = myHero->getX();
  uint8_t playerY = myHero->getY();

  switch (myHero->getDirection()) {

    case Direction::North:

      horizon3Plus = (playerY - 3 >= 0);
      horizon2Plus = (playerY - 2 >= 0);

      farFrontX = 0;      farFrontY = -3;
      farLeftX = -1;      farLeftY = -2;
      farRightX = 1;      farRightY = -2;

      middleFrontX = 0;   middleFrontY = -2;
      middleLeftX = -1;   middleLeftY = -1;
      middleRightX = 1;   middleRightY = -1;

      closeFrontX = 0;    closeFrontY = -1;
      closeLeftX = -1;    closeLeftY = 0;
      closeRightX = 1;    closeRightY = 0;

      break;

    case Direction::East:

      horizon3Plus = (playerX + 3 >= 0);
      horizon2Plus = (playerX + 2 >= 0);

      farFrontX = 3;      farFrontY = 0;
      farLeftX = 2;       farLeftY = -1;
      farRightX = 2;      farRightY = 1;

      middleFrontX = 2;   middleFrontY = 0;
      middleLeftX = 1;    middleLeftY = -1;
      middleRightX = 1;   middleRightY = 1;

      closeFrontX = 1;    closeFrontY = 0;
      closeLeftX = 0;     closeLeftY = -1;
      closeRightX = 0;    closeRightY = 1;

      break;

    case Direction::South:

      horizon3Plus = (playerY + 3 >= 0);
      horizon2Plus = (playerY + 2 >= 0);

      farFrontX = 0;      farFrontY = 3;
      farLeftX = 1;       farLeftY = 2;
      farRightX = -1;     farRightY = 2;

      middleFrontX = 0;   middleFrontY = 2;
      middleLeftX = 1;    middleLeftY = 1;
      middleRightX = -1;  middleRightY = 1;

      closeFrontX = 0;    closeFrontY = 1;
      closeLeftX = 1;     closeLeftY = 0;
      closeRightX = -1;   closeRightY = 0;

      break;

    case Direction::West:

      horizon3Plus = (playerX - 3 >= 0);
      horizon2Plus = (playerX - 2 >= 0);

      farFrontX = -3;     farFrontY = 0;
      farLeftX = -2;      farLeftY = 1;
      farRightX = -2;     farRightY = -1;

      middleFrontX = -2;  middleFrontY = 0;
      middleLeftX = -1;   middleLeftY = 1;
      middleRightX = -1;  middleRightY = -1;

      closeFrontX = -1;   closeFrontY = 0;
      closeLeftX = 0;     closeLeftY = 1;
      closeRightX = 0;    closeRightY = -1;

      break;

    default: break;

  }


  arduboy.drawCompressed(0, 0, frames_outside, WHITE);
  arduboy.drawCompressed(66, 4, frames_inside, WHITE);
//  arduboy.drawCompressed(VISION_X_OFFSET + 1, VISION_Y_OFFSET, map_images[MAP_IMAGE_BACK], WHITE);
  Sprites::drawOverwrite(VISION_X_OFFSET + 1, VISION_Y_OFFSET, map_images[MAP_IMAGE_BACK], 0);


  // Far front wall ..

  if (horizon3Plus) {

    MapElement mapElement = (MapElement)myLevel->getMapElement(playerX + farFrontX, playerY + farFrontY, false);

    int8_t imageIndex = 0;
    int8_t xOffset = 0;
    int8_t yOffset = 0;

    if (mapElement > MapElement::Floor) {

      switch (mapElement) {

      case MapElement::LockedGate:
      case MapElement::SelfLockingDoor:
          imageIndex = MAP_IMAGE_FAR_GATE_LOCKED;
          xOffset = 27;
          yOffset = 26;
          break;

        case MapElement::LockedDoor:
          imageIndex = MAP_IMAGE_FAR_DOOR_LOCKED;
          xOffset = 29;
          yOffset = 25;
          break;

        case MapElement::UnlockedDoor:
          imageIndex = MAP_IMAGE_FAR_DOOR_UNLOCKED;
          xOffset = 29;
          yOffset = 25;
          break;

        default: break;

      }

      for (uint8_t i = VISION_X_OFFSET + 1; i < VISION_X_OFFSET + 62; ++i) {
        arduboy.drawPixel(i, VISION_Y_OFFSET + 27 + (i %2), WHITE);
      }

      if (imageIndex > 0) {
        Sprites::drawOverwrite(VISION_X_OFFSET + xOffset, VISION_Y_OFFSET + yOffset, map_images[imageIndex], 0);
      }

    }

  }


  // Far left wall ..

  if (horizon2Plus) {
    if ((MapElement)myLevel->getMapElement(playerX + farLeftX, playerY + farLeftY, false) > MapElement::Floor) {
     Sprites::drawOverwrite(VISION_X_OFFSET + 1, VISION_Y_OFFSET + 23, map_images[MAP_IMAGE_FAR_LEFT], 0);
    }
  }


  // Far right wall ..

  if (horizon2Plus) {
    if ((MapElement)myLevel->getMapElement(playerX + farRightX, playerY + farRightY, false) > MapElement::Floor) {
     Sprites::drawOverwrite(VISION_X_OFFSET + 35, VISION_Y_OFFSET + 23, map_images[MAP_IMAGE_FAR_RIGHT], 0);
    }
  }


  // Mid front wall ..

  bool renderMiddleFront = false;

  if (horizon2Plus) {

    int8_t imageIndex = 0;
    int8_t xOffset = 0;
    int8_t yOffset = 0;

    MapElement mapElement = (MapElement)myLevel->getMapElement(playerX + middleFrontX, playerY + middleFrontY, false);

    if (mapElement > MapElement::Floor) {

      renderMiddleFront = (horizon2Plus && (mapElement > MapElement::Floor));

      switch (mapElement) {

        case MapElement::LockedGate:
        case MapElement::SelfLockingDoor:
          imageIndex = MAP_IMAGE_MID_GATE_LOCKED;
          xOffset = 24;
          yOffset = 23;
          break;

        case MapElement::LockedDoor:
          imageIndex = MAP_IMAGE_MID_DOOR_LOCKED;
          xOffset = 27;
          yOffset = 25;
          break;

        case MapElement::UnlockedDoor:
          imageIndex = MAP_IMAGE_MID_DOOR_UNLOCKED;
          xOffset = 27;
          yOffset = 25;
          break;

        default: break;

      }

      Sprites::drawOverwrite(VISION_X_OFFSET + 1, VISION_Y_OFFSET + 23, map_images[MAP_IMAGE_MID_FRONT], 0);

      if (imageIndex > 0) {
        Sprites::drawOverwrite(VISION_X_OFFSET + xOffset, VISION_Y_OFFSET + yOffset, map_images[imageIndex], 0);
      }

    }

  }


  // Mid left wall ..

  MapElement mapElement = myLevel->getMapElement(playerX + middleLeftX, playerY + middleLeftY, false);
  if (mapElement == MapElement::Wall) {
    Sprites::drawOverwrite(VISION_X_OFFSET + 1, VISION_Y_OFFSET + 14, map_images[MAP_IMAGE_MID_LEFT], 0);
  }
  else if (mapElement == MapElement::LockedGate || mapElement == MapElement::SelfLockingDoor) {
    Sprites::drawOverwrite(VISION_X_OFFSET + 15, VISION_Y_OFFSET + 15, map_images[MAP_IMAGE_MID_GATE_LEFT], 0);
  }


  // Mid right wall ..

  mapElement = myLevel->getMapElement(playerX + middleRightX, playerY + middleRightY, false);
  if (mapElement == MapElement::Wall) {
    Sprites::drawOverwrite(VISION_X_OFFSET + 39, VISION_Y_OFFSET + 14, map_images[MAP_IMAGE_MID_RIGHT], 0);
  }
  else if (mapElement == MapElement::LockedGate || mapElement == MapElement::SelfLockingDoor) {
    Sprites::drawOverwrite(VISION_X_OFFSET + 39, VISION_Y_OFFSET + 15, map_images[MAP_IMAGE_MID_GATE_RIGHT], 0);
  }


  // Close front wall ..

  mapElement = (MapElement)myLevel->getMapElement(playerX + closeFrontX, playerY + closeFrontY, false);
  bool renderCloseFront = (mapElement > MapElement::Floor);

  if (mapElement > MapElement::Floor) {

    int8_t imageIndex = -1;     //TODO: CHanged to 0 and if (imageIndex > 0) .. added 4 bytes
    int8_t xOffset = 0;
    int8_t yOffset = 0;

    switch (mapElement) {

      case MapElement::Wall:
        imageIndex = MAP_IMAGE_CLOSE_FRONT;
        xOffset = 0;
        yOffset = 14;
        break;

      case MapElement::LockedGate:
      case MapElement::SelfLockingDoor:
        imageIndex = MAP_IMAGE_CLOSE_GATE_LOCKED;
        xOffset = 15;
        yOffset = 14;
        break;

      case MapElement::LockedDoor:
        imageIndex = MAP_IMAGE_CLOSE_DOOR_LOCKED;
        xOffset = 22;
        yOffset = 21;
        break;

      case MapElement::UnlockedDoor:
        imageIndex = MAP_IMAGE_CLOSE_DOOR_UNLOCKED;
        xOffset = 22;
        yOffset = 21;

      default: break;

    }

    Sprites::drawOverwrite(VISION_X_OFFSET, VISION_Y_OFFSET + 14, closeWallFront, 0);
    if (imageIndex >= 0) {
      Sprites::drawOverwrite(VISION_X_OFFSET + xOffset, VISION_Y_OFFSET + yOffset, map_images[imageIndex], 0);
    }

  }


  // Close left wall ..

  mapElement = myLevel->getMapElement(playerX + closeLeftX, playerY + closeLeftY, false);
  if (mapElement == MapElement::Wall) {
    Sprites::drawOverwrite(VISION_X_OFFSET, VISION_Y_OFFSET, map_images[MAP_IMAGE_CLOSE_LEFT], 0);
  }
  else if (mapElement == MapElement::LockedGate || mapElement == MapElement::SelfLockingDoor) {
    Sprites::drawOverwrite(VISION_X_OFFSET, VISION_Y_OFFSET, map_images[MAP_IMAGE_CLOSE_GATE_LEFT], 0);
  }


  // Close right wall ..

  mapElement = myLevel->getMapElement(playerX + closeRightX, playerY + closeRightY, false);
  if (mapElement == MapElement::Wall) {
    Sprites::drawOverwrite(VISION_X_OFFSET + 48, VISION_Y_OFFSET, map_images[MAP_IMAGE_CLOSE_RIGHT], 0);
  }
  else if (mapElement == MapElement::LockedGate || mapElement == MapElement::SelfLockingDoor) {
    Sprites::drawOverwrite(VISION_X_OFFSET + 49, VISION_Y_OFFSET, map_images[MAP_IMAGE_CLOSE_GATE_RIGHT], 0);
  }


  uint8_t selector = static_cast<uint8_t>(myHero->getDirection());
  bool rendered = false;

  if (!renderCloseFront) {

    for (uint8_t i = 0; i < NUMBER_OF_ITEMS; ++i) {

      Enemy enemy = enemies[i];

      if (enemy.getEnabled()) {

        int8_t offsetX = offsetXTable[selector] * 2;
        int8_t offsetY = offsetYTable[selector] * 2;

        if (enemy.getX() == playerX + offsetX && enemy.getY() == playerY + offsetY) {

          Sprites::drawOverwrite(27, 25, enemy_two_tiles, 0);
          rendered = true;
          break;

        }

      }

    }

  }

  if (!renderCloseFront && !renderMiddleFront && !rendered) {

    for (uint8_t i = 0; i < NUMBER_OF_ITEMS; ++i) {

      Enemy enemy = enemies[i];

      if (enemy.getEnabled()) {

        int8_t offsetX = offsetXTable[selector] * 3;
        int8_t offsetY = offsetYTable[selector] * 3;

        if (enemy.getX() == playerX + offsetX && enemy.getY() == playerY + offsetY) {

          Sprites::drawOverwrite(30, 28, enemy_three_tiles, 0);
          break;

        }

      }

    }

  }


  for (uint8_t i = 0; i < NUMBER_OF_ENEMIES; ++i) {

    Enemy enemy = enemies[i];

    if (enemy.getEnabled()) {

      int16_t deltaX = myHero->getX() - enemy.getX();
      int16_t deltaY = myHero->getY() - enemy.getY();

      if ((deltaY == 0 && absT(deltaX) == 1) ^ (deltaX == 0 && absT(deltaY) == 1)) {

        if (deltaX > 0) { myHero->setDirection(Direction::West); }
        else if (deltaX < 0) { myHero->setDirection(Direction::East); }
        else if (deltaY > 0) { myHero->setDirection(Direction::North); }
        else if (deltaY < 0) { myHero->setDirection(Direction::South); }

        EnemyType enemyType = enemy.getEnemyType();
        uint8_t offsetX = 6;  // Occular
        uint8_t offsetY = 5;

        if (enemyType == EnemyType::Skeleton)         { offsetX = 11; offsetY = 5;  }
        else if (enemyType == EnemyType::Sparkat)     { offsetX = 8;  offsetY = 10; }
        else if (enemyType == EnemyType::Wraith)      { offsetX = 16; offsetY = 3;  }
        else if (enemyType == EnemyType::Dragon)      { offsetX = 3;  offsetY = 5;  }
        else if (enemyType == EnemyType::Rat)         { offsetX = 23; offsetY = 16; }
        else if (enemyType == EnemyType::Slime)       { offsetX = 13; offsetY = 22; }

        arduboy.drawCompressed(offsetX, offsetY, enemy_masks[(uint8_t)enemyType], BLACK);
        arduboy.drawCompressed(offsetX, offsetY, enemy_images[(uint8_t)enemyType], WHITE);

        if (gameState == GameState::Move) gameState = GameState::Battle_EnemyAttacks_Init;
        attackingEnemyIdx = i;
        break;

      }

    }

    if (gameState == GameState::Move || gameState == GameState::ItemSelect) {

      Item item = items[i];

      if (item.getEnabled()) {

        uint8_t selector = static_cast<uint8_t>(myHero->getDirection());

        int8_t offsetX = offsetXTable[selector];
        int8_t offsetY = offsetYTable[selector];

        if (item.getX() == playerX + offsetX && item.getY() == playerY + offsetY) {

          ItemType itemType = item.getItemType();

          uint8_t offsetX = 23;  // Potion
          uint8_t offsetY = 19;

          if (itemType == ItemType::Key)          { offsetX = 20; offsetY = 20; }
          else if (itemType == ItemType::Scroll)  { offsetX = 20; offsetY = 18; }
          else if (itemType == ItemType::Shield)  { offsetX = 20; offsetY = 18; }
          else if (itemType == ItemType::Sword)   { offsetX = 20; offsetY = 18; }

          arduboy.fillRect(14, 11, 41, 43, BLACK);
          arduboy.fillRect(15, 12, 39, 41, WHITE);
          arduboy.drawCompressed(offsetX, offsetY, item_images[(uint8_t)itemType], BLACK);

          font3x5.setTextColor(BLACK);
          font3x5.setCursor(17, 12);
          font3x5.print(F("YOU FOUND"));

          const __FlashStringHelper * text = nullptr;

          switch (itemType) {

            case ItemType::Key:
              text = F("SOME KEYS");
              break;

            case ItemType::Potion:
              text = F("HP POTION");
              break;

            case ItemType::Scroll:
              text = F("A SCROLL");
              break;

            case ItemType::Shield:
              text = F("A SHIELD");
              break;

            case ItemType::Sword:
              text = F(" A SWORD");
              break;

            default: break;

          }

          if(text != nullptr) {
            font3x5.setCursor(17, 46);
            font3x5.print(text);
          }

          font3x5.setTextColor(WHITE);
          gameState = GameState::ItemSelect;
          savedItem = i;
          break;

        }

      }

    }

  }

  #ifdef USE_LARGE_MAP
  drawMapAndStatistics(myHero, myLevel, true);
  #endif
  #ifndef USE_LARGE_MAP
  drawMapAndStatistics(myHero, myLevel);
  #endif

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Draw the Map and Statistics.
 *
 *  Was originally two separate functions however they are always rendered together.
 *
 * -----------------------------------------------------------------------------------------------------------------------------
 */
#ifndef USE_LARGE_MAP
#define MAP_X_OFFSET   98
#define MAP_Y_OFFSET   6
#endif

#ifdef USE_LARGE_MAP
void drawMapAndStatistics(Player *player, Level *myLevel, boolean smallMap) {
#endif
#ifndef USE_LARGE_MAP
void drawMapAndStatistics(Player *player, Level *myLevel) {
#endif

  uint8_t drawX = 0;
  uint8_t drawY = 0;

  #ifdef USE_LARGE_MAP
  uint8_t mapWidth = (smallMap ? 2 : 11);
  uint8_t mapHeight = (smallMap ? 3 : 5);
  uint8_t mapXOffset = (smallMap ? 98 : 6);
  uint8_t mapYOffset = (smallMap ? 6 : 5);
  #endif

  bool renderMapElement = false;

  #ifdef USE_LARGE_MAP
  for (int16_t mapY = player->getY() - mapHeight; mapY <= player->getY() + mapHeight; ++mapY) {
  #endif
  #ifndef USE_LARGE_MAP
  for (int16_t mapY = player->getY() - 3; mapY <= player->getY() + 3; ++mapY) {
  #endif

    #ifdef USE_LARGE_MAP
    for (int16_t mapX = player->getX() - mapWidth; mapX <= player->getX() + mapWidth; ++mapX) {
    #endif
    #ifndef USE_LARGE_MAP
    for (int16_t mapX = player->getX() - 2; mapX <= player->getX() + 2; ++mapX) {
    #endif

      #ifdef USE_LARGE_MAP
      if (mapX >= 0 && mapX < ((uint8_t)myLevel->getWidth() * MAP_TILE_WIDTH) && mapY >= 0 && mapY < ((uint8_t)myLevel->getHeight() * MAP_TILE_HEIGHT) && (smallMap ? !(drawX == 4 && drawY == 0) : ( !((drawX == 0 || drawX == 22) && drawY == 0) && !((drawX == 0 || drawX == 22) && drawY == 10) ) ) ) {
      #endif
      #ifndef USE_LARGE_MAP
      if (mapX >= 0 && mapX < ((uint8_t)myLevel->getWidth() * MAP_TILE_WIDTH) && mapY >= 0 && mapY < ((uint8_t)myLevel->getHeight() * MAP_TILE_HEIGHT) && !(drawX == 4 && drawY == 0) ) {
      #endif

        MapElement element = myLevel->getMapElement(mapX, mapY, false);

        if (element != MapElement::Wall) {


          // Is there an enemy standing here ?

          renderMapElement = true;

          for (uint8_t i = 0; i < NUMBER_OF_ENEMIES; ++i) {

            Enemy enemy = enemies[i];

            if (enemy.getEnabled() && enemy.getX() == mapX && enemy.getY() == mapY) {

              #ifdef USE_LARGE_MAP
              Sprites::drawSelfMasked(mapXOffset + (drawX * MAP_TILE_OFFSET), mapYOffset + (drawY * MAP_TILE_OFFSET), enemyMap, 0);
              #endif
              #ifndef USE_LARGE_MAP
              Sprites::drawSelfMasked(MAP_X_OFFSET + (drawX * MAP_TILE_OFFSET), MAP_Y_OFFSET + (drawY * MAP_TILE_OFFSET), enemyMap, 0);
              #endif
              renderMapElement = false;
              break;

            }

          }


          // Is there an item standing here ?

          if (renderMapElement) {

            for (uint8_t i = 0; i < NUMBER_OF_ITEMS; ++i) {

              Item item = items[i];

              if (item.getEnabled() && item.getX() == mapX && item.getY() == mapY) {

                #ifdef USE_LARGE_MAP
                Sprites::drawSelfMasked(mapXOffset + (drawX * MAP_TILE_OFFSET), mapYOffset + (drawY * MAP_TILE_OFFSET), itemMap, 0);
                #endif
                #ifndef USE_LARGE_MAP
                Sprites::drawSelfMasked(MAP_X_OFFSET + (drawX * MAP_TILE_OFFSET), MAP_Y_OFFSET + (drawY * MAP_TILE_OFFSET), itemMap, 0);
                #endif
                renderMapElement = false;
                break;

              }

            }

          }

          if (renderMapElement) {

            #ifdef USE_LARGE_MAP
            arduboy.fillRect(mapXOffset + (drawX * MAP_TILE_OFFSET), mapYOffset + (drawY * MAP_TILE_OFFSET), MAP_TILE_SIZE, MAP_TILE_SIZE, WHITE);
            #endif
            #ifndef USE_LARGE_MAP
            arduboy.fillRect(MAP_X_OFFSET + (drawX * MAP_TILE_OFFSET), MAP_Y_OFFSET + (drawY * MAP_TILE_OFFSET), MAP_TILE_SIZE, MAP_TILE_SIZE, WHITE);
            #endif

            switch (element) {

              case MapElement::LockedGate:
              case MapElement::LockedDoor:
              case MapElement::SelfLockingDoor:
                #ifdef USE_LARGE_MAP
                arduboy.drawLine(mapXOffset + (drawX * MAP_TILE_OFFSET) + 1, mapYOffset + (drawY * MAP_TILE_OFFSET) + 1, mapXOffset + (drawX * MAP_TILE_OFFSET) + 1, mapYOffset + (drawY * MAP_TILE_OFFSET) + 2, BLACK);
                #endif
                #ifndef USE_LARGE_MAP
                arduboy.drawLine(MAP_X_OFFSET + (drawX * MAP_TILE_OFFSET) + 1, MAP_Y_OFFSET + (drawY * MAP_TILE_OFFSET) + 1, MAP_X_OFFSET + (drawX * MAP_TILE_OFFSET) + 1, MAP_Y_OFFSET + (drawY * MAP_TILE_OFFSET) + 2, BLACK);
                #endif
                break;

              case MapElement::UnlockedDoor:
                #ifdef USE_LARGE_MAP
                arduboy.drawLine(mapXOffset + (drawX * MAP_TILE_OFFSET) + 2, mapYOffset + (drawY * MAP_TILE_OFFSET) + 1, mapXOffset + (drawX * MAP_TILE_OFFSET) + 2, mapYOffset + (drawY * MAP_TILE_OFFSET) + 2, BLACK);
                #endif
                #ifndef USE_LARGE_MAP
                arduboy.drawLine(MAP_X_OFFSET + (drawX * MAP_TILE_OFFSET) + 2, MAP_Y_OFFSET + (drawY * MAP_TILE_OFFSET) + 1, MAP_X_OFFSET + (drawX * MAP_TILE_OFFSET) + 2, MAP_Y_OFFSET + (drawY * MAP_TILE_OFFSET) + 2, BLACK);
                #endif
                break;

                default: break;

            }

          }

        }

      }

      ++drawX;

    }

    drawX = 0;
    ++drawY;

  }


  // Render player ..

  #ifdef USE_LARGE_MAP
  if (smallMap) {
    Sprites::drawExternalMask(mapXOffset + 10, mapYOffset + 15, playerMap, playerMap_Mask, 0, 0);
  }
  else {
    Sprites::drawExternalMask(mapXOffset + 55, mapYOffset + 25, playerMap, playerMap_Mask, 0, 0);
  }
  #endif
  #ifndef USE_LARGE_MAP
  Sprites::drawExternalMask(MAP_X_OFFSET + 10, MAP_Y_OFFSET + 15, playerMap, playerMap_Mask, 0, 0);
  #endif


  // Render statistics ..

  #ifdef USE_LARGE_MAP
  if (smallMap) {
  #endif
  font3x5.setCursor(70, 8);
  printStatistic(F("HP  "), player->getHitPoints());
  printStatistic(F("\nAP  "), player->getAttackPower());
  printStatistic(F("\nDF  "), player->getDefence());
  printStatistic(F("\nXP  "), player->getExperiencePoints());
  #ifdef USE_LARGE_MAP
  }
  #endif

}


/* -----------------------------------------------------------------------------------------------------------------------------
*  Print Statistic
* -----------------------------------------------------------------------------------------------------------------------------
*/
void printStatistic(const __FlashStringHelper * str, const uint8_t stat) {

  font3x5.print(str);
  if (stat < 10) font3x5.print(F(" "));
  font3x5.print(stat);

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Draw Direction Indicator
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void drawDirectionIndicator(Player *myHero) {

  Sprites::drawSelfMasked(DIRECTION_X_OFFSET, DIRECTION_Y_OFFSET, direction_images[(uint8_t)myHero->getDirection()], 0);

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Render Level Description
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void drawLevelDescription(Level *level) {

  font3x5.setCursor(80,44);
  font3x5.print(level->getTitleLine1());
  font3x5.print(F("\n"));
  font3x5.print(level->getTitleLine2());

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Render Enemy Hit Points Bar
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void drawEnemyHitPointsBar(uint8_t hitPoints, uint8_t hitPointsMax) {

  arduboy.drawCompressed(3, 49, fight_HP_bar_Mask, BLACK);
  arduboy.drawCompressed(3, 49, fight_HP_bar, WHITE);

  if(hitPoints == 0)
    return;

  const uint32_t hp = static_cast<uint32_t>(hitPoints) << 16; // U16x16
  const uint16_t hpMax = static_cast<uint16_t>(hitPointsMax) << 8; // U8x8
  const uint32_t divResult = hp / hpMax; // U8x8
  const uint32_t mulResult = divResult * (10 << 8); // U16x16
  const uint8_t resultAsInt = static_cast<uint8_t>(mulResult >> 16); // U8x0
  arduboy.drawRect(17, 54, resultAsInt, 2);

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Render Splash Screen
 *
 *  The player can choose between starting a new game or viewing the credits.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void displaySplash() {

  arduboy.drawCompressed(0, 0, frames_outside, WHITE);
  arduboy.drawCompressed(8, 4, splash, WHITE);

  if (splashStatus == SplashButtons::Play) {
    Sprites::drawSelfMasked(25, 54, hMarker, 0);
  }
  else {
    Sprites::drawSelfMasked(63, 54, hMarker, 0);
  }

  uint8_t buttons = arduboy.justPressedButtons();

  if (buttons & LEFT_BUTTON_MASK)              { splashStatus = SplashButtons::Play; }
  if (buttons & RIGHT_BUTTON_MASK)             { splashStatus = SplashButtons::About; }

  if (buttons & SELECT_BUTTON_MASK) {

    if (splashStatus == SplashButtons::Play) {
      gameState = GameState::InitGame;
    }
    else {
      gameState = GameState::About;
    }

  }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Display Garage Collective Logo
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void displayLogo() {

  arduboy.drawCompressed(0, 0, frames_outside, WHITE);
  arduboy.drawLine(42, 2, 68, 2, BLACK);
  arduboy.drawLine(42, 61, 68, 61, BLACK);

  arduboy.drawCompressed(42, 0, garCol, WHITE);

  font3x5.setCursor(8, 24);
  font3x5.print(F("CODE\nSIMON\nHOLMES"));

  font3x5.setCursor(90, 24);
  font3x5.print(F("     ART\n   CYRIL\nGUICHARD"));

  uint8_t buttons = arduboy.justPressedButtons();

  if (buttons & BACK_BUTTON_MASK) {
    gameState = GameState::Splash;
  }

}

