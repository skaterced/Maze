#ifndef maze_h
#define maze_h

//#include <Arduino.h>

#include "globals.h"
#include "function.h"

#define SYMETRIC true
#define NBTILES 42//casesCol*casesRow

uint8_t timeUnit=5;
uint8_t test=0;
int scrollIt=0;

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
  for (int i=0;i<NBTILES;i++){
    tiles[i].draw();
//    if (LAST_TILE==i)
//      break;
  }
  //SelectorManagment();
  //drawSelector(getIndice(cursX,cursY));
  uint8_t walls=0;
  if (p1Playing){
      walls=tiles[getIndice(p1.x+leftBorder,p1.y+upBorder)].walls;
  }
  else{
      walls=tiles[getIndice(p2.x+leftBorder,p2.y+upBorder)].walls;
  }
  moveRobot(p1Playing, walls);//p1
  p1.draw(true, WOB);
  p2.draw(false, WOB);
    //test
  if (arduboy.justPressed(B_BUTTON)){
    //destroyWall
    if (test++<4){
      p1Playing=!p1Playing;
    }
    else
      scrollTiles(&message[6*(scrollIt++)]);
  }
  if (arduboy.justPressed(A_BUTTON)){    
    uint8_t temp;
    bool temp2=true;
    if (p1Playing){
      temp=getIndice(p1.x+leftBorder,p1.y+upBorder);
    if (p1.dir>1)
      temp2=false;
    }
    else {
      temp=getIndice(p2.x+leftBorder,p2.y+upBorder);
      if (p2.dir>1)
        temp2=false;
    }
    
    tiles[findInd(temp)].turn(temp2);
    imposeWall(temp, true);
  }  
  //Sprites::drawOverwrite(4, 50, robots, test);
  /*p1.score=getIndice(p1.x, p1.y);
  p2.score=tiles[findInd(getIndice(p1.x, p1.y))].walls;*/
  
  p1.score=getIndice(p1.x+leftBorder,p1.y+upBorder);
  p2.score=tiles[findInd(getIndice(p1.x+leftBorder,p1.y+upBorder))].walls;  
  //turnUpdate();
  
}
#endif
