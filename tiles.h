#ifndef tiles_h
#define tiles_h

//#include <Arduino.h>

#include "globals.h"
#include "function.h"

int scrollIt=0;

int findInd(uint8_t ind){ //return the indice (in the tiles array) that is on the given grid indice
  for (int i=0; i<casesCol*casesRow; i++){
    if (tiles[i].i==ind)
      return i;
  }
  return -1;
}

int voisin(uint8_t ind, uint8_t direction){  //Yes, I switched back to french. I prefer the word "voisin" than "neighbor"
  switch(direction){
    case (HAUT):
      if (ind<casesCol)
        return (-1);
      else {
        return (findInd(ind-casesCol));
      }
    break;
    case (DROITE):
      if (0==((ind+1)%casesCol))
        return (-1);
      else {
        return (findInd(ind+1));
      }
    break;
    case (BAS):
      if (ind>=(casesCol*(casesRow-1)))
        return (-1);
      else {
        return (findInd(ind+casesCol));
      }
    break; 
    case (GAUCHE):
      if (0==(ind%casesCol))
        return (-1);
      else {
        return (findInd(ind-1));
      }
    break;
  }
  return -1;
}

void randomTiles (uint8_t randWall, bool sym, bool border){
  for (int i=0; i<NBTILES; i++){
    tiles[i].i=i; //thatt's a whole bunch of i's...
    uint8_t tw=0; //tiles[i].walls
    if (sym&&(i%casesCol>(casesCol/2))){ 
      tw=tiles[i-2*(i%casesCol-casesCol/2)].walls;
      bool temp=tw&WALL_LEFT;  // méthode bête et méchante...
      bool temp2=tw&WALL_RIGHT;
      tw&=~(WALL_LEFT|WALL_RIGHT);
      tw+=(temp? WALL_RIGHT:0);
      tw+=(temp2? WALL_LEFT:0);
    }
    else{
      for (int j=0; j<4; j++){
        tw=tw<<1;
        if (random(100)<randWall)
          tw+=0x10;      
      }
    }
    if (sym&&(i%casesCol==(casesCol/2))){ 
      if (tw&WALL_LEFT){
        tw|=WALL_RIGHT;
      }
      else {
        tw&=~(WALL_LEFT|WALL_RIGHT);
      }
    }
    tiles[i].walls=tw;
  }
  if (border){
    for (int i=0; i<NBTILES; i++){ //must be in another loop, otherwise les voisins ne sont pas définis
      for (int j=0; j<4; j++){
        if (-1==voisin(i,j))
          tiles[i].walls|=(0x10<<j);        
      }
    }
    tiles[21].walls&=~WALL_LEFT;  //entrance
    tiles[27].walls&=~WALL_RIGHT;
  }
}

void imposeWall(uint8_t ind, bool addAndRemove){
  uint8_t tileInd=findInd(ind);
  int temp=voisin(ind, HAUT);  
  if (-1!=temp){
    if (WALL_UP==(tiles[tileInd].walls&WALL_UP)){
      tiles[temp].walls|=WALL_DOWN;
    }
    else if(addAndRemove){
      tiles[temp].walls&=(~WALL_DOWN);
    }
  }
  temp=voisin(ind, DROITE);  
  if (-1!=temp){
    if (WALL_RIGHT==(tiles[tileInd].walls&WALL_RIGHT)){
      tiles[temp].walls|=WALL_LEFT;
    }
    else if(addAndRemove){
      tiles[temp].walls&=(~WALL_LEFT);
    }
  }
  temp=voisin(ind, BAS);  
  if (-1!=temp){
    if (WALL_DOWN==(tiles[tileInd].walls&WALL_DOWN)){
      tiles[temp].walls|=WALL_UP;
    }
    else if(addAndRemove){
      tiles[temp].walls&=(~WALL_UP);
    }
  }  
  temp=voisin(ind, GAUCHE);  
  if (-1!=temp){
    if (WALL_LEFT==(tiles[tileInd].walls&WALL_LEFT)){
      tiles[temp].walls|=WALL_RIGHT;
    }
    else if(addAndRemove){
      tiles[temp].walls&=(~WALL_RIGHT);
    }
  }    
}

void scrollTiles(uint8_t *col){ // uint8_t[5]
  //uint8_t col[]={0,0,0,0,0,0}; //def?
  for(int i=0;i<6;i++){
    for(int j=0;j<6;j++){
      //col[j]=tiles[j+i*7].walls;
      tiles[j+i*7].walls=tiles[j+i*7+1].walls;
    }
  }
  //reboucle
  for(int i=0;i<6;i++){      
    tiles[6+i*7].walls=col[i];  
  }
}

#endif
