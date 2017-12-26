#include "src/ArduboyExt/Arduboy2.h"

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Item loop.
 *
 *  Controls the selection or discarding of an item the player has stumbled over.  If a user choses to ignore an item found in
 *  the maze, the GameState is updated to ItemIgnore which will allow them to progress beyond the item.  If th user returns to
 *  the same position in the maze, they will be re-prompted to collect / discard the same item.
 *
 *  The player can press the BACK_BUTTON (as defined in Enums.h) to bring up the inventory management dialogue if they need to
 *  delete an existing inventory item to make space for the new item.  The current GameState is retained in the 'savedState'
 *  field allowing for control to return back to this action after the dialogue is dismissed.
 *
 *  Returns delay in milliseconds to wait.
 *
 * -----------------------------------------------------------------------------------------------------------------------------
 */
uint16_t itemLoop() {

    drawPlayerVision(&myHero, &myLevel);
    arduboy.drawCompressed(72 + (itemAction == INVENTORY_ACTION_DROP ? 12 : 0), 56, inv_select, WHITE);

    Sprites::drawOverwrite(71, 45, inv_hand, 0);
    Sprites::drawOverwrite(83, 45, inv_trash, 0);
    arduboy.drawLine(66, 43, 66, 59);

    uint8_t buttons = arduboy.justPressedButtons();

    if ((buttons & LEFT_BUTTON_MASK) && itemAction > ITEM_ACTION_USE)             { --itemAction; }
    else if ((buttons & RIGHT_BUTTON_MASK) && itemAction < ITEM_ACTION_DROP)      { ++itemAction; }


    // Return to the previous game state if the user presses the back button ..

    if (buttons & BACK_BUTTON_MASK) {
      savedState = gameState;
      gameState = GameState::InventorySelect;
    }


    // Handle the use of the selected item ..

    if (buttons & SELECT_BUTTON_MASK) {

      if (itemAction == ITEM_ACTION_USE) {

        ItemType inventoryType = items[savedItem].getItemType();
        int8_t inventorySlot = myHero.getConsumableSlot();

        if (inventoryType == ItemType::Shield || inventoryType == ItemType::Sword) {

          if (inventoryType == ItemType::Shield)  myHero.setDefence(myHero.getDefence() + 1);
          if (inventoryType == ItemType::Sword)   myHero.setAttackPower(myHero.getAttackPower() + 1);

          items[savedItem].setEnabled(false);
          gameState = GameState::Move;

        }
        else {
          if (inventorySlot >= 0) {

            myHero.setInventory(inventorySlot, inventoryType);
            items[savedItem].setEnabled(false);
            gameState = GameState::Move;

          }
          else {

            font3x5.setCursor(95, 44);
            font3x5.print(F("NO INV\nSLOTS!"));
            return ITEM_DELAY;

          }

        }


      }

      if (itemAction == ITEM_ACTION_DROP)  {

        gameState = GameState::ItemIgnore;

      }

    }

    return 0;

  }