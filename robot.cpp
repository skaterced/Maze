
//#include <stdlib.h>

#include "globals.h"
#include "weapon.h"
#include "function.h"
#include "monster.h"
#include "robot.h"
#include "tiles.h"

class Player p1(1,31);
class Player p2(71,31);



    Player::Player(uint8_t X, uint8_t Y) {
      this->x=X;
      this->y=Y;
      this->weapons=0;  //
      this->range=1;
      this->dir=0;      
      this->score=0;
    }
    void Player::draw(bool player1){ //todo keep only BoW
      uint8_t frame=0; //WhiteOnBlack? 0:4;
      frame+=(dir&0x0F);
      if (!player1){
        frame+=BETWEEN_ROBOTS;
      }
      if (DEAD!=dir){
        Sprites::drawOverwrite(x+leftBorder, y+upBorder, robots_bitmap, frame);          
      }
      else {
        if (timer<(HOLD_THRESHOLD+9)){
          Sprites::drawOverwrite(x+leftBorder, y+upBorder, robots_bitmap, (player1?  1:2)*BETWEEN_ROBOTS-1);
        }
        else {      
          Sprites::drawOverwrite(x+leftBorder, y+upBorder, robots_bitmap, 2*BETWEEN_ROBOTS);
        }
      }
    }
    void Player::move(uint8_t direction, uint8_t distance){
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
    void Player::drawBombs(void){
      for (uint8_t i=0; i<NB_BOMB_MAX; i++){
        if (bombs[i].counter!=0){
          bombs[i].draw();
        }
      }
    }

    void Player::init(bool player1){
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



    bool Player::placeBomb(void){  
      if ((tiles[getIndice(x,y)].walls&TILE_BOMB)==TILE_BOMB)
        return false;
      for (uint8_t i=0; i<NB_BOMB_MAX; i++){
        if (0==bombs[i].counter){
          bombs[i].x=x;
          bombs[i].y=y;
          bombs[i].counter=(twoPlayersMode? BOMB_DEFAULT_T : BOMB_DEFAULT_T-3);
          tiles[getIndice(x,y)].walls|=TILE_BOMB;
          return true;
        }
      }
      return false;
    }

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
  if (ab.justPressed(B_BUTTON)){ //B action (Bomb for now)    
    if (ab.pressed(A_BUTTON)){
      //secondary weapon (or switch weapon)
    }
    else {
      if (pp->placeBomb()){
      hold=true;
      timer=HOLD_THRESHOLD-1;
      }
    }
  }
  else if (ab.justPressed(UP_BUTTON)){
    if (ab.pressed(A_BUTTON)){
      //select weapon
    }
    else {
      dir=HAUT;      
      //if ((pp->y>=casesHeight)&&((walls&WALL_UP)!=WALL_UP)){
      if (canGoTo(temp,dir,0)){
        pp->move(dir, casesHeight);
        stay=false;
      }
    }
  }
  else if (ab.justPressed(DOWN_BUTTON)){
    if (ab.pressed(A_BUTTON)){
      //select weapon or just EoT...
      movesLeft=1; //because now "movesLeft-- is done outside this function
      hold=true;
      timer=HOLD_THRESHOLD-1;
    }
    else {
      dir=BAS;
      //if ((pp->y<((casesRow-1)*casesHeight))&&((walls&WALL_DOWN)!=WALL_DOWN)){
      if (canGoTo(temp,dir,0)){
        stay=false;
        pp->move(dir, casesHeight);
      }
    }
  }
  else if (ab.justPressed(RIGHT_BUTTON)){
    if (ab.pressed(A_BUTTON)){
      if (0!=tiles[temp].walls){
        tiles[temp].turn(false);
        imposeWall(temp, true);
        hold=true;      
        timer=HOLD_THRESHOLD-1;
      }
    }
    else {
      dir=DROITE;
      //if (((pp->x<((casesCol-1)*casesLength))&&((walls&WALL_RIGHT)!=WALL_RIGHT))||((pp->x<0)&&(pp->y==31))){
      if (canGoTo(temp,dir,0)){
        pp->move(dir, casesLength);
        stay=false;        
      }
    }
  }
  else if (ab.justPressed(LEFT_BUTTON)){
    if (ab.pressed(A_BUTTON)){
      if (0!=tiles[temp].walls){
        tiles[temp].turn(true);
        imposeWall(temp, true);
        hold=true;
        timer=HOLD_THRESHOLD-1;
      }
    }
    else {
      dir=GAUCHE;
      //if (((pp->x>=(casesLength))&&((walls&WALL_LEFT)!=WALL_LEFT))||((pp->x>70)&&(pp->y==31))){        
      if (canGoTo(temp,dir,0)){
        pp->move(dir, casesLength);
        stay=false;
      }
    }
  }
  if (99!=dir){ //
    pp->dir=dir;
    if (false==stay){         
      if ((p1.x==p2.x)&&(p1.y==p2.y)){//||((tiles[getIndice(pp->x,pp->y)].walls&0x07)>0)){
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
  tiles[ind].walls=(tiles[ind].walls&0xF0)|TILE_EXPLODING;
  int temp[BOMB_RANGE_MAX+1]={ind,-1,-1,-1};
  if (range>BOMB_RANGE_MAX)
    range=BOMB_RANGE_MAX;
  for (uint8_t j=0; j<4; j++){ //explode in all directions
    for (uint8_t i=0; i<range; i++){
      if (canGoTo(temp[i],j,2)){
        temp[i+1]=voisin(temp[i],j);
        if (-1!=temp[i+1]){
          if (TILE_BOMB==(tiles[temp[i+1]].walls&TILE_BOMB)){
            explode(temp[i+1],range); ///todo, get the bomb's range
          }
          tiles[temp[i+1]].walls=((tiles[temp[i+1]].walls&0xF0)|TILE_EXPLODING);
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
      for (uint8_t i=0; i<NB_BOMB_MAX; i++){
        if (0!=pp->bombs[i].counter){
          if ((tiles[getIndice(pp->bombs[i].x,pp->bombs[i].y)].walls&TILE_EXPLODING)==TILE_EXPLODING){
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
