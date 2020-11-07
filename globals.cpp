#include "globals.h"
#define globals_h

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

uint8_t cursX=2;
uint8_t cursY=10;
uint8_t movesInit=4;
uint8_t movesLeft=movesInit/2;
bool monstersMoved = false;
uint8_t monstersPlaying=1;
bool versus=false;
//uint8_t stoneArray [81];

