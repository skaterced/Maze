
//#include <Arduino.h>
#include "globals.h"
#include "tiles.h"
#include "robot.h"
#include "function.h"
//#include "monsters.h"

int getIndice(uint8_t x, uint8_t y){ //TO DO
  //ab.fillRect(x+1,y+1,7,10,fill);
  int temp=(x)/casesLength;
  temp+=(y)/casesHeight*casesCol;
  //ab.print(temp);
  return temp;
}

bool canGoTo(uint8_t ind, uint8_t direction, uint8_t what){ //what: 0:Robot, 1: monster, 2:explosion
  //check if there is no wall (or border) from tiles[ind] to tiles[voisin(ind, direction)]
  int temp=voisin(ind, direction);
  if (-1!=temp){
    if (0==(tiles[ind].walls&(0x10<<direction))){
      if (TILE_BOMB==(tiles[temp].walls&TILE_BOMB)){
        if (EXPLOSION==what)
          return true;
        return false;
      }
      if (TILE_MONSTER==(tiles[temp].walls&TILE_MONSTER)){
        if (MONSTER==what)
          return false;
        return true;
      }
      return true;
    }
  }
  return false;
}
/*
void checkCrush(uint8_t ind, uint8_t what){
  // crushing dead monsters body (no matter who did this)
  for (uint8_t i = 0; i < monstersPlaying; i++) {
    if ((monsters[i].dir&DEAD) == DEAD) {
      if (getIndice(monsters[i].x,monsters[i].y)==ind){
        monsters[i].y=182; //six feet under...
        return;
      }
    }
  }
}
*/
