#ifndef maze_h
#define maze_h

//#include <Arduino.h>

#include "globals.h"
#include "function.h"

uint8_t timeUnit=5;
uint8_t test=0;

void playMaze(){
  inGameMenu();
  if (0==WOB)
    arduboy.fillRect(leftBorder-2,0,84,64,1);

  for (int i=0;i<NBTILES;i++){
    tiles[i].draw();
  }
  //SelectorManagment();
  //drawSelector(getIndice(cursX,cursY));

  if (controlRobot()){ // check buttons and play robot's action
    //ticks (robot has done one action)
    checkBombs();
  }
  if(movesLeft==0){
    movesLeft=movesInit;
    p1Playing=!p1Playing;
  }
  p1.draw(true, WOB);
  p2.draw(false, WOB);
  
  //test  
  //Sprites::drawOverwrite(4, 50, robots, test);
  /*p1.score=getIndice(p1.x, p1.y);
  p2.score=tiles[findInd(getIndice(p1.x, p1.y))].walls;*/ 
  p1.score=getIndice(p1.x+leftBorder,p1.y+upBorder);
  p2.score=tiles[findInd(getIndice(p1.x+leftBorder,p1.y+upBorder))].walls;  
  //turnUpdate();
  
}
#endif
