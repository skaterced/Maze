#ifndef function_h
#define function_h

//#include <Arduino.h>
#include "globals.h"
#include "tiles.h"
//#include "robot.h"

int getIndice(int x, int y){ //TO DO
  //arduboy.fillRect(x+1,y+1,7,10,fill);
  int temp=(x)/casesLength;
  temp+=(y)/casesHeight*casesCol;
  //arduboy.print(temp);
  return temp;
}
/*
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
*/

bool canGoTo(uint8_t ind, uint8_t direction){
  //check if there is no wall (or border) from tiles[ind] to tiles[voisin(ind, direction)]
  if (-1!=voisin(ind, direction)){
    if (0==(tiles[ind].walls&(0x10<<direction))){
      return true;
    }
  }
  return false;
}

void inGameMenu(bool test, int test1, int test2){
  //int x=p1Playing? (leftBorder-42):(leftBorder+80);
  int x;
  char* s;
  if (p1Playing){
    x=0;
    if (leftBorder<LEFTBORDERP1)
      leftBorder+=(LEFTBORDERP1-leftBorder)/2;      
  }
  else {
    x=87;
    if (leftBorder>LEFTBORDERP2)
      leftBorder-=(leftBorder-LEFTBORDERP2)/2;
  }
  arduboy.fillRect(x ,0,31,64,0);
  arduboy.setCursor(x,1);
  arduboy.print(F("Moves"));
  arduboy.setCursor(x,10);
  arduboy.print(movesLeft);
  if (test){
    arduboy.setCursor(x,30);
    arduboy.print(test1);
    arduboy.setCursor(x,50);
    arduboy.print(test2);
  }
  else {
    if (arduboy.pressed(A_BUTTON)){
      arduboy.drawChar(x,25,27,1,0,1);
      arduboy.drawChar(x+5,25,26,1,0,1);
      arduboy.setCursor(x+13,25);
      arduboy.print(F("Turn"));
      arduboy.setCursor(x,34);
      arduboy.print(F("Walls"));
      /*
      arduboy.drawChar(x,46,24,1,0,1);
      arduboy.drawChar(x+5,47,25,1,0,1);    
      arduboy.setCursor(x+13,47);
      arduboy.print(F("SelB:"));
      arduboy.setCursor(x,56);
      arduboy.print(F("Weapon"));
      */
      arduboy.drawChar(x,47,25,1,0,1);    
      arduboy.setCursor(x+5,47);
      arduboy.print(F(": End"));
      arduboy.setCursor(x+15,56);
      arduboy.print(F("Turn"));
    }
    else {
      arduboy.setCursor(x,30);
      arduboy.print(F("B for:"));
      arduboy.setCursor(x+5,40);
      arduboy.print(F("Bomb"));
      arduboy.setCursor(x,55);
      arduboy.print(F("A+..."));
      //arduboy.print(s);
      //arduboy.print(F("EoT"));
    }
  }
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
