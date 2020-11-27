#ifndef tiles_h
#define tiles_h

//#include <Arduino.h>

#include "globals.h"
#include "function.h"

//int scrollIt=0;
/*
 * Before deletins i
 *Sketch uses 14212 bytes (49%) of program storage space. Maximum is 28672 bytes.
Global variables use 1499 bytes (58%) of dynamic memory, leaving 1061 bytes for local variables. Maximum is 2560 bytes.

after:
Sketch uses 14018 bytes (48%) of program storage space. Maximum is 28672 bytes.
Global variables use 1451 bytes (56%) of dynamic memory, leaving 1109 bytes for local variables. Maximum is 2560 bytes.

 */

class Tile {
  public:
    //uint8_t i; //indice
    uint8_t walls; //bit 7-4: walls  bit 3: explosion  bit 2: Stuff explosion can go throug  bit 1: stuff that block explosion spreading (but is destroyed)  bit 0: unbreakable stuff
    Tile(void){}
    Tile(uint8_t walls_){
      //i=i_;
      walls=walls_;
      //selected=false;
    }
    void turn(bool clockWise){
      uint8_t onlyWalls=(walls&0xF0);
      if (clockWise){        
        onlyWalls*=2;
        onlyWalls+=(walls&0x80)>>3;
      }
      else{
        onlyWalls/=2;
        if((onlyWalls&TILE_EXPLODING)!=0){
          onlyWalls&=0xF0;
          onlyWalls|=0x80;
        }
      }
      walls&=0x0F;
      walls|=onlyWalls;
    }    
};

Tile tiles[NBTILES];

void drawTiles(void){
  int x;
  int y;
  for (uint8_t i=0;i<NBTILES;i++){
    x= leftBorder + (i%casesCol)*casesLength;
    y= upBorder + i/casesCol*casesHeight;//+upBorder+2;
    if (WALL_UP==(tiles[i].walls&WALL_UP)){
      arduboy.drawLine(x,y,x+casesLength-1,y,0);
      arduboy.drawLine(x,y-1,x+casesLength-1,y-1,0);
    }
    if (WALL_DOWN==(tiles[i].walls&WALL_DOWN)){
      arduboy.drawLine(x,y+casesHeight-1,x+casesLength-1,y+casesHeight-1,0);
      arduboy.drawLine(x,y+casesHeight,x+casesLength-1,y+casesHeight,0);
    }      
    if (WALL_RIGHT==(tiles[i].walls&WALL_RIGHT)){
      arduboy.drawLine(x+casesLength-1,y,x+casesLength-1,y+casesHeight-1,0);
      arduboy.drawLine(x+casesLength,y,x+casesLength,y+casesHeight-1,0);
    }
    if (WALL_LEFT==(tiles[i].walls&WALL_LEFT)){
      arduboy.drawLine(x,y,x,y+casesHeight-1,0);
      arduboy.drawLine(x-1,y,x-1,y+casesHeight-1,0);
    }
    if (TILE_EXPLODING==(tiles[i].walls&TILE_EXPLODING)){
      if (timer<(HOLD_THRESHOLD+5)){
        //arduboy.drawChar(x+2,y+1,'O',0,1,1); 
        arduboy.fillCircle(x+5,y+4,timer-HOLD_THRESHOLD+1,0);
      }        
      else if (timer<(HOLD_THRESHOLD+10)){
        //arduboy.drawChar(x+2,y+1,'@',0,1,1);
        arduboy.fillCircle(x+5,y+4,5,0);
        arduboy.fillCircle(x+5,y+4,timer-HOLD_THRESHOLD-3,1);
      }
      else {
        tiles[i].walls&=0xF0; //clears the tile content
      }
    }
  }
}/*Sketch uses 18368 - 18504 bytes (64%) of program storage space. Maximum is 28672 bytes.
Global variables use 1470 bytes (57%) of dynamic memory, leaving 1090 bytes for local variables. Maximum is 2560 bytes.
*/
int voisin(uint8_t ind, uint8_t direction){  //Yes, I switched back to french. I prefer the word "voisin" than "neighbor"
  if (ind>200)
    return -1;
  switch(direction){
    case (HAUT):
      if (ind<casesCol)
        return (-1);
      else {
        return (ind-casesCol);
      }
    break;
    case (DROITE):
      if (0==((ind+1)%casesCol))
        return (-1);
      else {
        return (ind+1);
      }
    break;
    case (BAS):
      if (ind>=(casesCol*(casesRow-1)))
        return (-1);
      else {
        return (ind+casesCol);
      }
    break; 
    case (GAUCHE):
      if (0==(ind%casesCol))
        return (-1);
      else {
        return (ind-1);
      }
    break;
    case (WWN):
      return voisin(voisin(voisin(ind, GAUCHE),GAUCHE),HAUT);
    break;
    case (WWS):
      return voisin(voisin(voisin(ind, GAUCHE),GAUCHE),BAS);
    break;
    case (SSW):
      return voisin(voisin(voisin(ind, BAS),BAS),GAUCHE);
    break;
    case (SSE):
      return voisin(voisin(voisin(ind, BAS),BAS),DROITE);
    break;
    case (EES):
      return voisin(voisin(voisin(ind, DROITE),DROITE),BAS);
    break;
    case (EEN):
      return voisin(voisin(voisin(ind, DROITE),DROITE),HAUT);
    break;
    case (NNE):
      return voisin(voisin(voisin(ind, HAUT),HAUT),DROITE);
    break;
    case (NNW):
      return voisin(voisin(voisin(ind, HAUT),HAUT),GAUCHE);
    break;
  }
  return -1;
}

void imposeWall(uint8_t ind, bool addAndRemove){
//  uint8_t tileInd=findInd(ind);
  int temp=voisin(ind, HAUT);  
  if (-1!=temp){
    if (WALL_UP==(tiles[ind].walls&WALL_UP)){
      tiles[temp].walls|=WALL_DOWN;
    }
    else if(addAndRemove){
      tiles[temp].walls&=(~WALL_DOWN);
    }
  }
  temp=voisin(ind, DROITE);  
  if (-1!=temp){
    if (WALL_RIGHT==(tiles[ind].walls&WALL_RIGHT)){
      tiles[temp].walls|=WALL_LEFT;
    }
    else if(addAndRemove){
      tiles[temp].walls&=(~WALL_LEFT);
    }
  }
  temp=voisin(ind, BAS);  
  if (-1!=temp){
    if (WALL_DOWN==(tiles[ind].walls&WALL_DOWN)){
      tiles[temp].walls|=WALL_UP;
    }
    else if(addAndRemove){
      tiles[temp].walls&=(~WALL_UP);
    }
  }  
  temp=voisin(ind, GAUCHE);  
  if (-1!=temp){
    if (WALL_LEFT==(tiles[ind].walls&WALL_LEFT)){
      tiles[temp].walls|=WALL_RIGHT;
    }
    else if(addAndRemove){
      tiles[temp].walls&=(~WALL_RIGHT);
    }
  }    
}

void randomTiles (uint8_t randWall, bool sym, bool border){
  for (uint8_t i=0; i<NBTILES; i++){
    //tiles[i].i=i; //need this??
    uint8_t tw=0; //tiles[i].walls
    if (sym&&(i%casesCol>=(casesCol/2))){
      tw=tiles[i-2*(i%casesCol-casesCol/2)-1].walls;
      bool temp=tw&WALL_LEFT;  // méthode bête et méchante...
      bool temp2=tw&WALL_RIGHT;
      tw&=~(WALL_LEFT|WALL_RIGHT);
      tw+=(temp? WALL_RIGHT:0);
      tw+=(temp2? WALL_LEFT:0);
    }
    else{
      tw=0;
      for (uint8_t j=0; j<4; j++){
        tw=tw<<1;
        if (random(100)<randWall)
          tw+=0x10;      
      }
    }
    tiles[i].walls=tw;
  }
  if (border){
    for (uint8_t i=0; i<NBTILES; i++){ //must be in another loop, otherwise les voisins ne sont pas définis
      for (uint8_t j=0; j<4; j++){
        if (-1==voisin(i,j))
          tiles[i].walls|=(0x10<<j);        
      }
    }
    tiles[31].walls&=~WALL_LEFT;  //entrance
    tiles[24].walls&=~WALL_RIGHT;
    tiles[25].walls&=~WALL_LEFT;  //entrance
    tiles[30].walls&=~WALL_RIGHT;
  }
  for (uint8_t i=0;i<NBTILES;i++){
    imposeWall(i, false);
  }
}

void scrollTiles(uint8_t *col){ // uint8_t[5]
  //uint8_t col[]={0,0,0,0,0,0}; //def?
  for(uint8_t i=0;i<6;i++){
    for(uint8_t j=0;j<6;j++){
      //col[j]=tiles[j+i*7].walls;
      tiles[j+i*7].walls=tiles[j+i*7+1].walls;
    }
  }
  //reboucle
  for(uint8_t i=0;i<6;i++){      
    tiles[6+i*7].walls=col[i];  
  }
}

#endif
