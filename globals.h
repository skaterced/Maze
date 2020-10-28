#ifndef globals_h
#define globals_h

#include <Arduboy2.h>
//#include <stdlib.h>

Arduboy2 arduboy;

//todo clean those define
#define MENU 0
#define MENU2 1
#define MAZE 4
#define WOB 0 //1 for White on Black... I think I'll stick to Black on White

#define HAUT 3
#define DROITE 2
#define BAS 1
#define GAUCHE 0
#define WALL_UP 0x80
#define WALL_RIGHT 0x40
#define WALL_DOWN 0x20
#define WALL_LEFT 0x10
#define WALL_EXPLOSION 0x08
#define WALL_OQP 0x04
#define WALL_BLOCK 0x02
#define WALL_SOLID_BLOCK 0x01
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

#define BOMB_RANGE_MAX 3
#define NB_BOMB_MAX 3
#define BOMB_DEFAULT_T 9

/*
const char* weaponList[] PROGMEM = {
  "Mine",
  "Fist"
};
*/
//uint8_t scoreTimer=0;
uint8_t timer=0;
uint8_t difficulty=2;  //must be between 1-4
uint8_t game=MENU;
//int temp=0;
bool p1Playing=true; //false: p2's turn
bool selected=false; //MILL and MEMO - when a player has selected something - ReflX True: bold arrow
int8_t selectedI=-1;
bool removing=false;
//bool blink=true;
//uint8_t blinkTimer=BLINK_TIMER_INIT;
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


#endif
