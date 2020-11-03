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
    Monster(uint8_t x, uint8_t y) {
      this->x=x;
      this->y=y;
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
};

//Monster monster=Monster(31,31);
Monster monsters[2]={Monster(31,31),Monster(41,21)};

void checkMonsterCollision(void){ //between a robot and a monster
  Player * pp;
  pp=&p1;
  for (uint8_t j=0;j<2;j++ ){
    for (uint8_t i=0; i<NB_MONSTER_MAX; i++){  
      if ((pp->x==monsters[i].x)&&(pp->y==monsters[i].y)&&(monsters[i].dir!=DEAD)){
        pp->dir|=DEAD;
        hold=true;
      }
    }
    pp=&p2;
  }
}

bool controlMonsters(){  
  bool alive=false;
  uint8_t temp=0;
  for (uint8_t i=0; i<NB_MONSTER_MAX; i++){    
    if (monsters[i].dir!=DEAD){
      temp=(monsters[i].dir&0xF0);
      monsters[i].dir=(monsters[i].dir&0x0F);
      alive=true;
      if (0==temp){            
        monsters[i].firstStep();        
      }
      else if (0x10==temp){
        monsters[i].move(); //second step
        tiles[getIndice(monsters[i].x,monsters[i].y)].walls|=TILE_MONSTER;
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

/*
bool controlMonsters(bool start){
  if (start){
    bool alive=false;
    for (uint8_t i=0; i<NB_MONSTER_MAX; i++){    
      if (monsters[i].dir!=DEAD){
        alive=true;
        if (monsters[i].firstStep()){
          break;
        }
      }
    }
    return alive; // when start is true, if it returns false it means that every monsters are dead
  }
  else {
    for (uint8_t i=0; i<NB_MONSTER_MAX; i++){   
      if ((monsters[i].dir!=DEAD)&&(0x10==(monsters[i].dir&0x10))){
        monsters[i].dir&=0xF0;
        monsters[i].dir|=0x20;
        monsters[i].move();
        if ((NB_MONSTER_MAX-1)==i){
          return true; //every monster has moved
        }
        else {
          for (uint8_t j=i+1; j<NB_MONSTER_MAX; j++){
            if (monsters[j].dir!=DEAD){
              if (monsters[j].firstStep()){
                return false; //not over yet
              }
            }
          }          
          return true;
        }
      }
    }    
  }
}
*/
void monstersInit(void){
  for (uint8_t i=0; i<NB_MONSTER_MAX; i++){
    monsters[i].dir&=0;    
  }
  monsters[0].x=monsters[0].y=31;
  monsters[1].x=41;
  monsters[1].y=21;
  tiles[getIndice(31,31)].walls|=TILE_MONSTER; //todo: add random placing
  tiles[getIndice(21,41)].walls|=TILE_MONSTER;
}

void drawMonsters(){
  for (uint8_t i=0; i<NB_MONSTER_MAX; i++){
    monsters[i].draw();
  }
}
#endif
