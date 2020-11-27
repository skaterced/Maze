#ifndef weapon_h
#define weapon_h

#include "globals.h"

const unsigned char PROGMEM bonus_bitmap[] = {
// width, height,
8, 8,
0xf7,0xff,0xf7,0xa2,0xf7,0xff,0xf7,0xff,
0xff,0xff,0x85,0xb1,0x85,0xff,0xff,0xff,
0xff,0xff,0xd3,0xd5,0xd5,0xe5,0xff,0xff, //score
0xf7,0xe7,0xef,0x39,0x9c,0xf7,0xe7,0xef, //shuffler
0xff,0x7d,0x7d,0x55,0x55,0x7d,0x7d,0xff, //teleport
0xff,0xab,0xc7,0xc7,0xc7,0xc7,0xef,0xff, //nuke (droite)
0xff,0xff,0xd7,0x83,0xc7,0x83,0xd7,0xff, //mine
0xff,0xc1,0xdf,0xc1,0xff,0xc1,0xf5,0xfb, //1UP
//those are just to anim the nuking
0xff,0xef,0xc7,0xc7,0xc7,0xc7,0xab,0xff, //gauche
0xff,0xff,0xfd,0xc3,0x81,0xc3,0xfd,0xff, //bas
0xff,0xab,0xc7,0xc7,0xc7,0xc7,0xef,0xff, //nuke (droite)
0xff,0xff,0xbf,0xc3,0x81,0xc3,0xbf,0xff, //haut
0x00,0x00,0x40,0x3c,0x7e,0x3c,0x40,0x00  //haut blanc sur noir

};

class Mine {
  public:
    uint8_t x,y;
    bool active;
    Mine(){
      x=99;
      y=99;
      active=false;
    }
    void draw(void){
      if (active){
        arduboy.drawPixel(x+3+leftBorder,y+3+upBorder,(timer&0x02)>>1);
        arduboy.drawCircle(x+4+leftBorder,y+4+upBorder,2,0); 
      }
    }
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
    Bonus(){
      x=y=type=0;
    }
    Bonus(uint8_t x_, uint8_t y_, uint8_t t){
      x=x_;
      y=y_;
      type=t;
    }
    void draw(bool draw){
      if (draw){
        switch(type){
          case 0:
          break;
          default:
            Sprites::drawOverwrite(x + leftBorder, y + upBorder, bonus_bitmap, type-1);
          break;
        }
      }    
      else {
        arduboy.fillRect(x+leftBorder+1,y+upBorder+1,7,7,1);
      }
    }
};


#define DROP_RAND 40

Bonus bonus[NB_BONUS_MAX];
Mine mines[NB_MINE_MAX];

/* copy of globals.h
#define BONUS_BIGGER 1
#define BONUS_DETO 2
#define BONUS_SCORE 3
#define BONUS_SHUFFLER 4
#define BONUS_TELEPORT 5
#define BONUS_NUKE 6
*/

uint8_t bonusProb[NB_BONUS_TYPE]={10,10,10,10,10,10,10,10};
//uint8_t bonusProb[NB_BONUS_TYPE]={0,0,0,0,0,10};

uint8_t getRandBonus(void){
  uint8_t diceRoll=0;
  uint8_t probI=0;
  for (uint8_t i=0; i<NB_BONUS_TYPE; i++){
    diceRoll+=bonusProb[i]; //diceRoll is first used to get the sum, then the random result
  }
  diceRoll=random(diceRoll);
  for (uint8_t i=0; i<NB_BONUS_TYPE; i++){
    probI+=bonusProb[i];
    if (diceRoll<=probI){
      return (i+1);
    }
  }
  return 200; //should never happen
}

bool drop(uint8_t x_, uint8_t y_){
  if (random(100)<DROP_RAND){
    for (uint8_t i=0; i<NB_BONUS_MAX; i++){
      if (BONUS_INACTIVE==bonus[i].type){
        bonus[i].x=x_;
        bonus[i].y=y_;
        bonus[i].type=getRandBonus();
        return true;  
      }
    }
    
  }
  return false;
}

void drawBonuses(void){
  for (uint8_t i=0; i<NB_BONUS_MAX; i++){
    bonus[i].draw(true);
  }
  for (uint8_t i=0; i<NB_MINE_MAX; i++){
    mines[i].draw();
  }
}
#endif
