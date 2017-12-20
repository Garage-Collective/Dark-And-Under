#include <Arduboy2.h>

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Battle Loop
 *
 *  GameStates:
 *
 *  Battle_EnemyAttacks_Init -    Displays the initial '{enemy} attacks !' message and prepares for and enemy attck.
 *  Battle_EnemyAttacks -         Displays the dice animation and inflicts damage to the player.
 *  Battle_PlayerDecides -        Presents the players battle options including attack, defend, cast a spell ..
 *  Battle_PlayerAttacks -        Throws the dice and inflicts damage on the enemy.
 *  Battle_PlayerDefends -        Inflicts 1 point of damage on the enemy and randomly gains player hit points.
 *  Battle_PlayerCastsSpell -     Inflicts 5 points of damage on the enemy.
 *  Battle_EnemyDies -            Handles and end of battel where the enemy dies.
 *
 *  Returns delay in milliseconds to wait.
 *
 * -----------------------------------------------------------------------------------------------------------------------------
 */
uint16_t battleLoop() {

  uint16_t delayLength = FIGHT_DELAY;

  drawPlayerVision(&myHero, &myLevel);
  Sprites::drawSelfMasked(DIRECTION_X_OFFSET, DIRECTION_Y_OFFSET, fight_icon, 0);

  font3x5.setCursor(80,44);

  switch (gameState) {


    case GameState::Battle_EnemyAttacks_Init:  // ----------------------------------------------------------------------------------------------------------------------------------
      font3x5.print(getEnemyName(enemies[attackingEnemyIdx].getEnemyType()));
      font3x5.print(F("\nATTACKS!"));
      gameState = GameState::Battle_EnemyAttacks;
      break;


    case GameState::Battle_EnemyAttacks:  // ---------------------------------------------------------------------------------------------------------------------------------------
      {
        arduboy.drawCompressed(12, 12, fight_scratch_Mask, BLACK);
        arduboy.drawCompressed(12, 12, fight_scratch, WHITE);

        uint8_t hpLoss = random(0, ENEMY_MAX_ATTACK);
        font3x5.print(F("YOU TAKE\n"));
        font3x5.print(hpLoss);
        font3x5.print(F(" DAMAGE!"));
        font3x5.setCursor(33, 26);
        font3x5.print(hpLoss);
        myHero.setHitPoints(myHero.getHitPoints() > hpLoss ? myHero.getHitPoints() - hpLoss : 0);
        gameState = GameState::Battle_PlayerDecides;

        if (myHero.getHitPoints() == 0)  gameState = GameState::Battle_PlayerDies;
      }

      break;


    case GameState::Battle_EnemyDies:  // ---------------------------------------------------------------------------------------------------------------------------------------
      {
        uint8_t xp = enemies[attackingEnemyIdx].getExperiencePoints();

        arduboy.drawCompressed(18, 14, enemy_defeated_Mask, BLACK);
        arduboy.drawCompressed(18, 14, enemy_defeated, WHITE);

        font3x5.setCursor(32 + (xp < 10 ? 2 : 0), 26);
        font3x5.print(xp);
        myHero.setExperiencePoints(myHero.getExperiencePoints() + xp);

        if (myHero.getExperiencePoints() >= LEVEL_UP) {
          myHero.setExperiencePoints(myHero.getExperiencePoints() - LEVEL_UP);
          gameState = GameState::LevelUp;
        }
        else {
          gameState = GameState::Move;
        }

      }
      break;


    case GameState::Battle_PlayerDecides:  // -----------------------------------------------------------------------------------------------------------------------------------
      {
        uint8_t buttons = arduboy.justPressedButtons();

        bool fightButtons[4];
        fightButtons[(uint8_t)FightButtons::Attack] = true;
        fightButtons[(uint8_t)FightButtons::Shield] = true;
        fightButtons[(uint8_t)FightButtons::Magic] = (myHero.getInventoryCount(ItemType::Scroll) > 0);
        fightButtons[(uint8_t)FightButtons::Potion] = (myHero.getInventoryCount(ItemType::Potion) > 0);

        arduboy.drawCompressed(80, 44, fight_actions_1, WHITE);
        arduboy.drawCompressed(91, 44, fight_actions_2, WHITE);
        if (fightButtons[(uint8_t)FightButtons::Magic])    { arduboy.drawCompressed(102, 44, fight_actions_3, WHITE); }
        if (fightButtons[(uint8_t)FightButtons::Potion])   { arduboy.drawCompressed(113, 44, fight_actions_4, WHITE); }
        Sprites::drawSelfMasked(81 + (((uint8_t)fightButton) * 11), 56, icnSelect, 0);

        if ((buttons & LEFT_BUTTON_MASK) && (uint8_t)fightButton > 0) {

          for (int8_t i = (uint8_t)fightButton - 1; i >=0; --i) {
            if (fightButtons[i]) {
              fightButton = (FightButtons)i;
              break;
            }
          }

        }

        else if (buttons & RIGHT_BUTTON_MASK) {

          for (uint8_t i = (uint8_t)fightButton + 1; i < (uint8_t)FightButtons::Count; ++i) {
            if (fightButtons[i]) {
              fightButton = (FightButtons)i;
              break;
            }
          }

        }

        else if (buttons & BACK_BUTTON_MASK)  {
          savedState = gameState;
          gameState = GameState::InventorySelect;
        }

        else if (buttons & SELECT_BUTTON_MASK)  {

          switch (fightButton) {

            case (FightButtons::Attack):
              gameState = GameState::Battle_PlayerAttacks;
              break;

            case (FightButtons::Shield):
              gameState = GameState::Battle_PlayerDefends;
              break;

            case (FightButtons::Magic):
              gameState = GameState::Battle_PlayerCastsSpell;
              fightButton = FightButtons::Attack;
              break;

            case (FightButtons::Potion):
              myHero.setHitPoints(myHero.getHitPoints() + INVENTORY_POTION_HP_INC);
              myHero.setInventory(myHero.getSlotNumber(ItemType::Potion), ItemType::None);
              fightButton = FightButtons::Attack;
              break;

            default:
              break;

          }

        }

      }

      delayLength = 0;
      break;


    case GameState::Battle_PlayerAttacks:  // ---------------------------------------------------------------------------------------------------------------------------------------
      {
        arduboy.drawCompressed(19, 19, fight_hero_strike_Mask, BLACK);
        arduboy.drawCompressed(19, 19, fight_hero_strike, WHITE);

        uint8_t hpLoss = random(1, myHero.getAttackPower() + 1);
        font3x5.print(F("YOU DEAL\n"));
        font3x5.print(hpLoss);
        font3x5.print(F(" DAMAGE!\n"));
        font3x5.setCursor(32, 24);
        font3x5.print(hpLoss);
        damageEnemy(attackingEnemyIdx, hpLoss);

        gameState = GameState::Battle_EnemyDies;
        if (enemies[attackingEnemyIdx].getEnabled()) { gameState = GameState::Battle_EnemyAttacks_Init; }
      }

      break;


    case GameState::Battle_PlayerDefends:  // ---------------------------------------------------------------------------------------------------------------------------------------

      arduboy.drawCompressed(12, 15, fight_hero_shield_Mask, BLACK);
      arduboy.drawCompressed(12, 15, fight_hero_shield, WHITE);

      {
        uint8_t maxHP = enemies[attackingEnemyIdx].getAttackPower() - myHero.getDefence();
        if (maxHP < 2) maxHP = 2;
        if (maxHP > 10) maxHP = 10;
        uint8_t hpLoss = random(0, maxHP);

        font3x5.print(F("TAKE "));
        font3x5.print(hpLoss);
        font3x5.print(F(" DMG\n"));
        font3x5.print(F("DEAL 1 DMG\n"));
        font3x5.setCursor(17, 35);
        font3x5.print(hpLoss);

        myHero.setHitPoints(myHero.getHitPoints() - hpLoss);
        damageEnemy(attackingEnemyIdx, 1);

        gameState = GameState::Battle_EnemyDies;
        if (enemies[attackingEnemyIdx].getEnabled()) gameState = GameState::Battle_PlayerDecides;
      }

      break;


    case GameState::Battle_PlayerCastsSpell:  // ---------------------------------------------------------------------------------------------------------------------------------------

        arduboy.drawCompressed(12, 15, fight_hero_spell_Mask, BLACK);
        arduboy.drawCompressed(12, 15, fight_hero_spell, WHITE);

        damageEnemy(attackingEnemyIdx, PLAYER_CASTS_SPELL);
        myHero.setInventory(myHero.getSlotNumber(ItemType::Scroll), ItemType::None);

        gameState = GameState::Battle_EnemyDies;
        if (enemies[attackingEnemyIdx].getEnabled()) gameState = GameState::Battle_EnemyAttacks_Init;

        break;

    default: break;

  }

  drawEnemyHitPointsBar(enemies[attackingEnemyIdx].getHitPoints(), enemies[attackingEnemyIdx].getHitPointsMax());
  return delayLength;

}

void damageEnemy(uint8_t attackingEnemyIdx, uint8_t hpLoss) {

    if (enemies[attackingEnemyIdx].getHitPoints() > hpLoss) {
        enemies[attackingEnemyIdx].setHitPoints(enemies[attackingEnemyIdx].getHitPoints() - hpLoss);
    }
    else {
        enemies[attackingEnemyIdx].setHitPoints(0);
        enemies[attackingEnemyIdx].setEnabled(false);
    }

}
