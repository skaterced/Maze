#ifndef maze_h
#define maze_h

//#include <Arduino.h>

#include "globals.h"
#include "function.h"

#define WOB 0 //1 for White on Black
#define WALL_UP 0x80
#define WALL_RIGHT 0x40
#define WALL_DOWN 0x20
#define WALL_LEFT 0x10
#define HAUT 3
#define DROITE 2
#define BAS 1
#define GAUCHE 0
#define SYMETRIC true
#define BETWEEN_ROBOTS 4 //Frame difference between Robot1 & Robot2

#define NBTILES 42//casesCol*casesRow

const unsigned char robots[] PROGMEM = {
// width, height,
8, 8,
// BoW? (not intended)
//R_1_0-3
0xff,0xff,0x83,0xd5,0xd6,0x9d,0x83,0xff,
0xff,0xff,0x83,0xd5,0x96,0xd5,0x83,0xff,
0xff,0xff,0x83,0x9d,0xd6,0xd5,0x83,0xff,
0xff,0xff,0x83,0xdd,0x9e,0xdd,0x83,0xff,
//R_2_0-3
0xff,0xff,0x81,0xd5,0x9d,0xdd,0x81,0xff,
0xff,0xff,0x81,0x9d,0xd5,0x9d,0x81,0xff,
0xff,0xff,0x81,0xdd,0x9d,0xd5,0x81,0xff,
0xff,0xff,0x81,0x9d,0xdd,0x9d,0x81,0xff};

uint8_t timeUnit=5;

class Tile {
  public:
    uint8_t i; //indice
    uint8_t walls;
    //bool black;
    //bool selected;
    Tile(void){}
    Tile(uint8_t i_, uint8_t walls_){
      i=i_;
      walls=walls_;
      //selected=false;
    }
    void draw(void){
      int x= leftBorder + (i%casesCol)*casesLength;
      int y= upBorder + i/casesCol*casesHeight;//+upBorder+2;
      if (WALL_UP==(walls&WALL_UP)){
        arduboy.drawLine(x,y,x+casesLength-1,y,WOB);
        arduboy.drawLine(x,y-1,x+casesLength-1,y-1,WOB);
      }
      if (WALL_DOWN==(walls&WALL_DOWN)){
        arduboy.drawLine(x,y+casesHeight-1,x+casesLength-1,y+casesHeight-1,WOB);
        arduboy.drawLine(x,y+casesHeight,x+casesLength-1,y+casesHeight,WOB);
      }      
      if (WALL_RIGHT==(walls&WALL_RIGHT)){
        arduboy.drawLine(x+casesLength-1,y,x+casesLength-1,y+casesHeight-1,WOB);
        arduboy.drawLine(x+casesLength,y,x+casesLength,y+casesHeight-1,WOB);
      }
      if (WALL_LEFT==(walls&WALL_LEFT)){
        arduboy.drawLine(x,y,x,y+casesHeight-1,WOB);
        arduboy.drawLine(x-1,y,x-1,y+casesHeight-1,WOB);
      }
    }
    void turn(bool clockWise){
      uint8_t onlyWalls=(walls&0xF0);
      if (clockWise){        
        onlyWalls*=2;
        onlyWalls+=(walls&0x80)>>3;
      }
      else{
        onlyWalls/=2;
        if((onlyWalls&0x08)!=0){
          onlyWalls&=0xF0;
          onlyWalls|=0x80;
        }
      }
      walls&=0x0F;
      walls+=onlyWalls;
    }
};
/*
 * || 0xA0
 *  = 0x50
 */
Tile tiles[NBTILES];
/*={Tile(0,0xD0),Tile(1,0),Tile(2,0x50),Tile(3,0),Tile(4,0xA0),Tile(5,0),Tile(6,0x30), Tile(7,0),
              Tile(8,0xD0),Tile(9,0),Tile(10,0x50),Tile(11,0),Tile(12,0xA0),Tile(13,0),Tile(14,0x30),
              Tile(15,0xD0),Tile(16,0),Tile(17,0x50),Tile(18,0),Tile(19,0xA0),Tile(20,0),Tile(21,0x30),
              Tile(22,0xD0),Tile(23,0),Tile(24,0x50),Tile(25,0),Tile(26,0xA0),Tile(27,0),Tile(28,0x30),
              Tile(29,0xD0),Tile(30,0),Tile(31,0x50),Tile(32,0),Tile(33,0x40),Tile(34,0),Tile(35,0x10),};*/

void Robot_Affiche(bool p1_){  //why not in player now that there are no other games...
  int frame=0;
  if (p1_)
    Sprites::drawOverwrite(p1.x, p1.y, robots, frame+p1.dir);
  else
    Sprites::drawOverwrite(p2.x,p2.y , robots, frame+BETWEEN_ROBOTS+p2.dir );
}


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

void mazeSetup(void){
  p1.x=32;
  p1.y=34;
  p2.x=114;
  p2.y=34;
  casesCol=7;
  casesRow=6;
  casesHeight=10;
  casesLength=10;
  leftBorder=42;
  upBorder=2;
  //adjSelectX=4;
  //adjSelectY=4;
  randomTiles(30, SYMETRIC, true ); //sym //border
  for (int i=0;i<NBTILES;i++){
    imposeWall(i, false);
  }  
}

void playMaze(){
  if (0==WOB)
    arduboy.fillRect(0,0,128,64,1);
  if ((arduboy.justPressed(A_BUTTON))||(arduboy.justPressed(B_BUTTON))){    
    uint8_t temp=getIndice(cursX,cursY);
    tiles[findInd(temp)].turn(arduboy.justPressed(A_BUTTON));
    imposeWall(temp, true);
  }
  for (int i=0;i<NBTILES;i++){
    tiles[i].draw();
//    if (LAST_TILE==i)
//      break;
  }
  SelectorManagment();
  drawSelector(getIndice(cursX,cursY));
  Robot_Affiche(true);
  Robot_Affiche(false);
  
    //test
  //p1.score=getIndice(p1.x, p1.y);
  //p2.score=tiles[findInd(getIndice(p1.x, p1.y))].walls;
  //turnUpdate();
  inGameMenu();
}
#endif
