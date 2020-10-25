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
    arduboy.fillRect(leftBorder-10,0,89,64,1);
  /*
  if ((arduboy.justPressed(A_BUTTON))||(arduboy.justPressed(B_BUTTON))){    
    uint8_t temp=getIndice(cursX,cursY);
    tiles[findInd(temp)].turn(arduboy.justPressed(A_BUTTON));
    imposeWall(temp, true);
  }*/
  //bool kaboom=false;
  for (int i=0;i<NBTILES;i++){
  //  kaboom=(kaboom||tiles[i].draw());  //dec the bomb timer, return true if it explode
    tiles[i].draw();
  }
  //SelectorManagment();
  //drawSelector(getIndice(cursX,cursY));

  if (controlRobot()){ // check arrow keys and move robot
    //ticks
    for (int i=0;i<NBTILES;i++){
//      if (tiles[i].walls=((tiles[i].walls&0xF0)|(--(tiles[i].walls&0x0F)));
    }
  }
  if(movesLeft==0){
    movesLeft=3;
    p1Playing=!p1Playing;
  }
  p1.draw(true, WOB);
  p2.draw(false, WOB);
    //test
  if (arduboy.justPressed(B_BUTTON)){
    //place mine    
    /*if (test++<4){
      
    }
    else
      scrollTiles(&message[6*(scrollIt++)]);*/
  }
  if (arduboy.justPressed(A_BUTTON)){
    //A + <- / -> : turn wall CCW/CW
    //A + up or down : switch weapon (not yet) 
  }  

  
  //Sprites::drawOverwrite(4, 50, robots, test);
  /*p1.score=getIndice(p1.x, p1.y);
  p2.score=tiles[findInd(getIndice(p1.x, p1.y))].walls;*/
  
  //p1.score=getIndice(p1.x+leftBorder,p1.y+upBorder);
  //p2.score=tiles[findInd(getIndice(p1.x+leftBorder,p1.y+upBorder))].walls;  
  //turnUpdate();
  
}
#endif
