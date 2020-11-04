#ifndef menu_h
#define menu_h

#include "globals.h"
#include "robot.h"
#define MENU_SCROLL 254

bool mainMenu(void) {
  /*
  arduboy.drawChar(1,1,82,1,0,3); //ROBOT MAZE
  arduboy.drawChar(17,1,79,1,0,3);
  arduboy.drawChar(33,1,66,1,0,3);
  arduboy.drawChar(49,1,79,1,0,3);
  arduboy.drawChar(65,1,84,1,0,3);
  */
  int scroll;
  if ((timer<=MENU_SCROLL)&&(false==hold)){
    scroll=timer-MENU_SCROLL;
  }
  else {
    scroll=0;
    hold=true;
  }
  
  if ((!hold)&&(timer<=MENU_SCROLL-25)){
    arduboy.setCursor(22,15);
    arduboy.print(F("C" "\x82" "dric Martin\n"));
    arduboy.setCursor(39,25);
    arduboy.print(F("present"));    
  }
  
  arduboy.drawChar(scroll+66,25,77,1,0,3);
  arduboy.drawChar(scroll+82,25,65,1,0,3);
  arduboy.drawChar(scroll+98,25,90,1,0,3);
  arduboy.drawChar(scroll+114,25,69,1,0,3);

  scroll*=-1;

  arduboy.drawChar(scroll+1,1,66,1,0,3); //Bomber Maze
  arduboy.drawChar(scroll+17,1,79,1,0,3);
  arduboy.drawChar(scroll+33,1,77,1,0,3);
  arduboy.drawChar(scroll+49,1,66,1,0,3);
  arduboy.drawChar(scroll+65,1,69,1,0,3);
  arduboy.drawChar(scroll+81,1,82,1,0,3);

  if (hold){
    arduboy.setCursor(0,55);
    switch (cursX){
      case 1:
        arduboy.print(F("      1P Start  ->"));
        break;
      case 2:
        arduboy.print(F("  <-  2P Start  ->"));
        break;
      case 3:
        arduboy.print(F("  <-  Option"));  
        break;
/*
       case 1:
        arduboy.print(F("       Credit -> Star"));
        break;
      case 2:
        arduboy.print(F("edit <- Start -> Opti"));
        break;
      case 3:
        arduboy.print(F("tart <- Option"));  
        break;
 */
    }
  }
  
  if (arduboy.justPressed(LEFT_BUTTON))
  {
    if (cursX>1)
      cursX--;
  } 
    if (arduboy.justPressed(RIGHT_BUTTON))
  {
    if (cursX<3)
      cursX++;
  }    
  if (arduboy.justPressed(A_BUTTON))
  {
    if (hold){
      game=cursX;
      if (game<3){
        if (1==game){
          game=2;
          twoPlayersMode=false;
        }
        randomSeed((int)timer*37); 
        return true;     
      }
    }
    else {
      hold=true;
    }
  }
  return false;
}
  
void optionMenu(void){      
  arduboy.setCursor(10,10);
  arduboy.print("Difficulty : ");
  arduboy.print(difficulty);
  arduboy.setCursor(10,20);
  arduboy.print("Controls : " );
//    arduboy.print(forEmulator ? "PC":"Arduboy");
  arduboy.setCursor(0,55);
  arduboy.print("WW.Github.com/skaterced");
  arduboy.setCursor(1,40);
  arduboy.print("A: Change    B: Back");
  
  arduboy.drawChar(0,cursY*10+10,16,1,0,1);


  if (arduboy.justPressed(DOWN_BUTTON))
  {
    if (cursY<1){
      cursY++;
    }
  }
  if (arduboy.justPressed(UP_BUTTON))
  {
    if (cursY>0){
      cursY--;
    }
  }
  if (arduboy.justPressed(A_BUTTON))
  {
    switch (cursY){
      case 0:
      
        if (++difficulty==5)
          difficulty=1;
      break;
    }
    
  }
  if (arduboy.justPressed(B_BUTTON))
  {
    game=MENU;
    arduboy.clear();
  }     
}

void credit (void){
  arduboy.setCursor(0,55);
  arduboy.print("W.Github.com/skaterced"); 

}
   
#endif
