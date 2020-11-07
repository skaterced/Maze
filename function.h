#ifndef function_h
#define function_h
#include <Arduboy2.h>
//#include <Arduino.h>
/*
#include "globals.h"
#include "tiles.h"
#include "robot.h"
*/
int getIndice(uint8_t x, uint8_t y);

bool canGoTo(uint8_t ind, uint8_t direction, uint8_t what);
void inGameMenu(bool test, int test1, int test2);
#endif
