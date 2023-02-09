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

  drawPlayerVision(&myHero, &myLevel);
  Sprites::drawSelfMasked(DIRECTION_X_OFFSET, DIRECTION_Y_OFFSET, fight_icon, 0);

  font3x5.setCursor(80,44);

  const GameState priorState = gameState;
  switch (priorState) {
    case GameState::Battle_EnemyAttacks_Init:
      gameState = battleEnemyAttacksInit();
      break;

    case GameState::Battle_EnemyAttacks:
      gameState = battleEnemyAttacks();
      break;

    case GameState::Battle_EnemyDies:
      gameState = battleEnemyDies();
      break;

    case GameState::Battle_PlayerDecides:
      gameState = battlePlayerDecides();
      break;

    case GameState::Battle_PlayerAttacks:
      gameState = battlePlayerAttacks();
      break;

    case GameState::Battle_PlayerDefends:
      gameState = battlePlayerDefends();
      break;

    case GameState::Battle_PlayerCastsSpell:
      gameState = battlePlayerCastsSpell();
      break;

    default:
      break;
  }

  drawEnemyHitPointsBar(enemies[attackingEnemyIdx].getHitPoints(), enemies[attackingEnemyIdx].getHitPointsMax());
  return (priorState != GameState::Battle_PlayerDecides) ? FIGHT_DELAY : 0;

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

GameState battleEnemyAttacksInit(void) {

  font3x5.print(getEnemyName(enemies[attackingEnemyIdx].getEnemyType()));
  font3x5.print(F("\nATTACKS!"));
  return GameState::Battle_EnemyAttacks;
  
}

GameState battleEnemyAttacks(void) {

  arduboy.drawCompressed(12, 12, fight_scratch_Mask, BLACK);
  arduboy.drawCompressed(12, 12, fight_scratch, WHITE);

  uint8_t hpLoss = random(0, ENEMY_MAX_ATTACK);
  font3x5.print(F("YOU TAKE\n"));
  font3x5.print(hpLoss);
  font3x5.print(F(" DAMAGE!"));
  font3x5.setCursor(33, 26);
  font3x5.print(hpLoss);
  myHero.setHitPoints(myHero.getHitPoints() > hpLoss ? myHero.getHitPoints() - hpLoss : 0);

  return (myHero.getHitPoints() > 0)
  ? GameState::Battle_PlayerDecides
  :  GameState::Battle_PlayerDies;
}

GameState battleEnemyDies(void)
{
  arduboy.drawCompressed(18, 14, enemy_defeated_Mask, BLACK);
  arduboy.drawCompressed(18, 14, enemy_defeated, WHITE);

  uint8_t xp = enemies[attackingEnemyIdx].getExperiencePoints();
  font3x5.setCursor(32 + (xp < 10 ? 2 : 0), 26);
  font3x5.print(xp);
  myHero.setExperiencePoints(myHero.getExperiencePoints() + xp);

  if (myHero.getExperiencePoints() >= LEVEL_UP) {
    myHero.setExperiencePoints(myHero.getExperiencePoints() - LEVEL_UP);
    return GameState::LevelUp;
  }
  else {
    return GameState::Move;
  }
}

const uint8_t * const fight_actions[] PROGMEM =
{
  fight_actions_1,
  fight_actions_2,
  fight_actions_3,
  fight_actions_4,
};

GameState battlePlayerDecides(void)
{
  uint8_t buttons = arduboy.justPressedButtons();

  bool fightButtons[4];
  fightButtons[(uint8_t)FightButtons::Attack] = true;
  fightButtons[(uint8_t)FightButtons::Shield] = true;
  fightButtons[(uint8_t)FightButtons::Magic] = (myHero.getInventoryCount(ItemType::Scroll) > 0);
  fightButtons[(uint8_t)FightButtons::Potion] = (myHero.getInventoryCount(ItemType::Potion) > 0);
  
  constexpr uint8_t fightButtonsCount = sizeof(fightButtons) / sizeof(fightButtons[0]);

  for(uint8_t i = 0; i < fightButtonsCount; ++i) {

    if(fightButtons[i])
      arduboy.drawCompressed(80 + 11 * i, 44, static_cast<const uint8_t *>(pgm_read_ptr(&fight_actions[i])), WHITE);

  }
  
  Sprites::drawSelfMasked(81 + (((uint8_t)fightButton) * 11), 56, icnSelect, 0);

  if ((buttons & LEFT_BUTTON_MASK) && (uint8_t)fightButton > 0) {

    for (uint8_t i = (uint8_t)fightButton - 1; i < fightButtonsCount; --i) {
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
    savedState = GameState::Battle_PlayerDecides;
    return GameState::InventorySelect;
  }
  else if (buttons & SELECT_BUTTON_MASK)  {

    switch (fightButton) {

      case (FightButtons::Attack):
        return GameState::Battle_PlayerAttacks;

      case (FightButtons::Shield):
        return GameState::Battle_PlayerDefends;

      case (FightButtons::Magic):
        fightButton = FightButtons::Attack;
        return GameState::Battle_PlayerCastsSpell;

      case (FightButtons::Potion):
        fightButton = FightButtons::Attack;
        myHero.setHitPoints(myHero.getHitPoints() + INVENTORY_POTION_HP_INC);
        myHero.setInventory(myHero.getSlotNumber(ItemType::Potion), ItemType::None);
        break;

      default:
        break;

    }

  }

  return GameState::Battle_PlayerDecides;

}

GameState battlePlayerAttacks(void) {

  arduboy.drawCompressed(19, 19, fight_hero_strike_Mask, BLACK);
  arduboy.drawCompressed(19, 19, fight_hero_strike, WHITE);

  uint8_t hpLoss = random(1, myHero.getAttackPower() + 1);
  damageEnemy(attackingEnemyIdx, hpLoss);
  
  font3x5.print(F("YOU DEAL\n"));
  font3x5.print(hpLoss);
  font3x5.print(F(" DAMAGE!\n"));
  font3x5.setCursor(32, 24);
  font3x5.print(hpLoss);

  return (enemies[attackingEnemyIdx].getEnabled())
  ? GameState::Battle_EnemyAttacks_Init
  : GameState::Battle_EnemyDies;
  
}

GameState battlePlayerDefends(void) {

  arduboy.drawCompressed(12, 15, fight_hero_shield_Mask, BLACK);
  arduboy.drawCompressed(12, 15, fight_hero_shield, WHITE);

  const uint8_t enemyAttack = enemies[attackingEnemyIdx].getAttackPower();
  const uint8_t playerDefence = myHero.getDefence();

  uint8_t maxHP = (enemyAttack > playerDefence) ? (enemyAttack - playerDefence) : 2;
  if(maxHP < 2) maxHP = 2;
  if(maxHP > 10) maxHP = 10;
  const uint8_t hpLoss = random(0, maxHP);

  font3x5.print(F("TAKE "));
  font3x5.print(hpLoss);
  font3x5.print(F(" DMG\n"));
  font3x5.print(F("DEAL 1 DMG\n"));
  font3x5.setCursor(17, 35);
  font3x5.print(hpLoss);

  myHero.setHitPoints(myHero.getHitPoints() - hpLoss);
  damageEnemy(attackingEnemyIdx, 1);

  return (enemies[attackingEnemyIdx].getEnabled())
  ? GameState::Battle_PlayerDecides
  : GameState::Battle_EnemyDies;
  
}

GameState battlePlayerCastsSpell(void) {

  arduboy.drawCompressed(12, 15, fight_hero_spell_Mask, BLACK);
  arduboy.drawCompressed(12, 15, fight_hero_spell, WHITE);

  damageEnemy(attackingEnemyIdx, PLAYER_CASTS_SPELL);
  myHero.setInventory(myHero.getSlotNumber(ItemType::Scroll), ItemType::None);

  return (enemies[attackingEnemyIdx].getEnabled())
  ? GameState::Battle_EnemyAttacks_Init
  : GameState::Battle_EnemyDies;
  
}