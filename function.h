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

bool canGoTo(uint8_t ind, uint8_t direction){
  //check if there is no wall (or border) from tiles[ind] to tiles[voisin(ind, direction)]
  if (-1!=voisin(ind, direction)){
    if (0==(tiles[ind].walls&(0x10<<direction))){
      return true;
    }
  }
  return false;
}

//Sketch uses 12744 bytes (44%) of program storage space. Maximum is 28672 bytes.
//Global variables use 1494 bytes (58%) of dynamic memory, leaving 1066 bytes for local variables. Maximum is 2560 bytes.

bool controlRobot(void){ //check if arrow key is pressed, check if move is possible and then move
  uint8_t dir=99;
  bool stay=true;
  //uint8_t walls=0;
  uint8_t temp;
  uint8_t movesBack=movesLeft;
  Player* pp=&p2;

  if (p1Playing){
    pp=&p1;
  }
  temp=getIndice(pp->x+leftBorder,pp->y+upBorder);
  //walls=tiles[temp].walls;
  if (arduboy.justPressed(B_BUTTON)){ //B action (Bomb for now)
    if (0==(tiles[temp].walls&0x0F)){
      tiles[temp].walls+=7; //todo define that
      movesLeft--;
    }
  }
  else if (arduboy.justPressed(UP_BUTTON)){
    if (arduboy.pressed(A_BUTTON)){
      //select weapon
    }
    else {
      dir=HAUT;      
      //if ((pp->y>=casesHeight)&&((walls&WALL_UP)!=WALL_UP)){
      if (canGoTo(temp,dir)){
        pp->move(dir);
        stay=false;
      }
    }
  }
  else if (arduboy.justPressed(DOWN_BUTTON)){
    if (arduboy.pressed(A_BUTTON)){
      //select weapon
    }
    else {
      dir=BAS;
      //if ((pp->y<((casesRow-1)*casesHeight))&&((walls&WALL_DOWN)!=WALL_DOWN)){
      if (canGoTo(temp,dir)){
        stay=false;
        pp->move(dir);
      }
    }
  }
  else if (arduboy.justPressed(RIGHT_BUTTON)){
    if (arduboy.pressed(A_BUTTON)){
      if (0!=tiles[findInd(temp)].walls){
        tiles[findInd(temp)].turn(false);
        imposeWall(temp, true);
        movesLeft--;
      }
    }
    else {
      dir=DROITE;
      //if (((pp->x<((casesCol-1)*casesLength))&&((walls&WALL_RIGHT)!=WALL_RIGHT))||((pp->x<0)&&(pp->y==31))){
      if (canGoTo(temp,dir)){
        pp->move(dir);
        stay=false;        
      }
    }
  }
  else if (arduboy.justPressed(LEFT_BUTTON)){
    if (arduboy.pressed(A_BUTTON)){
      if (0!=tiles[findInd(temp)].walls){
        tiles[findInd(temp)].turn(true);
        imposeWall(temp, true);
        movesLeft--;
      }
    }
    else {
      dir=GAUCHE;
      //if (((pp->x>=(casesLength))&&((walls&WALL_LEFT)!=WALL_LEFT))||((pp->x>70)&&(pp->y==31))){        
      if (canGoTo(temp,dir)){
        pp->move(dir);
        stay=false;
      }
    }
  }
  if (99!=dir){
    pp->dir=dir;
    if (false==stay){         
      if ((p1.x==p2.x)&&(p1.y==p2.y)){
        //move back
        dir+=2;
        if (dir>3)
          dir-=4;
        pp->move(dir);
      }
      else { //valid move
        movesLeft--;
      }
    }
  }
  return(0!=(movesBack-movesLeft));
}

void explode(uint8_t ind, uint8_t range){
  tiles[ind].walls=((tiles[ind].walls&0xF0)|0x08);
  int temp[BOMB_RANGE_MAX+1]={ind,-1,-1};
  if (range>BOMB_RANGE_MAX)
    range=BOMB_RANGE_MAX;
  for (int j=0; j<4; j++){ //explode in all directions
    for (int i=0; i<range; i++){
      if (canGoTo(temp[i],j)){
        temp[i+1]=voisin(temp[i],j);
        if (-1!=temp[i+1]){
          if ((tiles[temp[i+1]].walls&0x07)>0){
            explode(temp[i+1],1);
          }
          tiles[temp[i+1]].walls=((tiles[temp[i+1]].walls&0xF0)|0x08);
        }
        else break;
      }
      else break;
    }
  }
}

void checkBombs(void){
  for (int i=0;i<NBTILES;i++){
    uint8_t temp=(tiles[i].walls&0x07);
    if (temp>0){
      tiles[i].walls=((tiles[i].walls&0xF0)|(--temp));
      if (0==temp){
        //boom
        //tiles[i].walls|=0x08;
        //tiles[i].walls&=0xF8;
        explode(i,1);        
      }
    }
  }
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
    x=87;
    if (leftBorder>LEFTBORDERP2)
      leftBorder-=(leftBorder-LEFTBORDERP2)/2;
  }
  arduboy.fillRect(x ,0,31,64,0);
  arduboy.setCursor(x,1);
  arduboy.print(F("Moves"));
  arduboy.setCursor(x,10);
  arduboy.print(movesLeft);
  if (arduboy.pressed(A_BUTTON)){
    arduboy.drawChar(x,25,27,1,0,1);
    arduboy.drawChar(x+5,25,26,1,0,1);
    arduboy.setCursor(x+13,25);
    arduboy.print(F("Turn"));
    arduboy.setCursor(x,34);
    arduboy.print(F("Walls"));
    arduboy.drawChar(x,46,24,1,0,1);
    arduboy.drawChar(x+5,47,25,1,0,1);    
    arduboy.setCursor(x+13,47);
    arduboy.print(F("Sel"));
    arduboy.setCursor(x,56);
    arduboy.print(F("Weapon"));
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
