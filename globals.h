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

#define BLINK_TIMER_INIT 5
//#define BLACK_STONE 1 // (P1)
//#define WHITE_STONE 2 // (P2)

#define BETWEEN_ROBOTS 8 //Frame difference between Robot1 & Robot2

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
uint8_t casesLength=8;
uint8_t casesHeight=11;
uint8_t casesCol=7+difficulty;
uint8_t casesRow=2+difficulty;
int leftBorder=30;
int upBorder=-1;
//int adjSelectX=0; //no more other games... todo: clean
//int adjSelectY=0;

int cursX=50;
int cursY=8;

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
0xff,0xff,0x81,0x9d,0xdd,0x9d,0x81,0xff};


class Player {
  public :
    int x,y;
    uint8_t weapon;
    uint8_t dir;
    int score;
    Player(int X, int Y) {
      this->x=X;
      this->y=Y;
      this->weapon=0;  //
      this->dir=0;      
      this->score=0;
    }
    draw(bool player1, bool WhiteOnBlack){
      int frame=WhiteOnBlack? 0:4;
      frame+=dir;
      if (!player1){
        frame+=BETWEEN_ROBOTS;
      }
      Sprites::drawOverwrite(x+leftBorder, y+upBorder, robots, frame);
    }
    move(uint8_t direction){
      dir=direction;
      switch (dir){
        case HAUT:
          if (y>=casesHeight)
            y-=casesHeight;
        break;
        case BAS:
          if (y<((casesRow-1)*casesHeight))
            y+=casesHeight;
        break;
        case DROITE:
          if (x<((casesCol-1)*casesLength))
            x+=casesLength;
        break;
       case GAUCHE:
          if (x>=(casesLength))
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
class Player p1(4,30);
class Player p2(132,30);

#endif
