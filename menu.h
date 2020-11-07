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
        p1.lives=3;
        p1.score=p2.score=0;
        if (1==game){
          game=2;
          twoPlayersMode=false;
          p2.lives=3;
          p2.x=90;
          p2.dir=0;
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
  
  arduboy.drawChar(0,cursY,27,1,0,1);
  arduboy.drawChar(4,cursY,26,1,0,1);
  arduboy.setCursor(10,10);
  arduboy.print(F("2P mode : "));
  arduboy.print(versus? "VS":"Coop");
  arduboy.setCursor(10,20);
  arduboy.print(F("Starting Lvl : "));
  arduboy.print(monstersPlaying);
  // random bomb timer?
  
  //cheat
/*  arduboy.print(difficulty);
  arduboy.setCursor(10,20);
  arduboy.print("Controls : " )
  //    arduboy.print(forEmulator ? "PC":"Arduboy");
  arduboy.setCursor(0,55);
  arduboy.print("WW.Github.com/skaterced");*/
  arduboy.setCursor(1,55);
  arduboy.print("A: Change    B: Back");
  
  //arduboy.drawChar(0,cursY*10+10,16,1,0,1);


  if (arduboy.justPressed(DOWN_BUTTON))
  {
    if (cursY<40){
      cursY+=10;
    }
  }
  if (arduboy.justPressed(UP_BUTTON))
  {
    if (cursY>0){
      cursY-=10;
    }
  }
  if ((arduboy.justPressed(A_BUTTON))||(arduboy.justPressed(LEFT_BUTTON))||(arduboy.justPressed(RIGHT_BUTTON)))
  {
    switch (cursY){
      case 10:
        versus=!versus;
      break;
      case 20:
        if (arduboy.justPressed(LEFT_BUTTON)){
          if (--monstersPlaying==0)
          monstersPlaying=NB_MONSTER_MAX;  
        }
        else {
          if (++monstersPlaying>NB_MONSTER_MAX)
            monstersPlaying=1;
        }
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

bool drawScore(){
  bool GO=false;
  if ((0==p1.lives)||(0==p2.lives)){
    GO=true;
    arduboy.print(F("Game Over")); 
  }
  else {
    arduboy.print(F("lives : "));
    arduboy.print(p1.lives); 
  }
  arduboy.setCursor(42,20);
  arduboy.print(F("Level: ")); 
  arduboy.print(monstersPlaying);  
  arduboy.setCursor(42,30);
  arduboy.print(F("score:")); 
  
  if (twoPlayersMode){
    arduboy.setCursor(20,40);
    arduboy.print(p1.score);
    arduboy.setCursor(80,40);
    arduboy.print(p2.score);
  }
  else {
    arduboy.setCursor(80,30);
    arduboy.print(p1.score);
  }

return GO;
}
   
#endif
