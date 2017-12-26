#include "src/ArduboyExt/Arduboy2.h"

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Inventory loop.
 *
 *  Controls the management and use of the inventory panel.  As a player transitions to this panel, the current GameState is
 *  saved in a variable named 'savedState' which allows the player to exit the panel and return to whence they came.
 *
 *  GameStates:
 *
 *  InventorySelect     - navigate through the five inventory slots.
 *  InventoryAction     - after selecting an item, the user can use or delete an item.
 *
 *  Returns delay in milliseconds to wait.
 *
 * -----------------------------------------------------------------------------------------------------------------------------
 */
uint16_t inventoryLoop() {

  arduboy.drawCompressed(0, 0, frames_outside, WHITE);
  arduboy.drawCompressed(66, 4, frames_inside, WHITE);
  arduboy.drawCompressed(4, 4, inv_background, WHITE);
  drawDirectionIndicator(&myHero);

  #ifdef USE_LARGE_MAP
  drawMapAndStatistics(&myHero, &myLevel, true);
  #endif
  #ifndef USE_LARGE_MAP
  drawMapAndStatistics(&myHero, &myLevel);
  #endif

  #ifndef SAVE_GAME
  for (uint8_t i = 0; i < 5; ++i) {

    if (myHero.getInventory(i) != ItemType::None) {

      Point inventoryCoords = inventory_Coords[i];

      arduboy.fillRect(inventoryCoords.x, inventoryCoords.y, 14, 16, BLACK);
      arduboy.drawCompressed(inventoryCoords.x, inventoryCoords.y, inventory_images[(uint8_t)myHero.getInventory(i)], WHITE);

    }

  }
  #endif
  #ifdef SAVE_GAME
  for (uint8_t i = 0; i < 3; ++i) {

    if (myHero.getInventory(i) != ItemType::None) {

      Point inventoryCoords = inventory_Coords[i];

      arduboy.fillRect(inventoryCoords.x, inventoryCoords.y, 14, 16, BLACK);
      arduboy.drawCompressed(inventoryCoords.x, inventoryCoords.y, inventory_images[(uint8_t)myHero.getInventory(i)], WHITE);

    }

  }
  #endif


  // Render selector ..

  Point inventoryCoords = inventory_Coords[inventory_selection];
  arduboy.drawCompressed(inventoryCoords.x + 3, inventoryCoords.y + 11, inv_select, BLACK);
  uint8_t buttons = arduboy.justPressedButtons();


  // Render AP and DF values ..

  font3x5.setTextColor(BLACK);
  font3x5.setCursor(19, 35);
  font3x5.print(myHero.getAttackPower());
  font3x5.setCursor(19, 45);
  font3x5.print(myHero.getDefence());
  font3x5.setTextColor(WHITE);


  switch (gameState) {

    case GameState::InventorySelect:

      #ifndef SAVE_GAME
      if ((buttons & LEFT_BUTTON_MASK) && inventory_selection > 0)           { --inventory_selection; }
      else if ((buttons & RIGHT_BUTTON_MASK) && inventory_selection < 2)     { ++inventory_selection; }
      #endif

      #ifdef SAVE_GAME
      if ((buttons & LEFT_BUTTON_MASK) && inventory_selection > 0)           { --inventory_selection; }
      else if ((buttons & RIGHT_BUTTON_MASK) && inventory_selection < 4)     { ++inventory_selection; }
      #endif

      else if (buttons & BACK_BUTTON_MASK)                                   { gameState = savedState;}

      else if (buttons & SELECT_BUTTON_MASK) {

        #ifdef SAVE_GAME
        if (inventory_selection == 3) {
          saveGame();
          font3x5.setCursor(90, 44);
          font3x5.print(F(" GAME\nSAVED!"));
          return ITEM_DELAY;
        }
        else if (inventory_selection == 4) {
          if (getLevel() == 255) {
            font3x5.setCursor(86, 44);
            font3x5.print(F("NO GAME\n SAVED"));
          }
          else {
            restoreGame();
            font3x5.setCursor(86, 44);
            font3x5.print(F("  GAME\nRESTORED"));
          }
          return ITEM_DELAY;
        }
        else {
        #endif

          if (myHero.getInventory(inventory_selection) != ItemType::None) {
            gameState = GameState::InventoryAction;
          }

        #ifdef SAVE_GAME
        }
        #endif

      }

      break;

    case GameState::InventoryAction:

      arduboy.drawCompressed(82 + (inventory_action == INVENTORY_ACTION_DROP ? 12 : 0), 56, inv_select, WHITE);
      Sprites::drawOverwrite(81, 45, inv_hand, 0);
      Sprites::drawOverwrite(93, 45, inv_trash, 0);

      if ((buttons & LEFT_BUTTON_MASK) && inventory_action > INVENTORY_ACTION_USE)            { --inventory_action; }
      else if ((buttons & RIGHT_BUTTON_MASK) && inventory_action < INVENTORY_ACTION_DROP)     { ++inventory_action; }
      else if (buttons & BACK_BUTTON_MASK)                                                    { gameState = GameState::InventorySelect;}
      else if (buttons & SELECT_BUTTON_MASK) {

        if (inventory_action == INVENTORY_ACTION_USE) {

          switch (myHero.getInventory(inventory_selection)) {

            case ItemType::Key:

              for (uint8_t i = 0; i < NUMBER_OF_DOORS; ++i) {

                int16_t deltaX = doors[i].getX() - myHero.getX();
                int16_t deltaY = doors[i].getY() - myHero.getY();

                if (doors[i].getEnabled() && (doors[i].getItemType() == ItemType::LockedDoor || doors[i].getItemType() == ItemType::LockedGate || doors[i].getItemType() == ItemType::SelfLockingDoor) &&
                    absT(deltaX) <= 1 && absT(deltaY) <= 1) {

                  if (doors[i].getItemType() == ItemType::LockedGate)         doors[i].setEnabled(false);
                  if (doors[i].getItemType() == ItemType::LockedDoor)         doors[i].setItemType(ItemType::UnlockedDoor);
                  if (doors[i].getItemType() == ItemType::SelfLockingDoor)    doors[i].setEnabled(false);
                  
                  myHero.setInventory(inventory_selection, ItemType::None);
                  inventory_action = INVENTORY_ACTION_USE;
                  gameState = GameState::InventorySelect;

                }

              }
              break;

            case ItemType::Potion:
              myHero.setHitPoints(myHero.getHitPoints() + INVENTORY_POTION_HP_INC);
              myHero.setInventory(inventory_selection, ItemType::None);
              inventory_action = INVENTORY_ACTION_USE;
              gameState = GameState::InventorySelect;
              break;

            default: break;

          }

        }

        if (inventory_action == INVENTORY_ACTION_DROP) {

          bool spaceOccupied = false;
          uint8_t itemIndex = 0;

          for (uint8_t i = 0; i < NUMBER_OF_ITEMS; ++i) {

            Item item = items[i];

            if (item.getItemType() != ItemType::None) {

              if (item.getX() == myHero.getX() && item.getY() == myHero.getY()) {

                spaceOccupied = true;
                break;

              }

            }
            else {

              itemIndex = i;

            }

          }

          if (spaceOccupied) {

            font3x5.setCursor(95, 44);
            font3x5.print(F("SPACE\nOCCUPIED!"));
            return ITEM_DELAY;

          }
          else {

            items[itemIndex].setEnabled(true);
            items[itemIndex].setItemType((ItemType)(uint8_t)myHero.getInventory(inventory_selection));
            items[itemIndex].setX(myHero.getX());
            items[itemIndex].setY(myHero.getY());

            myHero.setInventory(inventory_selection, ItemType::None);
            inventory_action = INVENTORY_ACTION_USE;
            gameState = GameState::InventorySelect;

          }

        }

      }

      break;

    default: break;

  }

  return 0;

}
