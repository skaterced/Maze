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
//void checkCrush(uint8_t ind, uint8_t what);

#endif


/*
void SelectorManagment(void){
    if (ab.justPressed(UP_BUTTON)){
      if (cursY>=upBorder+casesHeight){
        cursY-=casesHeight;
      }
    }
    if (ab.justPressed(DOWN_BUTTON)){
      if (cursY<(upBorder+(casesRow-1)*casesHeight)){ 
        cursY+=casesHeight;
      }
    }
    if (ab.justPressed(RIGHT_BUTTON)){
      if (cursX<(leftBorder+(casesCol-1)*casesLength)){
        cursX+=casesLength;
      }
    }
    if (ab.justPressed(LEFT_BUTTON)){
      if (cursX>=(leftBorder+casesLength)){
        cursX-=casesLength;
      }
    }
}

void drawSelector(int i){
   if (blinkTimer--==0){
    blinkTimer=BLINK_TIMER_INIT;
    blink=!blink;
  }  
  int x=(i%casesCol)*casesLength+leftBorder+4;
  int y=(i/casesCol)*casesHeight+upBorder+4;
  //x+=adjSelectX;
  //y+=adjSelectY;
  ab.drawLine(x-4,y-4,x-2,y-2,blink? 1:0);  // Why not add a variable to adjust the Selector for each game? todo?
  ab.drawLine(x-4,y+4,x-2,y+2,blink? 1:0);
  ab.drawLine(x+2,y+2,x+4,y+4,blink? 1:0);
  ab.drawLine(x+2,y-2,x+4,y-4,blink? 1:0);
}
*/

