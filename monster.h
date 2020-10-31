#ifndef monster_h
#define weapon_h

#include "globals.h"
#include "robot.h"

const unsigned char PROGMEM monstre_bitmap[] = {
// width, height,
8, 8,
0x0f,0xc7,0x03,0x81,0x81,0x03,0xc7,0x0f};

class Monster {
  public :
    int x,y;
    uint8_t dir;
    //uint8_t counter; // decreasing to 0 means exploding, then inactive  
    Monster(uint8_t x, uint8_t y) {
      this->x=x;
      this->y=y;
      //this->range=1;  //-> robot
      this->dir=0;            
    }
    void draw(void){
      if (ROBOT_DIED!=dir){
        Sprites::drawOverwrite(x+leftBorder, y+upBorder, monstre_bitmap, 0);
        if (3!=dir)
          arduboy.drawPixel(x+3+leftBorder+((timer&0x10)>>4),y+4+upBorder,1);
        arduboy.drawPixel(x+2+leftBorder+((timer&0x08)>>3)*3,y+7+upBorder,1);
      }
      else {
        Sprites::drawOverwrite(x+leftBorder, y+upBorder, robots, 2*BETWEEN_ROBOTS);
      }        
    }
};

Monster monster=Monster(31,31);

#endif
