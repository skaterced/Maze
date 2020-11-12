#ifndef function_h
#define function_h

//#include <Arduino.h>
#include "globals.h"
#include "tiles.h"
//#include "robot.h"

int getIndice(uint8_t x, uint8_t y){ //TO DO
  //arduboy.fillRect(x+1,y+1,7,10,fill);
  int temp=(x)/casesLength;
  temp+=(y)/casesHeight*casesCol;
  //arduboy.print(temp);
  return temp;
}

void SelectorManagment(void){
    if (arduboy.justPressed(UP_BUTTON)){
      if (cursY>=casesHeight){
        cursY-=casesHeight;
      }
    }
    if (arduboy.justPressed(DOWN_BUTTON)){
      if (cursY<((casesRow-1)*casesHeight)){ 
        cursY+=casesHeight;
      }
    }
    if (arduboy.justPressed(RIGHT_BUTTON)){
      if (cursX<((casesCol-1)*casesLength)){
        cursX+=casesLength;
      }
    }
    if (arduboy.justPressed(LEFT_BUTTON)){
      if (cursX>=(casesLength)){
        cursX-=casesLength;
      }
    }
}

void drawSelector(/*int i*/){
/*   if (blinkTimer--==0){
    blinkTimer=BLINK_TIMER_INIT;
    blink=!blink;
  }*/  
  //uint8_t x=(i%casesCol)*casesLength+leftBorder+4;
  //uint8_t y=(i/casesCol)*casesHeight+upBorder+4;
  uint8_t x=cursX+leftBorder+4;
  uint8_t y=cursY+upBorder+4;
  
  arduboy.drawLine(x-4,y-4,x-2,y-2,0);  
  arduboy.drawLine(x-4,y+4,x-2,y+2,0);
  arduboy.drawLine(x+2,y+2,x+4,y+4,0);
  arduboy.drawLine(x+2,y-2,x+4,y-4,0);
}


bool canGoTo(uint8_t ind, uint8_t direction, uint8_t what){ //what: 0:Robot, 1: monster, 2:explosion, 3:cursor (OR NUKE)
  //check if there is no wall (or border) from tiles[ind] to tiles[voisin(ind, direction)]
  int temp=voisin(ind, direction);
  if (-1!=temp){
    if (CURSOR==what){
      return true;
    }
    if(direction>3){ // (direction>3 only for worm and he doesn't care about the walls)
      if ((tiles[temp].walls&(TILE_BOMB|TILE_MONSTER))!=0){
        return false;
      }
      else return true;
    }
    else if (0==(tiles[ind].walls&(0x10<<direction))){
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

#endif
