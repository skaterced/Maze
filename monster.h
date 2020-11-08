#ifndef monster_h
#define monster_h
#include <Arduboy2.h>

#define MONSTER_TYPE_GIANT 0
#define MONSTER_TYPE_BOMB 1

const unsigned char PROGMEM monstre_bitmap[] = {
  // width, height,
  8, 8,
  0x0f, 0xc7, 0x03, 0x81, 0x81, 0x03, 0xc7, 0x0f,
  0xf0, 0xe7, 0x03, 0x85, 0x81, 0x03, 0xe7, 0xf0
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
    void draw(void);
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
      }
    }
    bool firstStep(void);
    void reInit ();
};

extern Monster monsters[NB_MONSTER_MAX];

bool checkMonsterCollision(void);
bool controlMonsters(); //return false if every monsters are dead
void monstersInit(void);
void drawMonsters();
void checkCrush(uint8_t ind, uint8_t what);
#endif
