#ifndef function_h
#define function_h

//#include <Arduino.h>
#include "globals.h"

int getIndice(int x, int y){ //TO DO
  //arduboy.fillRect(x+1,y+1,7,10,fill);
  int temp=(x-leftBorder)/casesLength;
  temp+=(y-upBorder)/casesHeight*casesCol;
  //arduboy.print(temp);
  return temp;
}

/*
int drawStone(int i, bool color){ //black 0 white 1
  int x=(i%casesCol)*casesLength+leftBorder;
  int y=(i/casesCol)*casesHeight+upBorder;
  arduboy.fillCircle(x,y,3,0);
  if (color){
    arduboy.fillCircle(x,y,2,1);
  }
}
void drawStones(void){
  for (int i=0; i<81; i++){
    if (i==selectedI){
      if (blink) {
        if (BLACK_STONE==stoneArray[i]){
          drawStone(i,0);
        }
        else if (WHITE_STONE==stoneArray[i]){
          drawStone(i,1);
        }
      }
    }
    else {
      if (BLACK_STONE==stoneArray[i]){
      drawStone(i,0);
      }
      else if (WHITE_STONE==stoneArray[i]){
        drawStone(i,1);
      }
    }
  }
}
*/
void SelectorManagment(void){
    if (arduboy.justPressed(UP_BUTTON)){
      if (cursY>=upBorder+casesHeight){
        cursY-=casesHeight;
      }
    }
    if (arduboy.justPressed(DOWN_BUTTON)){
      if (cursY<(upBorder+(casesRow-1)*casesHeight)){ 
        cursY+=casesHeight;
      }
    }
    if (arduboy.justPressed(RIGHT_BUTTON)){
      if (cursX<(leftBorder+(casesCol-1)*casesLength)){
        cursX+=casesLength;
      }
    }
    if (arduboy.justPressed(LEFT_BUTTON)){
      if (cursX>=(leftBorder+casesLength)){
        cursX-=casesLength;
      }
    }
}
void drawSelector(int i){

   if (blinkTimer--==0){
    blinkTimer=BLINK_TIMER_INIT;
    blink=!blink;
  }  
  int x=(i%casesCol)*casesLength+leftBorder+4;
  int y=(i/casesCol)*casesHeight+upBorder+4;
  //x+=adjSelectX;
  //y+=adjSelectY;
  arduboy.drawLine(x-4,y-4,x-2,y-2,blink? 1:0);  // Why not add a variable to adjust the Selector for each game? todo?
  arduboy.drawLine(x-4,y+4,x-2,y+2,blink? 1:0);
  arduboy.drawLine(x+2,y+2,x+4,y+4,blink? 1:0);
  arduboy.drawLine(x+2,y-2,x+4,y-4,blink? 1:0);
}

void moveRobot(bool player1){
  uint8_t dir=99;
  if (arduboy.justPressed(UP_BUTTON)){
    dir=HAUT;
  }
  if (arduboy.justPressed(DOWN_BUTTON)){
    dir=BAS;
  }
  if (arduboy.justPressed(RIGHT_BUTTON)){
    dir=DROITE;
  }
  if (arduboy.justPressed(LEFT_BUTTON)){
    dir=GAUCHE;
  }
  if (99!=dir){
    if (player1)
      p1.move(dir);
    else
      p2.move(dir);
  }
}

void inGameMenu(){
  //int x=p1Playing? (leftBorder-42):(leftBorder+80);
  int x=p1Playing? 0:100;
  arduboy.fillRect(x ,0,31,64,0);
  arduboy.setCursor(x,10);
  arduboy.print(F("Walls"));
  arduboy.setCursor(x,30);
  arduboy.print(F("Robot"));
  arduboy.setCursor(x,50);
  arduboy.print(F("EoT"));
}
/*
void turnUpdate(void){ /////////////////////////////////////// score ///////////////////////  score ///////////////////////
  if (1==difficulty) { // for MEMO that have bigger cards in difficulty 1
    arduboy.fillRect(0,0,17,64,0);
    if (p1Playing){
      arduboy.setCursor(1,6);
      arduboy.print(F("P1"));
    }
    else{
      arduboy.setCursor(1,39);
      arduboy.print(F("P2"));
    }
  }
  else {
    arduboy.fillRect(0,0,26,64,0);
    if((game!=CHESS)&&(game!=GO)){ //we don't knows who's turn it is in those games
      if (p1Playing){
        arduboy.setCursor(1,6);
        arduboy.print(F("P1's"));
        arduboy.setCursor(1,14);
        arduboy.print(F("turn"));
      }
      else{
        arduboy.setCursor(1,39);
        arduboy.print(F("P2's"));
        arduboy.setCursor(1,47);
        arduboy.print(F("turn"));
      }
    }
  }
  arduboy.setCursor(5,24);
  arduboy.print(p1.score);
  arduboy.setCursor(5,57);
  arduboy.print(p2.score);
  if ((GO==game)||(MILL==game)){
    if (removing){
      arduboy.setCursor(1,30);
      arduboy.print(F("Removing"));
    }
    arduboy.drawCircle(22,27,3,1); // to picture the remaining (or placed) stones
    arduboy.fillCircle(22,60,3,1);
  }
}*/
#endif
