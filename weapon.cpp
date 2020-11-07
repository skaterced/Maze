
//#include <Arduboy2.h>
#include "globals.h"
#include "weapon.h"


Bomb::Bomb(void) {
  this->x=0;
  this->y=0;
  //this->range=1;  //-> robot
  this->counter=0;            
}

void Bomb::draw(void){
  arduboy.drawPixel(x+3+leftBorder,y+3+upBorder,(timer&0x04)>>2);
  arduboy.drawCircle(x+4+leftBorder,y+4+upBorder,2,0);      
}


void Bonus::draw(void){
    Sprites::drawOverwrite(x + leftBorder, y + upBorder, bonus_bitmap, 0);
}


