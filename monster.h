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
      if (DEAD!=dir){
        Sprites::drawOverwrite(x+leftBorder, y+upBorder, monstre_bitmap, 0);
        if (3!=(dir&0x0F))
          arduboy.drawPixel(x+3+leftBorder+((timer&0x10)>>4),y+4+upBorder,1);
        arduboy.drawPixel(x+2+leftBorder+((timer&0x08)>>3)*3,y+7+upBorder,1);
      }
      else {
        Sprites::drawOverwrite(x+leftBorder, y+upBorder, robots, 2*BETWEEN_ROBOTS);
      }        
    }
    void move(){
      switch (dir&0x0F){
        case HAUT:
          y-=5;
        break;
        case BAS:         
          y+=5;
        break;
        case DROITE:         
          x+=5;
        break;
       case GAUCHE:
          x-=5;
        break;
      }
    }
    void update(void){
      if (dir!=DEAD){
        if ((dir&0xF0)==0){ //first step
          hold=true;
          dir=random(4);
          for (uint8_t i=0;i<4;i++){
            if (canGoTo(getIndice(x,y),dir)){
              move();
              break;
            }
            if(++dir>3)
              dir=0;
          }
          dir|=0x10;
        }
        else if ((dir&0x30)==0x10){ //second step
          move();
          dir|=0x20;
        }
        else {
          dir&=0x0F;
        }
      }
    }
};

Monster monster=Monster(31,31);

void checkMonsterCollision(void){ //between a robot and a monster
  Player * pp;
  pp=&p1;
  for (uint8_t i=0;i<2;i++ ){
    if ((pp->x==monster.x)&&(pp->y==monster.y)&&(monster.dir!=DEAD)){
      pp->dir|=DEAD;
      hold=true;
    }
    pp=&p2;
  }
}


#endif
