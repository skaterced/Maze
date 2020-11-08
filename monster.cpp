#include "globals.h"
#include "function.h"
#include "robot.h"
#include "monster.h"
#include "tiles.h"

//Monster monsters[NB_MONSTER_MAX];

void Monster::draw(void) {
  if (DEAD != (dir & 0x0F)) {
    if (MONSTER_TYPE_GIANT==type){
      Sprites::drawOverwrite(x + leftBorder, y + upBorder, monstre_bitmap, 0);
  
      if (3 != (dir & 0x0F))
        ab.drawPixel(x + 3 + leftBorder + ((timer & 0x10) >> 4), y + 4 + upBorder, 1);
      ab.drawPixel(x + 2 + leftBorder + ((timer & 0x08) >> 3) * 3, y + 7 + upBorder, 1);
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
      }
    }
  }
  else if (MONSTER_TYPE_BOMB==type){
    ab.drawPixel(x+3+leftBorder,y+3+upBorder,(timer&0x04)>>2);
    ab.drawCircle(x+4+leftBorder,y+4+upBorder,3,0);      
  }   
}
bool Monster::firstStep(void) {
  uint8_t temp=getIndice(x, y);
  dir = random(4);
  for (uint8_t i = 0; i < 4; i++) {
    if (canGoTo(getIndice(x, y), dir, 1)){// &&(0==(dir&0xF0)))) { useless, I check that before
      tiles[temp].walls &= ~TILE_MONSTER;
      tiles[voisin(temp, dir)].walls |= TILE_MONSTER;          
      move();
      dir |= 0x10;
      return true;
    }
    if (++dir > 3)
      dir = 0;
  }
  dir |= 0x20; // couldn't move
  return false;
}

void Monster::reInit () {
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

bool checkMonsterCollision(void) { //between a robot and a monster
  Player * pp;
  pp = &p1;
  for (uint8_t j = 0; j < 2; j++ ) {
    for (uint8_t i = 0; i < monstersPlaying; i++) {
      if ((pp->x == monsters[i].x) && (pp->y == monsters[i].y) && ((monsters[i].dir&DEAD) != DEAD)) {
        pp->dir |= DEAD;
        return true;
      }
    }
    pp = &p2;
  }
  return false;
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

void checkCrush(uint8_t ind, uint8_t what){
  // crushing dead monsters body (no matter who did this)
  for (uint8_t i = 0; i < monstersPlaying; i++) {
    if ((monsters[i].dir&DEAD) == DEAD) {
      if (getIndice(monsters[i].x,monsters[i].y)==ind){
        monsters[i].y=182; //six feet under...
        return;
      }
    }
  }
}

