#ifndef tiles_h
#define tiles_h

#include <Arduboy2.h>

//#include "globals.h"
//#include "function.h"

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
//    uint8_t walls; //bit 7-4: walls  bit 3: explosion  bit 2: Stuff explosion can go throug  bit 1: stuff that block explosion spreading (but is destroyed)  bit 0: unbreakable stuff
    uint8_t walls;
    Tile(void){}
    Tile(uint8_t walls_){
      //i=i_;
      walls=walls_;
      //selected=false;
    }
    void turn(bool clockWise);
};

//Tile tiles[NBTILES];
extern Tile tiles[48];

void drawTiles(void);
int voisin(uint8_t ind, uint8_t direction);
void imposeWall(uint8_t ind, bool addAndRemove);
void randomTiles (uint8_t randWall, bool sym, bool border);
void scrollTiles(uint8_t *col);
#endif
