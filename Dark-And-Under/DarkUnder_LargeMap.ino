#ifdef USE_LARGE_MAP
#include "src/Arduboy/Arduboy2.h"

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Display Large Map.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void displayLargeMap() {

  drawFrames();
  drawMapAndStatistics(&myHero, &myLevel, false);
  uint8_t buttons = arduboy.justPressedButtons();

  if (buttons & BACK_BUTTON_MASK) { gameState = savedState; }

}
#endif
