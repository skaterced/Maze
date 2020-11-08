
#include "globals.h"
#include "menu.h"

bool mainMenu(void) {
  /*
  ab.drawChar(1,1,82,1,0,3); //ROBOT MAZE
  ab.drawChar(17,1,79,1,0,3);
  ab.drawChar(33,1,66,1,0,3);
  ab.drawChar(49,1,79,1,0,3);
  ab.drawChar(65,1,84,1,0,3);
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
    ab.setCursor(22,15);
    ab.print(F("C" "\x82" "dric Martin\n"));
    ab.setCursor(39,25);
    ab.print(F("present"));    
  }
  
  ab.drawChar(scroll+66,25,77,1,0,3);
  ab.drawChar(scroll+82,25,65,1,0,3);
  ab.drawChar(scroll+98,25,90,1,0,3);
  ab.drawChar(scroll+114,25,69,1,0,3);

  scroll*=-1;

  ab.drawChar(scroll+1,1,66,1,0,3); //Bomber Maze
  ab.drawChar(scroll+17,1,79,1,0,3);
  ab.drawChar(scroll+33,1,77,1,0,3);
  ab.drawChar(scroll+49,1,66,1,0,3);
  ab.drawChar(scroll+65,1,69,1,0,3);
  ab.drawChar(scroll+81,1,82,1,0,3);

  if (hold){
    ab.setCursor(0,55);
    switch (cursX){
      case 1:
        ab.print(F("      1P Start  ->"));
        break;
      case 2:
        ab.print(F("  <-  2P Start  ->"));
        break;
      case 3:
        ab.print(F("  <-  Option"));  
        break;
/*
       case 1:
        ab.print(F("       Credit -> Star"));
        break;
      case 2:
        ab.print(F("edit <- Start -> Opti"));
        break;
      case 3:
        ab.print(F("tart <- Option"));  
        break;
 */
    }
  }
  
  if (ab.justPressed(LEFT_BUTTON))
  {
    if (cursX>1)
      cursX--;
  } 
    if (ab.justPressed(RIGHT_BUTTON))
  {
    if (cursX<3)
      cursX++;
  }    
  if (ab.justPressed(A_BUTTON))
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
  
  ab.drawChar(0,cursY,27,1,0,1);
  ab.drawChar(4,cursY,26,1,0,1);
  ab.setCursor(10,10);
  ab.print(F("2P mode : "));
  ab.print(versus? "VS":"Coop");
  ab.setCursor(10,20);
  ab.print(F("Starting Lvl : "));
  ab.print(monstersPlaying);
  // random bomb timer?
  
  //cheat
/*  ab.print(difficulty);
  ab.setCursor(10,20);
  ab.print("Controls : " )
  //    ab.print(forEmulator ? "PC":"Arduboy");
  ab.setCursor(0,55);
  ab.print("WW.Github.com/skaterced");*/
  ab.setCursor(1,55);
  ab.print("A: Change    B: Back");
  
  //ab.drawChar(0,cursY*10+10,16,1,0,1);


  if (ab.justPressed(DOWN_BUTTON))
  {
    if (cursY<40){
      cursY+=10;
    }
  }
  if (ab.justPressed(UP_BUTTON))
  {
    if (cursY>0){
      cursY-=10;
    }
  }
  if ((ab.justPressed(A_BUTTON))||(ab.justPressed(LEFT_BUTTON))||(ab.justPressed(RIGHT_BUTTON)))
  {
    switch (cursY){
      case 10:
        versus=!versus;
      break;
      case 20:
        if (ab.justPressed(LEFT_BUTTON)){
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
  if (ab.justPressed(B_BUTTON))
  {
    game=MENU;
    ab.clear();
  }     
}

void credit (void){
  ab.setCursor(0,55);
  ab.print("W.Github.com/skaterced"); 

}

bool drawScore(){
  bool GO=false; //(Game Over)
  if ((0==p1.lives)||(0==p2.lives)){
    GO=true;
    ab.print(F("Game Over")); 
  }
  else {
    ab.print(F("lives : "));
    ab.print(p1.lives); 
  }
  ab.setCursor(42,20);
  ab.print(F("Level: ")); 
  ab.print(monstersPlaying);  
  ab.setCursor(42,30);
  ab.print(F("score:")); 
  
  if (twoPlayersMode){
    ab.setCursor(20,40);
    ab.print(p1.score);
    ab.setCursor(90,40);
    ab.print(p2.score);
  }
  else {
    ab.setCursor(80,30);
    ab.print(p1.score);
  }
return GO;
}

void inGameMenu(bool test, int test1, int test2){
  //int x=p1Playing? (leftBorder-42):(leftBorder+80);
  int x;
  char* s;
  if (p1Playing){
    x=0;
    if (leftBorder<LEFTBORDERP1)
      leftBorder+=(LEFTBORDERP1-leftBorder)/2;      
  }
  else {
    x=87;
    if (leftBorder>LEFTBORDERP2)
      leftBorder-=(leftBorder-LEFTBORDERP2)/2;
  }
  ab.fillRect(x ,0,31,64,0);
  if (test){
    ab.setCursor(x,30);
    ab.print(test1);
    ab.setCursor(x,50);
    ab.print(test2);
  }
  else {
    if (ab.pressed(A_BUTTON)){
      ab.drawChar(x,3,24,1,0,1);
      ab.setCursor(x+5,3);
      ab.print(F(": 2nd"));
      ab.setCursor(x,12);
      ab.print(F("weapon"));
      ab.drawChar(x,25,27,1,0,1);
      ab.drawChar(x+5,25,26,1,0,1);
      ab.setCursor(x+13,25);
      ab.print(F("Turn"));
      ab.setCursor(x,34);
      ab.print(F("Walls"));

      ab.drawChar(x,47,25,1,0,1);    
      ab.setCursor(x+5,47);
      ab.print(F(": End"));
      ab.setCursor(x+15,56);
      ab.print(F("Turn"));
    }
    else {
      if (twoPlayersMode){
        ab.setCursor(x,1);
        ab.print(F("Moves"));
        ab.setCursor(x+35,1);
        ab.print(movesLeft);
      }
      ab.setCursor(x,20);
      ab.print(F("B for:"));
      ab.setCursor(x+5,30);
      ab.print(F("Bomb"));
      ab.setCursor(x,45);
      ab.print(F("hold A"));
      ab.setCursor(x,55);
      ab.print(F("+..."));
    }
  }
}
