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

#define LEFTBORDERP1 48
#define LEFTBORDERP2 10

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
int leftBorder=LEFTBORDERP1;
int upBorder=2;
//int adjSelectX=0; //no more other games... todo: clean
//int adjSelectY=0;

int cursX=4;
int cursY=30;

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

const uint8_t message[] ={0,0,0,0,0,0,
                   176,0,0,0,208,48, //J
                   160,0,0,0,0,160,
                   192,80,80,80,80,96,
                   0,0,0,0,0,0,
                   144,80,80,80,80,48, //O
                   160,0,0,0,0,160,
                   192,80,80,80,80,96,
                   0,0,0,0,0,0,
                   208,80,48,0,0,0, //Y
                   80,80,128,80,80,112,
                   208,80,96,0,0,0,
                   0,0,0,0,0,0,
                   144,80,16,80,80,48, //E
                   160,176,224,144,48,160,
                   224,0,0,0,0,224,
                   0,0,0,0,0,0,
                   208,80,80,80,80,48, //U
                   0,0,0,0,0,160,
                   208,80,80,80,80,96,
                   0,0,0,0,0,0,
                   208,80,48,96,80,112, //X
                   80,80,128,32,80,80,
                   208,80,144,192,80,112,
                   0,0,0,0,0,0,
                   0,0,0,0,0,0,
                   0,144,80,16,80,112,//A
                   208,32,240,160,208,80,
                   0,192,80,64,80,112,
                   0,0,0,0,0,0,
                   144,16,80,80,80,112,  //N
                   48,192,48,208,16,16,
                   64,112,192,48,64,64,
                   208,80,80,64,80,112,
                   0,0,0,0,0,0,
                   144,16,80,80,80,112,  //N
                   48,192,48,208,16,16,
                   64,112,192,48,64,64,
                   208,80,80,64,80,112,
                   0,0,0,0,0,0,
                   208,80,80,80,80,112, //I
                   0,0,0,0,0,0,
                   /*208,80,80,16,48,0, //V
                   80,80,112,0,0,112,
                   208,80,80,64,96,0,
                   0,0,0,0,0,0,
                   144,80,16,80,80,48, //E
                   160,176,224,144,48,160,
                   224,0,0,0,0,224,
                   0,0,0,0,0,0,
                   144,80,16,80,80,112, //R
                   160,240,160,240,0,0,
                   192,80,96,0,240,0,
                   0,0,0,0,0,240,
                   144,80,48,0,0,176, //S
                   160,0,160,0,0,160,
                   224,0,192,80,80,96,
                   0,0,0,0,0,0,
                   0,144,80,16,80,112,//A
                   208,32,240,160,208,80,
                   0,192,80,64,80,112,                  
                   0,0,0,0,0,0,
                   208,80,80,80,80,112, //I
                   0,0,0,0,0,0,
                   144,80,16,80,80,112, //R
                   160,240,160,240,0,0,
                   192,80,96,0,240,0,
                   0,0,0,0,0,240,
                   0,0,0,0,0,0,
                   144,80,16,80,80,48, //E
                   160,176,224,144,48,160,
                   224,0,0,0,0,224,*/
                   
                   0,0,0,0,0,0,
                   176,0,0,0,208,48, //J
                   160,0,0,0,0,160,
                   192,80,80,80,80,96,
                   0,0,0,0,0,0,
                   144,80,16,80,80,112, //P
                   160,240,160,0,0,0,
                   192,80,96,0,0,0,
                   0,0,0,0,0,0,/*
                   208,80,80,80,80,48, //L
                   0,0,0,0,0,160,
                   0,0,0,0,0,224,
                   0,0,0,0,0,0,
                   208,80,80,80,80,48, //U
                   0,0,0,0,0,160,
                   208,80,80,80,80,96,
                   0,0,0,0,0,0,
                   144,80,80,80,80,48, //C
                   160,0,0,0,0,160,
                   224,0,0,0,0,224,
                   0,0,0,0,0,0,*/                   
                   208,80,80,112,0,240, //!
                   0,0,0,0,0,0,
                   208,80,80,112,0,240, //!
                   0,0,0,0,0,0,
                   208,80,80,112,0,240, //!
                   0,0,0,0,0,0
                                    }; //def?

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
    move(uint8_t direction, uint8_t walls){
      dir=direction;
      switch (dir){
        case HAUT:
          if ((y>=casesHeight)&&((walls&WALL_UP)!=WALL_UP))
            y-=casesHeight;
        break;
        case BAS:
          if ((y<((casesRow-1)*casesHeight))&&((walls&WALL_DOWN)!=WALL_DOWN))
            y+=casesHeight;
        break;
        case DROITE:
          if (((x<((casesCol-1)*casesLength))&&((walls&WALL_RIGHT)!=WALL_RIGHT))||((x<0)&&(y==31)))
            x+=casesLength;
        break;
       case GAUCHE:
          if (((x>=(casesLength))&&((walls&WALL_LEFT)!=WALL_LEFT))||((x>70)&&(y==31)))
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
class Player p1(-9,31);
class Player p2(71,31);

#endif
