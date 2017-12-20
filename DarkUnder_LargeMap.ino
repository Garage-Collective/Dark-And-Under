#ifdef USE_LARGE_MAP
#include <Arduboy2.h>

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Display Large Map.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void displayLargeMap() {

  arduboy.drawCompressed(0, 0, frames_outside, WHITE);
  drawMapAndStatistics(&myHero, &myLevel, false);
  uint8_t buttons = arduboy.justPressedButtons();

  if (buttons & BACK_BUTTON_MASK) { gameState = savedState; }

}
#endif
