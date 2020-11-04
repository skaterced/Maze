#ifndef monster_h
#define weapon_h

#include "globals.h"
#include "robot.h"

const unsigned char PROGMEM monstre_bitmap[] = {
// width, height,
8, 8,
0x0f,0xc7,0x03,0x81,0x81,0x03,0xc7,0x0f};

class Monster {
  public :
    uint8_t x,y;
    uint8_t dir;
    //uint8_t counter; // decreasing to 0 means exploding, then inactive  
    Monster(){//uint8_t x, uint8_t y) {
      this->x=0;
      this->y=0;
      //this->range=1;  //-> robot
      this->dir=0;            
    }
    void draw(void){
      if (DEAD!=dir){
        Sprites::drawOverwrite(x+leftBorder, y+upBorder, monstre_bitmap, 0);
        if (3!=(dir&0x0F))
          arduboy.drawPixel(x+3+leftBorder+((timer&0x10)>>4),y+4+upBorder,1);
        arduboy.drawPixel(x+2+leftBorder+((timer&0x08)>>3)*3,y+7+upBorder,1);
      }
      else {
        Sprites::drawOverwrite(x+leftBorder, y+upBorder, robots, 2*BETWEEN_ROBOTS);
      }        
    }
    void move(){
      switch (dir&0x0F){
        case HAUT:
          y-=5;
        break;
        case BAS:         
          y+=5;
        break;
        case DROITE:         
          x+=5;
        break;
       case GAUCHE:
          x-=5;
        break;
      }
    }
    bool firstStep(void){
      dir=random(4);
      for (uint8_t i=0;i<4;i++){
        if (canGoTo(getIndice(x,y),dir,1)){
          tiles[getIndice(x,y)].walls&=~TILE_MONSTER;
          tiles[voisin(getIndice(x,y),dir)].walls|=TILE_MONSTER;
          move();
          dir|=0x10;
          return true;
        }
        if(++dir>3)
          dir=0;
      }
      dir|=0x40; // couldn't move              
      return false;
    }
    void reInit (){
      uint8_t tempX = random(casesCol)*casesLength+1;
      uint8_t tempY = random(casesRow)*casesHeight+1;
      uint8_t tempI = getIndice(tempX,tempY);
      while ((tempI==24)||(tempI==31)||((tiles[tempI].walls&TILE_MONSTER)==TILE_MONSTER)){
        tempX = random(casesCol)*casesLength;
        tempY = random(casesRow)*casesHeight;
        tempI = getIndice(tempX,tempY);
      }
      x=tempX;
      y=tempY;
      tiles[tempI].walls|=TILE_MONSTER;
      dir=((random(100)>50)? 0:0x40);
    }
};

//Monster monster=Monster(31,31);
//Monster monsters[2]={Monster(31,31),Monster(41,21)};
Monster monsters[NB_MONSTER_MAX];

bool checkMonsterCollision(void){ //between a robot and a monster
  Player * pp;
  pp=&p1;
  for (uint8_t j=0;j<2;j++ ){
    for (uint8_t i=0; i<nbMonstersPlaying; i++){  
      if ((pp->x==monsters[i].x)&&(pp->y==monsters[i].y)&&(monsters[i].dir!=DEAD)){
        pp->dir|=DEAD;
        return true;
      }
    }
    pp=&p2;
  }
  return false;
}

bool controlMonsters(){  //return false if every monsters are dead
  bool alive=false;
  uint8_t temp=0;
  for (uint8_t i=0; i<nbMonstersPlaying; i++){    
    if (monsters[i].dir!=DEAD){
      temp=(monsters[i].dir&0xF0);
      monsters[i].dir=(monsters[i].dir&0x0F);
      alive=true;
      if (0==temp){            
        monsters[i].firstStep();        
      }
      else if (0x10==temp){
        monsters[i].move(); //second step
        //tiles[getIndice(monsters[i].x,monsters[i].y)].walls|=TILE_MONSTER;
        temp=(temp<<2);   
        //|temp; //0x20;
      }
      else if (0x80==temp){
        temp=0;  //monster now ready to move again
        //return true;
      }
      else{
        temp=(temp<<1);           
      }
      monsters[i].dir|=temp;
    }
  }
  return alive;
}

void monstersInit(void){
  for (uint8_t i=0; i<NB_MONSTER_MAX; i++){
    if (i<nbMonstersPlaying){
      monsters[i].reInit();
    }
    else{ //just in case...
      monsters[i].x=monsters[i].y=90;
      monsters[i].dir=DEAD;
    }
  }
  /*
  monsters[0].x=monsters[0].y=31;
  monsters[1].x=41;
  monsters[1].y=21;
  tiles[getIndice(31,31)].walls|=TILE_MONSTER; //todo: add random placing
  tiles[getIndice(21,41)].walls|=TILE_MONSTER;*/
}

void drawMonsters(){
  for (uint8_t i=0; i<nbMonstersPlaying; i++){
    monsters[i].draw();
  }
}
#endif
