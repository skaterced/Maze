#ifndef weapon_h
#define weapon_h

#include "globals.h"

#define NB_BOMB_MAX 3
#define BOMB_DEFAULT_T 7

class Bomb {
  public :
    int x,y;
    uint8_t range;
    uint8_t counter; //1 means exploding, 0 means inactive  
    Bomb(void) {
      this->x=0;
      this->y=0;
      this->range=1;  //
      this->counter=0;            
    }
    void draw(void){
      arduboy.drawPixel(x+3+leftBorder,y+3+upBorder,(timer&0x04)>>2);
      arduboy.drawCircle(x+4+leftBorder,y+4+upBorder,2,0);      
    }
};

#endif
