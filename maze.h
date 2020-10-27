#ifndef maze_h
#define maze_h

//#include <Arduino.h>

#include "globals.h"
#include "function.h"
#include "robot.h"


/*
void playMaze(){
  /* //test
  p1.score=getIndice(p1.x+leftBorder,p1.y+upBorder);
  p2.score=tiles[findInd(getIndice(p1.x+leftBorder,p1.y+upBorder))].walls;    
  inGameMenu(true, p1.score, p2.score); //"test mode" if true
  */
  inGameMenu(false, 0,0); //"test mode" if true
  
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
  p1.drawBombs();
  p2.drawBombs();
  p1.draw(true, WOB);  
  p2.draw(false, WOB);
}*/
#endif
