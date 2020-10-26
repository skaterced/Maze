#ifndef globals_h
#define globals_h

#include <Arduboy2.h>
//#include <stdlib.h>

Arduboy2 arduboy;

//todo clean those define
#define MENU 0
#define MENU2 1
#define PONG 2
#define TRACE 3
#define MAZE 4
#define MEMO 5
#define MILL 6
#define GO 7
#define CHESS 8
#define REFLX 9

#define HAUT 3
#define DROITE 2
#define BAS 1
#define GAUCHE 0

#define WOB 0 //1 for White on Black
#define WALL_UP 0x80
#define WALL_RIGHT 0x40
#define WALL_DOWN 0x20
#define WALL_LEFT 0x10
#define SYMETRIC true
#define NBTILES 48//casesCol*casesRow
#define LEFTBORDERP1 46
#define LEFTBORDERP2 2

#define BLINK_TIMER_INIT 5
//#define BLACK_STONE 1 // (P1)
//#define WHITE_STONE 2 // (P2)

#define BETWEEN_ROBOTS 8 //Frame difference between Robot1 & Robot2

#define WEAPON_MINE 0
#define WEAPON_PUNCH 1

#define BOMB_RANGE_MAX 2
/*
const char* weaponList[] PROGMEM = {
  "Mine",
  "Fist"
};
*/
  //Controls designed for Arduboy
bool forEmulator = false;  // ToDo : remove this and change Menu. No more need for this bool
uint8_t P1_LEFT=UP_BUTTON;
uint8_t  P1_RIGHT=DOWN_BUTTON;
uint8_t  P2_RIGHT=B_BUTTON;
uint8_t  P2_LEFT=A_BUTTON;

uint8_t scoreTimer=0;
int timer=0;
uint8_t difficulty=2;  //must be between 1-4
uint8_t game=MENU;
//int temp=0;
bool p1Playing=true; //false: p2's turn
bool selected=false; //MILL and MEMO - when a player has selected something - ReflX True: bold arrow
int8_t selectedI=-1;
bool removing=false;
bool blink=true;
uint8_t blinkTimer=BLINK_TIMER_INIT;
uint8_t casesLength=10;
uint8_t casesHeight=10;
uint8_t casesCol=8;
uint8_t casesRow=6;
int leftBorder=LEFTBORDERP1;
int upBorder=2;

//int adjSelectX=0; //no more other games... todo: clean
//int adjSelectY=0;

int cursX=4;
int cursY=30;
uint8_t movesInit=4;
uint8_t movesLeft=movesInit/2;
//uint8_t stoneArray [81];

const unsigned char robots[] PROGMEM = {
// width, height,
8, 8,
//WoB
//R1_0-3
0x00,0x00,0x7c,0x2a,0x29,0x62,0x7c,0x00,
0x00,0x00,0x7c,0x2a,0x69,0x2a,0x7c,0x00,
0x00,0x00,0x7c,0x62,0x29,0x2a,0x7c,0x00,
0x00,0x00,0x7c,0x22,0x61,0x22,0x7c,0x00,
// BoW
//R_1_0-3
0xff,0xff,0x83,0xd5,0xd6,0x9d,0x83,0xff,
0xff,0xff,0x83,0xd5,0x96,0xd5,0x83,0xff,
0xff,0xff,0x83,0x9d,0xd6,0xd5,0x83,0xff,
0xff,0xff,0x83,0xdd,0x9e,0xdd,0x83,0xff,
//WoB
//R2_0-3
0x00,0x00,0x7e,0x2a,0x62,0x22,0x7e,0x00,
0x00,0x00,0x7e,0x62,0x2a,0x62,0x7e,0x00,
0x00,0x00,0x7e,0x22,0x62,0x2a,0x7e,0x00,
0x00,0x00,0x7e,0x62,0x22,0x62,0x7e,0x00,
//R_2_0-3
0xff,0xff,0x81,0xd5,0x9d,0xdd,0x81,0xff,
0xff,0xff,0x81,0x9d,0xd5,0x9d,0x81,0xff,
0xff,0xff,0x81,0xdd,0x9d,0xd5,0x81,0xff,
0xff,0xff,0x81,0x9d,0xdd,0x9d,0x81,0xff
};

/*
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
      if (0!=(walls&0x0F)){
        if (0x08==(walls&0x08)){
          arduboy.drawChar(x+2,y+1,88,0,1,1); //"X" for nox
        }
        else {
          arduboy.drawCircle(x+4,y+4,2,0);
        }        
      }
      //return false;
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

 */
//Tile tiles[NBTILES];

class Player {
  public :
    int x,y;
    uint8_t weaponSel;
    uint8_t dir;
    int score;
    Player(int X, int Y) {
      this->x=X;
      this->y=Y;
      this->weaponSel=0;  //
      this->dir=0;      
      this->score=0;
    }
    void draw(bool player1, bool WhiteOnBlack){
      int frame=WhiteOnBlack? 0:4;
      frame+=dir;
      if (!player1){
        frame+=BETWEEN_ROBOTS;
      }
      Sprites::drawOverwrite(x+leftBorder, y+upBorder, robots, frame);
    }
    void move(uint8_t direction){
      switch (direction){
        case HAUT:
          y-=casesHeight;
        break;
        case BAS:         
          y+=casesHeight;
        break;
        case DROITE:         
          x+=casesLength;
        break;
       case GAUCHE:
          x-=casesLength;
        break;
      }
    }
};

/*void Robot_Affiche(bool p1_){  //why not in player now that there are no other games...
  int frame=0;
  if (p1_)
    Sprites::drawOverwrite(p1.x, p1.y, robots, frame+p1.dir);
  else
    Sprites::drawOverwrite(p2.x,p2.y , robots, frame+BETWEEN_ROBOTS+p2.dir );
}
*/
class Player p1(1,31);
class Player p2(71,31);

#endif
