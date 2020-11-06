#ifndef globals_h
#define globals_h

#include <Arduboy2.h>
//#include <stdlib.h>

Arduboy2 arduboy;

//todo clean those define
#define MENU 0
#define CREDIT 1
#define MAZE 2
#define OPTION 3
//#define 0 0 //1 for White on Black... I think I'll stick to Black on White

#define HAUT 3
#define DROITE 2
#define BAS 1
#define GAUCHE 0
#define WALL_UP 0x80
#define WALL_RIGHT 0x40
#define WALL_DOWN 0x20
#define WALL_LEFT 0x10
#define TILE_EXPLODING 0x08
#define TILE_BOMB 0x04
#define TILE_MONSTER 0x02
#define TILE_TBD 0x01
#define ROBOT 0
#define MONSTER 1
#define EXPLOSION 2

#define SYMETRIC true
#define NBTILES 48//casesCol*casesRow
#define LEFTBORDERP1 46
#define LEFTBORDERP2 2

#define BLINK_TIMER_INIT 5
//#define BLACK_STONE 1 // (P1)
//#define WHITE_STONE 2 // (P2)
#define HOLD_THRESHOLD 10
#define DEAD 0x0F

#define BETWEEN_ROBOTS 5 //Frame difference between Robot1 & Robot2
#define WEAPON_MINE 0
#define WEAPON_PUNCH 1

#define BOMB_RANGE_MAX 3
#define NB_BOMB_MAX 3
#define NB_MONSTER_MAX 10
#define BOMB_DEFAULT_T 9

/*
const char* weaponList[] PROGMEM = {
  "Mine",
  "Fist"
};
*/
//uint8_t scoreTimer=0;
uint8_t timer=0;
bool hold=false;
//uint8_t difficulty=2;  //must be between 1-4
uint8_t game=MENU;
//int temp=0;
bool p1Playing=true; //false: p2's turn
bool twoPlayersMode = true;
uint8_t casesLength=10;
uint8_t casesHeight=10;
uint8_t casesCol=8;
uint8_t casesRow=6;
int leftBorder=LEFTBORDERP1;
int upBorder=2;

//int adjSelectX=0; //no more other games... todo: clean
//int adjSelectY=0;

int cursX=2;
int cursY=30;
uint8_t movesInit=4;
uint8_t movesLeft=movesInit/2;
bool monstersMoved = false;
uint8_t nbMonstersPlaying=7;
//uint8_t stoneArray [81];


#endif
