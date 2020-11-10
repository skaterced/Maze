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
0xff,0x7d,0x7d,0x55,0x55,0x7d,0x7d,0xff //teleport
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


#define DROP_RAND 40

Bonus bonus[NB_BONUS_MAX];
/* copy of globals.h
#define BONUS_BIGGER 1
#define BONUS_DETO 2
#define BONUS_SCORE 3
#define BONUS_SHUFFLER 4
#define BONUS_TELEPORT 5
*/

uint8_t bonusProb[NB_BONUS_TYPE]={10,10,20,10,10};

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
    bonus[i].draw();
  }
}
#endif
