#ifndef robot_h
#define robot_h

#include <Arduboy2.h>
//#include <stdlib.h>

#include "globals.h"
#include "weapon.h"
#include "function.h"
//#include "monster.h"

/*
#define BETWEEN_ROBOTS 8 //Frame difference between Robot1 & Robot2

#define WEAPON_MINE 0
#define WEAPON_PUNCH 1

#define BOMB_RANGE_MAX 3

const char* weaponList[] PROGMEM = {
  "Mine",
  "Fist"
};
*/

const unsigned char robots[] PROGMEM = {
// width, height,
8, 8,
//WoB
//R1_0-3
0x00,0x00,0x7c,0x2a,0x29,0x62,0x7c,0x00,
0x00,0x00,0x7c,0x2a,0x69,0x2a,0x7c,0x00,
0x00,0x00,0x7c,0x62,0x29,0x2a,0x7c,0x00,
0x00,0x00,0x7c,0x22,0x61,0x22,0x7c,0x00,
// BoW
//R_1_0-3
0xff,0xff,0x83,0xd5,0xd6,0x9d,0x83,0xff,
0xff,0xff,0x83,0xd5,0x96,0xd5,0x83,0xff,
0xff,0xff,0x83,0x9d,0xd6,0xd5,0x83,0xff,
0xff,0xff,0x83,0xdd,0x9e,0xdd,0x83,0xff,
//WoB
//R2_0-3
0x00,0x00,0x7e,0x2a,0x62,0x22,0x7e,0x00,
0x00,0x00,0x7e,0x62,0x2a,0x62,0x7e,0x00,
0x00,0x00,0x7e,0x22,0x62,0x2a,0x7e,0x00,
0x00,0x00,0x7e,0x62,0x22,0x62,0x7e,0x00,
//R_2_0-3
0xff,0xff,0x81,0xd5,0x9d,0xdd,0x81,0xff,
0xff,0xff,0x81,0x9d,0xd5,0x9d,0x81,0xff,
0xff,0xff,0x81,0xdd,0x9d,0xd5,0x81,0xff,
0xff,0xff,0x81,0x9d,0xdd,0x9d,0x81,0xff,
//blasted
0xbf,0x9f,0xcf,0x97,0x87,0xcf,0x9f,0xbf
};

class Player {
  public :
    int x,y;
    uint8_t weapons; //0 means normal Bombs
    uint8_t range;
    uint8_t dir; //if dir==DEAD, direction heaven
    int score;
    Bomb bombs[3];
    Player(int X, int Y) {
      this->x=X;
      this->y=Y;
      this->weapons=0;  //
      this->range=1;
      this->dir=0;      
      this->score=0;
    }
    void draw(bool player1, bool WhiteOnBlack){ //todo keep only BoW
      int frame=WhiteOnBlack? 0:4;
      frame+=(dir&0x0F);
      if (!player1){
        frame+=BETWEEN_ROBOTS;
      }
      if (DEAD!=dir){
        Sprites::drawOverwrite(x+leftBorder, y+upBorder, robots, frame);
      }
      else
        Sprites::drawOverwrite(x+leftBorder, y+upBorder, robots, 2*BETWEEN_ROBOTS);
    }
    void move(uint8_t direction, uint8_t distance){
      switch (direction){
        case HAUT:
          y-=distance;
        break;
        case BAS:         
          y+=distance;
        break;
        case DROITE:         
          x+=distance;
        break;
       case GAUCHE:
          x-=distance;
        break;
      }
    }
    void drawBombs(void){
      for (int i=0; i<NB_BOMB_MAX; i++){
        if (bombs[i].counter!=0){
          bombs[i].draw();
        }
      }
    }
    bool placeBomb(void){
      for (int i=0; i<NB_BOMB_MAX; i++){ // check if place is bomb free
        if ((0!=bombs[i].counter)&&(bombs[i].x==x)&&(bombs[i].y==y)){
          return false;
        }
      }      
      for (int i=0; i<NB_BOMB_MAX; i++){
        if (0==bombs[i].counter){
          bombs[i].x=x;
          bombs[i].y=y;
          bombs[i].counter=BOMB_DEFAULT_T;
          tiles[getIndice(x,y)].walls|=WALL_OQP;
          return true;
        }
      }
      return false;
    }
    void init(bool player1){
      if (player1){
        dir=2;
        x=1;
      }
      else {
        dir=0;
        x=71;
      }      
      y=31;
      for (uint8_t i=0; i<NB_BOMB_MAX; i++){
        bombs[i].counter=0;
      }
    }
};

class Player p1(1,31);
class Player p2(71,31);


void controlRobot(void){ //check if arrow key is pressed, check if move is possible and then move
  uint8_t dir=99;
  bool stay=true;
  //uint8_t walls=0;
  uint8_t temp;
  // uint8_t movesBack=movesLeft;
  Player* pp=&p2;

  if (p1Playing){
    pp=&p1;
  }
  temp=getIndice(pp->x,pp->y);
  //walls=tiles[temp].walls;
  if (arduboy.justPressed(B_BUTTON)){ //B action (Bomb for now)    
    /*if (0==(tiles[temp].walls&0x0F)){
      tiles[temp].walls+=7; //todo define that*/
    if (pp->placeBomb()){
      hold=true;
      timer=HOLD_THRESHOLD-1;
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
        pp->move(dir, casesHeight);
        stay=false;
      }
    }
  }
  else if (arduboy.justPressed(DOWN_BUTTON)){
    if (arduboy.pressed(A_BUTTON)){
      //select weapon or just EoT...
      movesLeft=1; //because now "movesLeft-- is done outside this function
      hold=true;
      timer=HOLD_THRESHOLD-1;
    }
    else {
      dir=BAS;
      //if ((pp->y<((casesRow-1)*casesHeight))&&((walls&WALL_DOWN)!=WALL_DOWN)){
      if (canGoTo(temp,dir)){
        stay=false;
        pp->move(dir, casesHeight);
      }
    }
  }
  else if (arduboy.justPressed(RIGHT_BUTTON)){
    if (arduboy.pressed(A_BUTTON)){
      if (0!=tiles[findInd(temp)].walls){
        tiles[findInd(temp)].turn(false);
        imposeWall(temp, true);
        hold=true;      
        timer=HOLD_THRESHOLD-1;
      }
    }
    else {
      dir=DROITE;
      //if (((pp->x<((casesCol-1)*casesLength))&&((walls&WALL_RIGHT)!=WALL_RIGHT))||((pp->x<0)&&(pp->y==31))){
      if (canGoTo(temp,dir)){
        pp->move(dir, casesLength);
        stay=false;        
      }
    }
  }
  else if (arduboy.justPressed(LEFT_BUTTON)){
    if (arduboy.pressed(A_BUTTON)){
      if (0!=tiles[findInd(temp)].walls){
        tiles[findInd(temp)].turn(true);
        imposeWall(temp, true);
        hold=true;
        timer=HOLD_THRESHOLD-1;
      }
    }
    else {
      dir=GAUCHE;
      //if (((pp->x>=(casesLength))&&((walls&WALL_LEFT)!=WALL_LEFT))||((pp->x>70)&&(pp->y==31))){        
      if (canGoTo(temp,dir)){
        pp->move(dir, casesLength);
        stay=false;
      }
    }
  }
  if (99!=dir){ //
    pp->dir=dir;
    if (false==stay){         
      if (((p1.x==p2.x)&&(p1.y==p2.y))||((tiles[getIndice(pp->x,pp->y)].walls&0x07)>0)){
        //return false;    
      }
      else { //valid move
        //movesLeft--; //inside checkMoving
        pp->dir|=0x20; //adds a "moving timer"
        hold=true;
        timer=HOLD_THRESHOLD-2;
        //return true;
      }
      dir+=2; //move back
      if (dir>3)
        dir-=4;
      pp->move(dir, casesLength); //just because length=height      
    }
  }
}

void checkMoving(void){
  Player* pp=&p2; //or monster?

  if (p1Playing){
    pp=&p1;
  }
  uint8_t temp=(pp->dir&0xF0)>>4;
  if (temp-->0){
    pp->move(pp->dir&0x0F,5);
    pp->dir&=0x0F;
    pp->dir|=(temp<<4);  
  }
}

void explode(uint8_t ind, uint8_t range){
  tiles[ind].walls=((tiles[ind].walls&0xF0)|WALL_EXPLOSION);
  int temp[BOMB_RANGE_MAX+1]={ind,-1,-1,-1};
  if (range>BOMB_RANGE_MAX)
    range=BOMB_RANGE_MAX;
  for (int j=0; j<4; j++){ //explode in all directions
    for (int i=0; i<range; i++){
      if (canGoTo(temp[i],j)){
        temp[i+1]=voisin(temp[i],j);
        if (-1!=temp[i+1]){
          if ((tiles[temp[i+1]].walls&0x07)>0){
            explode(temp[i+1],range);
          }
          tiles[temp[i+1]].walls=((tiles[temp[i+1]].walls&0xF0)|WALL_EXPLOSION);
        }
        else break;
      }
      else break;
    }
  }
}

bool checkBombs(void){
  bool boom=false;
  Player* pp=&p1;
  for (uint8_t j=0;j<2;j++){
    for (uint8_t i=0; i<NB_BOMB_MAX; i++){
      if (0!=pp->bombs[i].counter){
        if (0==--pp->bombs[i].counter){
          explode(getIndice(pp->bombs[i].x,pp->bombs[i].y),pp->range);
          boom=true;
        }
      }
    }
    pp=&p2; 
  }
  if (boom){ //check if there's a chain reaction
    for (uint8_t j=0;j<2;j++){
      for (int i=0; i<NB_BOMB_MAX; i++){
        if (0!=pp->bombs[i].counter){
          if ((tiles[getIndice(pp->bombs[i].x,pp->bombs[i].y)].walls&WALL_EXPLOSION)==WALL_EXPLOSION){
            explode(getIndice(pp->bombs[i].x,pp->bombs[i].y),pp->range);
            pp->bombs[i].counter=0;
            i=0; // because it might have triggered another...
          }
        }       
      }
      pp=&p1;
    }
  }
  return boom;
}

#endif
