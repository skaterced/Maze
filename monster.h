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
        if (3!=dir)
          arduboy.drawPixel(x+3+leftBorder+((timer&0x10)>>4),y+4+upBorder,1);
        arduboy.drawPixel(x+2+leftBorder+((timer&0x08)>>3)*3,y+7+upBorder,1);
      }
      else {
        Sprites::drawOverwrite(x+leftBorder, y+upBorder, robots, 2*BETWEEN_ROBOTS);
      }        
    }
    void move(){
      if (dir!=DEAD){
        if ((dir&0x10)!=0){
          //dir&=0xE0;
          dir=random(4);
          for (uint8_t i=0;i<4;i++){
            if (canGoTo(getIndice(x,y),dir)){
              switch (dir){
                case HAUT:
                  y-=10;
                break;
                case BAS:         
                  y+=10;
                break;
                case DROITE:         
                  x+=10;
                break;
               case GAUCHE:
                  x-=10;
                break;
              }
              break;
            }
            if(++dir>3)
              dir=0;
          }        
        }
        else{
          dir|=0x10;
        }
      }
    }
};

Monster monster=Monster(31,31);

void checkCollision(void){ //between a robot and a monster
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
