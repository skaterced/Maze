#ifndef weapon_h
#define weapon_h

#include "globals.h"

#define DROP_RAND 30

const unsigned char PROGMEM bonus_bitmap[] = {
// width, height,
8, 8,
0xf7,0xff,0xf7,0xa2,0xf7,0xff,0xf7,0xff,
0xff,0xff,0x85,0xb1,0x85,0xff,0xff,0xff
};

class Bomb {
  public :
    uint8_t x,y;
    uint8_t range;
    uint8_t counter; // decreasing to 0 means exploding, then inactive  
    Bomb(void) {
      this->x=0;
      this->y=0;
      //this->range=1;  //-> robot
      this->counter=0;            
    }
    void draw(void){
      arduboy.drawPixel(x+3+leftBorder,y+3+upBorder,(timer&0x04)>>2);
      arduboy.drawCircle(x+4+leftBorder,y+4+upBorder,2,0);      
    }
};

class Bonus{
  public:
    uint8_t x,y;
    uint8_t type;
    Bonus(uint8_t x_, uint8_t y_, uint8_t t){
      x=x_;
      y=y_;
      type=t;
    }
    void draw(void){
      switch(type){
        case 0:
        break;
        default:
          Sprites::drawOverwrite(x + leftBorder, y + upBorder, bonus_bitmap, type-1);
        break;
      }
    }
};

Bonus bonus1(11,21,0);

bool drop(uint8_t x_, uint8_t y_){
  if (random(100)<DROP_RAND){
    bonus1.x=x_;
    bonus1.y=y_;
    bonus1.type=2;
    return true;  
  }
  return false;
}

#endif
