#ifndef weapon_h
#define weapon_h

#include "globals.h"

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

#endif
