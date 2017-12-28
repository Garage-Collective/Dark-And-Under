#include <Arduino.h>
#include <Print.h>
#include "../Arduboy/Sprites.h"
#include "Font3x5.h"

#define FONT3X5_WIDTH 3
#define FONT3X5_HEIGHT 6

const uint8_t PROGMEM font_images[] = {
3, 7,
0x1F, 0x05, 0x1F, // A
0x1F, 0x15, 0x1B, 
0x1F, 0x11, 0x11, 
0x1F, 0x11, 0x0E, 
0x1F, 0x15, 0x11, 
0x1F, 0x05, 0x01, 
0x1F, 0x11, 0x1D, 
0x1F, 0x04, 0x1F, 
0x00, 0x1F, 0x00, 
0x10, 0x10, 0x1F, 
0x1F, 0x04, 0x1B, 
0x1F, 0x10, 0x10, 
0x1F, 0x06, 0x1F, 
0x1F, 0x01, 0x1F, 
0x1F, 0x11, 0x1F, 
0x1F, 0x05, 0x07, 
0x1F, 0x31, 0x1F, 
0x1F, 0x05, 0x1B,  
0x17, 0x15, 0x1D,  
0x01, 0x1F, 0x01,  
0x1F, 0x10, 0x1F,  
0x0F, 0x10, 0x0F,  
0x1F, 0x0C, 0x1F,  
0x1B, 0x04, 0x1B,  
0x07, 0x1C, 0x07,  
0x19, 0x15, 0x13, // Z
0x1F, 0x11, 0x1F, // 0  
0x00, 0x1F, 0x00,
0x1D, 0x15, 0x17,  
0x11, 0x15, 0x1F,  
0x07, 0x04, 0x1F,  
0x17, 0x15, 0x1D,  
0x1F, 0x15, 0x1D,  
0x01, 0x01, 0x1F,  
0x1F, 0x15, 0x1F,  
0x17, 0x15, 0x1F, // 9
0x00, 0x17, 0x00, // !
0X00, 0x20, 0x00, // .
};


Font3x5::Font3x5() {

  _lineHeight = FONT3X5_HEIGHT + 2;
  _letterSpacing = 1;

  _cursorX = _cursorY = _baseX = 0;
  _textColor = 1;

}

size_t Font3x5::write(uint8_t c) {

  if (c == '\n')      { _cursorX = _baseX; _cursorY += _lineHeight; }
  else {

    printChar(c, _cursorX, _cursorY);
    _cursorX += FONT3X5_WIDTH + _letterSpacing;

  }

  return 1;

}

#define CHAR_EXCLAMATION 33
#define CHAR_PERIOD 46
#define CHAR_LETTER_A 65
#define CHAR_LETTER_Z 90
#define CHAR_NUMBER_0 48
#define CHAR_NUMBER_9 57

#define FONT_EXCLAMATION_INDEX 36
#define FONT_PERIOD_INDEX 37
#define FONT_NUMBER_INDEX 26

void Font3x5::printChar(const char c, const int8_t x, int8_t y) {

  int8_t idx = -1;

  // no need to draw at all of we're offscreen
  //if (x + FONT3X5_WIDTH <= 0 || x > sWidth - 1 || y + FONT3X5_HEIGHT <= 0 || y > sHeight - 1) return;

  ++y;

  switch (c) {
    
    case CHAR_LETTER_A ... CHAR_LETTER_Z:
      idx = c - CHAR_LETTER_A;
      break;

    case CHAR_NUMBER_0 ... CHAR_NUMBER_9:
      idx = c - CHAR_NUMBER_0 + FONT_NUMBER_INDEX;
      break;
      
    case CHAR_EXCLAMATION:
      idx = FONT_EXCLAMATION_INDEX;
      break;
      
    case CHAR_PERIOD:
      idx = FONT_PERIOD_INDEX;
      break;

  }

  if (idx > -1) {
    
    if (_textColor == WHITE) {
      Sprites::drawSelfMasked(x, y, font_images, idx);
    }
    else {
      Sprites::drawErase(x, y, font_images, idx);
    }

  }

}

void Font3x5::setCursor(const int8_t x, const int8_t y) {
  _cursorX = _baseX = x;
  _cursorY = y;
}

void Font3x5::setTextColor(const uint8_t color){
  _textColor = color;
}

void Font3x5::setHeight(const uint8_t color){
  _lineHeight = color;
}
