#ifndef function_h
#define function_h

//#include <Arduino.h>
#include "globals.h"
#include "tiles.h"

int getIndice(int x, int y){ //TO DO
  //arduboy.fillRect(x+1,y+1,7,10,fill);
  int temp=(x-leftBorder)/casesLength;
  temp+=(y-upBorder)/casesHeight*casesCol;
  //arduboy.print(temp);
  return temp;
}

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

bool controlRobot(void){ //check if arrow key is pressed, check if move is possible and then move
  uint8_t dir=99;
  bool stay=true;
  uint8_t walls=0;
  uint8_t temp;
  uint8_t movesBack=movesLeft;

  if (p1Playing){
    temp=getIndice(p1.x+leftBorder,p1.y+upBorder);
    walls=tiles[temp].walls;
    if (arduboy.justPressed(B_BUTTON)){
      if (0==tiles[temp].walls&0x0F){
        tiles[temp].walls+=7; //todo define that
        movesLeft--;
      }
    }
    else if (arduboy.justPressed(UP_BUTTON)){
      dir=HAUT;      
      if ((p1.y>=casesHeight)&&((walls&WALL_UP)!=WALL_UP)){
        p1.move(dir);
        stay=false;
      }
    }
    else if (arduboy.justPressed(DOWN_BUTTON)){
      dir=BAS;
      if ((p1.y<((casesRow-1)*casesHeight))&&((walls&WALL_DOWN)!=WALL_DOWN)){
        stay=false;
        p1.move(dir);
      }
    }
    else if (arduboy.justPressed(RIGHT_BUTTON)){
      if (arduboy.pressed(A_BUTTON)){
        tiles[findInd(temp)].turn(false);
        imposeWall(temp, true);
        movesLeft--;
      }
      else {
        dir=DROITE;
        if (((p1.x<((casesCol-1)*casesLength))&&((walls&WALL_RIGHT)!=WALL_RIGHT))||((p1.x<0)&&(p1.y==31))){
          p1.move(dir);
          stay=false;        
        }
      }
    }
    else if (arduboy.justPressed(LEFT_BUTTON)){
      if (arduboy.pressed(A_BUTTON)){
        tiles[findInd(temp)].turn(true);
        imposeWall(temp, true);
        movesLeft--;
      }
      else {
        dir=GAUCHE;
        if (((p1.x>=(casesLength))&&((walls&WALL_LEFT)!=WALL_LEFT))||((p1.x>70)&&(p1.y==31))){        
          p1.move(dir);
          stay=false;
        }
      }
    }
    if (99!=dir){
      p1.dir=dir;
      if (false==stay){         
        if ((p1.x==p2.x)&&(p1.y==p2.y)){
          //move back
          dir+=2;
          if (dir>3)
            dir-=4;
          p1.move(dir);
        }
        else { //valid move
          movesLeft--;
        }
      }
    }
  }
  else { //same for p2 ------------2 -2 2 222222222222222222222222222222222222222222222222222222222222222222222
    temp=getIndice(p2.x+leftBorder,p2.y+upBorder);
    walls=tiles[temp].walls;
    if (arduboy.justPressed(UP_BUTTON)){
      dir=HAUT;      
      if ((p2.y>=casesHeight)&&((walls&WALL_UP)!=WALL_UP)){
        p2.move(dir);
        stay=false;
      }
    }
    else if (arduboy.justPressed(DOWN_BUTTON)){
      dir=BAS;
      if ((p2.y<((casesRow-1)*casesHeight))&&((walls&WALL_DOWN)!=WALL_DOWN)){
        stay=false;
        p2.move(dir);
      }
    }
    else if (arduboy.justPressed(RIGHT_BUTTON)){
      if (arduboy.pressed(A_BUTTON)){
        tiles[findInd(temp)].turn(false);
        imposeWall(temp, true);
        movesLeft--;
      }
      else {
        dir=DROITE;
        if (((p2.x<((casesCol-1)*casesLength))&&((walls&WALL_RIGHT)!=WALL_RIGHT))||((p2.x<0)&&(p2.y==31))){
          p2.move(dir);
          stay=false;       
        }
      }
    }
    else if (arduboy.justPressed(LEFT_BUTTON)){
      if (arduboy.pressed(A_BUTTON)){
        tiles[findInd(temp)].turn(true);
        imposeWall(temp, true);
        movesLeft--;
      }
      else {
        dir=GAUCHE;
        if (((p2.x>=(casesLength))&&((walls&WALL_LEFT)!=WALL_LEFT))||((p2.x>70)&&(p2.y==31))){
          p2.move(dir);
          stay=false;
        }
      }
    }
    if (99!=dir){
      p2.dir=dir;
      if (false==stay){        
        if ((p1.x==p2.x)&&(p1.y==p2.y)){
          dir+=2;
          if (dir>3)
            dir-=4;
          p2.move(dir);
        }
        else { //valid move
          movesLeft--;
        }
      }
    }
  }
  return(0==(movesBack-movesLeft));
}

void inGameMenu(){
  //int x=p1Playing? (leftBorder-42):(leftBorder+80);
  int x;
  char* s;
  if (p1Playing){
    x=0;
    if (leftBorder<LEFTBORDERP1)
      leftBorder+=(LEFTBORDERP1-leftBorder)/2;
      
  }
  else {
    x=96;
    if (leftBorder>LEFTBORDERP2)
      leftBorder-=(leftBorder-LEFTBORDERP2)/2;
  }
  arduboy.fillRect(x ,0,31,64,0);
  arduboy.setCursor(x,1);
  arduboy.print(F("Moves"));
  arduboy.setCursor(x,10);
  arduboy.print(movesLeft);
  arduboy.setCursor(x,30);
  arduboy.print(F("Equip"));
  arduboy.setCursor(x,40);
  arduboy.print(F("Mine"));
  //arduboy.print(s);
  //arduboy.print(F("EoT"));
}

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
}
#endif
