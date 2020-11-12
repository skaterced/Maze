#ifndef monster_h
#define weapon_h

#include "globals.h"
#include "robot.h"

#define MONSTER_TYPE_CYCLOPE 0
#define MONSTER_TYPE_BOMB 1
#define MONSTER_TYPE_WORM 2

const unsigned char PROGMEM monstre_bitmap[] = {
  // width, height,
  8, 8,
  0x0f, 0xc7, 0x03, 0x81, 0x81, 0x03, 0xc7, 0x0f,
  0xf0, 0xe7, 0x03, 0x85, 0x81, 0x03, 0xe7, 0xf0,
  0xf3,0xad,0x0e,0x6a,0x7e,0x7d,0x03,0xbf, //worm
  0xff,0xbf,0x0f,0x77,0x7b,0x6b,0x07,0xbf,
  0xff,0xbf,0x5f,0x5f,0x5f,0x5f,0x5f,0xbf, //worm hole
  0xff,0xbf,0x19,0x62,0x66,0x7e,0x0a,0xb1 //worm screaming
};

class Monster {
  public :
    uint8_t x, y;
    uint8_t dir;
    uint8_t type;
    //uint8_t counter; // decreasing to 0 means exploding, then inactive
    Monster() { //uint8_t x, uint8_t y) {
      this->x = 0;
      this->y = 0;
      //this->range=1;  //-> robot
      this->dir = 0;
      type=0;
    }
    void draw(void) {
      if (MONSTER_TYPE_CYCLOPE==type){
        if (DEAD != (dir & 0x0F)) {
          Sprites::drawOverwrite(x + leftBorder, y + upBorder, monstre_bitmap, 0);
  
          if (3 != (dir & 0x0F))
            arduboy.drawPixel(x + 3 + leftBorder + ((timer & 0x10) >> 4), y + 4 + upBorder, 1);
          arduboy.drawPixel(x + 2 + leftBorder + ((timer & 0x08) >> 3) * 3, y + 7 + upBorder, 1);
        }
        else {
          if ((timer < (HOLD_THRESHOLD + 9))&&((dir&0x10)!=0x10)) {
            Sprites::drawOverwrite(x + leftBorder, y + upBorder, monstre_bitmap, 1); //monster "sreaming"
          }
          else {
            Sprites::drawOverwrite(x + leftBorder, y + upBorder, robots_bitmap, 2 * BETWEEN_ROBOTS);
          }
          if (timer==HOLD_THRESHOLD+10){ //tile has finished exploding, now it contains a dead body
            dir|=0x10;
            tiles[getIndice(x,y)].walls|=TILE_TBD;
            if (0x20!=(dir&0x20)){
              if (drop(x,y)){
                y=166;
              }
            }
            dir|=0x20;            
          }
        }
      }
      else if (MONSTER_TYPE_WORM==type){
        if (DEAD != (dir & 0x0F)) {
          if (0x10!=(dir&0x10))
            Sprites::drawOverwrite(x + leftBorder, y + upBorder, monstre_bitmap, 2);
        }
        else {
          if ((timer < (HOLD_THRESHOLD + 9))&&((dir&0x10)!=0x10)) {
            Sprites::drawOverwrite(x + leftBorder, y + upBorder, monstre_bitmap, 5); //monster "sreaming"
          }
          else {
            Sprites::drawOverwrite(x + leftBorder, y + upBorder, robots_bitmap, 2 * BETWEEN_ROBOTS);
          }
          if (timer==HOLD_THRESHOLD+10){ //tile has finished exploding, now it contains a dead body
            dir|=0x10;
            tiles[getIndice(x,y)].walls|=TILE_TBD;
            if (0x20!=(dir&0x20)){
              if (drop(x,y)){
                y=166;
              }
            }
            dir|=0x20;            
          }
        }
      }
      else if (MONSTER_TYPE_BOMB==type){ //drop?
        if (DEAD != (dir & 0x0F)) {          
          arduboy.drawCircle(x+4+leftBorder,y+4+upBorder,3,0);
          arduboy.drawPixel(x+1+leftBorder,y+7+upBorder,(0x04==(timer&0x0C))?1:0);
          arduboy.drawPixel(x+7+leftBorder,y+7+upBorder,(0x0C==(timer&0x0C))?1:0);
  
          if (3 != (dir & 0x0F)) {
            arduboy.drawPixel(x + 3 + leftBorder, y + 4 + upBorder, 0);
            arduboy.drawPixel(x + 5 + leftBorder, y + 4 + upBorder, 0);
          //arduboy.drawPixel(x + 2 + leftBorder + ((timer & 0x08) >> 3) * 3, y + 7 + upBorder, 0);
          }
        }
      }
    }
    void move() {
      switch (dir & 0x0F) {
        case HAUT:
          y -= 5;
        break;
        case BAS:
          y += 5;
        break;
        case DROITE:
          x += 5;
        break;
        case GAUCHE:
          x -= 5;
        break;
        case (WWN):
          x -= 20;
          y -= 10;
        break;
        case (WWS):
          x -= 20;
          y += 10;
        break;
        case (SSW):
          x -= 10;
          y += 20;
        break;
        case (SSE):
          x += 10;
          y += 20;
        break;
        case (EES):
          x += 20;
          y += 10;
        break;
        case (EEN):
          x += 20;
          y -= 10;
        break;
        case (NNE):
          x += 10;
          y -= 20;
        break;
        case (NNW):
          x -= 10;
          y -= 20;
        break;          
      }
    }
    bool firstStep(void) {
      uint8_t tempI=getIndice(x, y);
      if (MONSTER_TYPE_WORM==type){
        dir = random(8)+4;
        for (uint8_t i = 0; i < 8; i++) {
          if (canGoTo(tempI, dir, MONSTER)){// &&(0==(dir&0xF0)))) { useless, I check that before
            tiles[tempI].walls &= ~TILE_MONSTER;
            tiles[voisin(tempI, dir)].walls |= TILE_MONSTER;          
            //animation...
            Sprites::drawOverwrite(x + leftBorder, y + upBorder, monstre_bitmap, 3);
            arduboy.display();
            delay(80);
            Sprites::drawOverwrite(x + leftBorder, y + upBorder, monstre_bitmap, 4);
            arduboy.display();
            delay(80);
            dir |= 0x10;
            return true;
          }   
          if (++dir > 11)
            dir = 4;
        }
      }
      else {
        dir = random(4);
        for (uint8_t i = 0; i < 4; i++) {
          if (canGoTo(tempI, dir, 1)){// &&(0==(dir&0xF0)))) { useless, I check that before
            tiles[tempI].walls &= ~TILE_MONSTER;
            tiles[voisin(tempI, dir)].walls |= TILE_MONSTER;          
            move();
            dir |= 0x10;
            return true;
          }        
          if (++dir > 3)
            dir = 0;
        }
      }
      dir |= 0x20; // couldn't move
      return false;
    }
    void reInit () {
      //type=MONSTER_TYPE_WORM;
      //type=(random(100)<20)? MONSTER_TYPE_BOMB:MONSTER_TYPE_CYCLOPE;  

      uint8_t randMon0=monstersPlaying*6;
      uint8_t randMon1=(monstersPlaying-1)*5;
      uint8_t randMon2=(monstersPlaying-1)*3;
      
      uint8_t diceRoll=random(randMon0+randMon1+randMon2);
      if (diceRoll<=randMon0)
        type=0;
      else if (diceRoll<=randMon0+randMon1)
        type=1;
      else if (diceRoll<=randMon0+randMon1+randMon2)
        type=2;
      
      uint8_t tempX = random(casesCol) * casesLength + 1;
      uint8_t tempY = random(casesRow) * casesHeight + 1;
      uint8_t tempI = getIndice(tempX, tempY);      
      while ((tempI == 24) || (tempI == 31) || ((tiles[tempI].walls & TILE_MONSTER) == TILE_MONSTER)) {
        tempX = random(casesCol) * casesLength + 1;
        tempY = random(casesRow) * casesHeight + 1;
        tempI = getIndice(tempX, tempY);
      }
      x = tempX;
      y = tempY;
      tiles[tempI].walls |= TILE_MONSTER;
      dir = ((random(100) > 50) ? 0 : 0x40);
    }
};

//Monster monster=Monster(31,31);
//Monster monsters[2]={Monster(31,31),Monster(41,21)};
Monster monsters[NB_MONSTER_MAX];

bool checkMonsterCollision(void) { //between a robot and a monster
  Player * pp;
  pp = &p1;
  for (uint8_t j = 0; j < 2; j++ ) {
    for (uint8_t i = 0; i < monstersPlaying; i++) {
      if ((pp->x == monsters[i].x) && (pp->y == monsters[i].y) && ((monsters[i].dir&DEAD) != DEAD)) {
        if (INVINCIBILITY!=(pp->dir&INVINCIBILITY)){
          pp->dir |= DEAD;
          return true;
        }        
      }
    }
    pp = &p2;
  }
  return false;
}

void checkCrush(uint8_t ind, uint8_t what){
  if (ROBOT==what){
    Player * pp =&p1;
    if (!p1Playing)
      pp=&p2;
    ind=getIndice(pp->x,pp->y);
    for (uint8_t i=0; i<NB_BONUS_MAX; i++){
      if((pp->x==bonus[i].x)&&(pp->y==bonus[i].y)){
        switch (bonus[i].type){
          case BONUS_INACTIVE:            
          break;
          case BONUS_BIGGER:
            pp->range++;
          break;
          case BONUS_DETO:
            pp->weapons=WEAPON_DETO;
          break;
          case BONUS_SCORE:
            pp->score+=25;
          break;
          case BONUS_SHUFFLER:
            pp->weapons=WEAPON_SHUFFLER;
          break;
          case BONUS_TELEPORT:
            pp->weapons=WEAPON_TELEPORT;
          break;
          case BONUS_NUKE:
            pp->weapons=WEAPON_NUKE;
          break;
          default:
            pp->x=1; //means Error, bonus not defined
          break;
        }
        bonus[i].type=BONUS_INACTIVE; //break;
      }
    }
  }
  else {
    // crushing dead monsters body (by another monster or by an explosion)
    for (uint8_t i = 0; i < monstersPlaying; i++) {
      if ((monsters[i].dir&DEAD) == DEAD) {
        if (getIndice(monsters[i].x,monsters[i].y)==ind){
          monsters[i].y=182; //six feet under...
          return;
        }
      }
    }
  }
}

bool controlMonsters() { //return false if every monsters are dead
  bool alive = false;
  uint8_t temp = 0;
  for (uint8_t i = 0; i < monstersPlaying; i++) {
    if ((monsters[i].dir&DEAD) != DEAD) {
      temp = (monsters[i].dir & 0xF0);
      monsters[i].dir = (monsters[i].dir & 0x0F);
      alive = true;
      if (0 == temp) {
        monsters[i].firstStep();
      }
      else if (0x10 == temp) {
        monsters[i].move(); //second step
        if (MONSTER_TYPE_WORM==monsters[i].type){          
          //anim
          Sprites::drawOverwrite(monsters[i].x + leftBorder, monsters[i].y + upBorder, monstre_bitmap, 4);
          arduboy.display();
          delay(80);
          Sprites::drawOverwrite(monsters[i].x + leftBorder, monsters[i].y + upBorder, monstre_bitmap, 3);
          arduboy.display();
          delay(80);
        }        
        uint8_t tempI=getIndice(monsters[i].x,monsters[i].y);
        if (TILE_TBD==(tiles[tempI].walls&TILE_TBD)){
            checkCrush(tempI, MONSTER);
        }
        temp = (temp << 2);
        //|temp; //0x20;
      }
      else if (0x80 == temp) {
        temp = 0; //monster now ready to move again
        //return true;
      }
      else {
        temp = (temp << 1);
      }
      monsters[i].dir |= temp;
    }
  }
  return alive;
}

void monstersInit(void) {
  for (uint8_t i = 0; i < NB_MONSTER_MAX; i++) {
    if (i < monstersPlaying) {
      monsters[i].reInit();
    }
    else { //just in case...
      monsters[i].x = monsters[i].y = 90;
      monsters[i].dir = DEAD;
    }
  }
}

void drawMonsters() {
  for (uint8_t i = 0; i < monstersPlaying; i++) {
    monsters[i].draw();
  }
}
#endif
