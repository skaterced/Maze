#ifndef weapon_h
#define weapon_h

//#include <Arduboy2.h>
//#include "globals.h"

const unsigned char PROGMEM bonus_bitmap[] = {
// width, height,
8, 8,
0xf7,0xff,0xf7,0xa2,0xf7,0xff,0xf7,0xff
};

class Bomb {
  public :
    uint8_t x,y;
    uint8_t range;
    uint8_t counter; // decreasing to 0 means exploding, then inactive  
    Bomb(void) ;
    void draw(void);
};

class Bonus{
  uint8_t x,y;
  uint8_t type;
  void Bomb(uint8_t x_, uint8_t y_, uint8_t t);
  void draw(void);
};

#endif
