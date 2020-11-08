/*  Welcome to  ** Bomber Maze **. It might still evolve but for now it is a 2 players turn based Bomberman 
 *      
      Menu:
        To be defined
        
      CONTROLS : 
          Arrows: move your Robot
          B: drop a Bomb (for now)
          A+Left/Right: Turn the tile couter-clockwise/clockwise
          A+Up/Down: (not yet but...) Change the B button effect          
          
      Hope you'll enjoy !   

      known bug: 
        -if you go on a monster, he will even. move
        -dead monsters body keep "screaming" when a bomb explode

befor:
Sketch uses 16058 bytes (56%) of program storage space. Maximum is 28672 bytes.
Global variables use 1459 bytes (56%) of dynamic memory, leaving 1101 bytes for local variables. Maximum is 2560 bytes.
after spliting:
Sketch uses 16124 bytes (56%) of program storage space. Maximum is 28672 bytes.
Global variables use 1459 bytes (56%) of dynamic memory, leaving 1101 bytes for local variables. Maximum is 2560 bytes.

*/
#include <Arduboy2.h>
#include "menu.h" 
#include "globals.h"
#include "function.h" 
#include "robot.h"
#include "monster.h"
#include "tiles.h"

#define NBGAMES 8

Monster monsters[NB_MONSTER_MAX];

uint8_t timeUnit=5;
uint8_t test=0;

void setup() { // SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS  Setup SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
  ab.begin();
  ab.clear();
  ab.setFrameRate(60);
  ab.initRandomSeed();  
}

void mazeInit(void){
  randomTiles(20, twoPlayersMode, true ); //symmetric if 2P //border
  //todo? random player starts
  movesLeft=movesInit/2;
  monstersInit();
  monsters[0].type=1; //test
  p1.init(true);
  if (twoPlayersMode){
    p2.init(false);
  }
  else {
    p2.x=p2.y=99;
  }
  hold=false;
}
void loop() { // -------------------------  Init loop -------------------------------------------------------------------------

  if (!(ab.nextFrame()))
    return;
    
  ab.clear(); 
  timer++;
  ab.pollButtons();    
    
  if (MENU==game){
    if (mainMenu())
      mazeInit();
  }
  else if (OPTION==game){      
    optionMenu();
  }
  else if (CREDIT==game){      
    credit();
  }
  
  else if (MAZE==game){  // _____________________|     |___________| Maze |___________|    |______________________________|
      //ab.clear();
     //test
    //p1.score=90;
    //p2.score=-10;   
    //inGameMenu(true, p1.score, p2.score); //"test mode" if true
    
    inGameMenu(false, 0,0); //"test mode" if true
    
    ab.fillRect(leftBorder-2,0,84,64,1);
  
    drawTiles();
  
    //SelectorManagment();
    //drawSelector(getIndice(cursX,cursY));
  
    if (!hold){    
      bool temp=true;
      controlRobot(); // check buttons and play robot's action
      for (uint8_t i = 0; i < monstersPlaying; i++) {
        if ((monsters[i].dir&DEAD) != DEAD) {  
          temp=false;
          break;
        }        
      }
      if (temp){
          timer=0;
          if (monstersPlaying<NB_MONSTER_MAX){
            monstersPlaying++;
          }
          game=BETWEEN_GAMES;          
        }
    }
    else {
      checkMoving();
      if (timer==HOLD_THRESHOLD){ //to prevent a player to play one move too much (and use one of his opponent's move)
        //hold=false;
        //ticks (robot has done one action)    
        checkMonsterCollision();
        if (checkBombs()){        
          //hold=true;
          for (uint8_t i=0; i<monstersPlaying; i++){
            if (TILE_EXPLODING==(tiles[getIndice(monsters[i].x,monsters[i].y)].walls&TILE_EXPLODING)){
              monsters[i].dir=DEAD;
              p1.score+=SCORE_MONSTER;
              p2.score+=SCORE_MONSTER;
            }
          }
          if (TILE_EXPLODING==(tiles[getIndice(p1.x,p1.y)].walls&TILE_EXPLODING))
            p1.dir=DEAD;
          if (TILE_EXPLODING==(tiles[getIndice(p2.x,p2.y)].walls&TILE_EXPLODING))
            p2.dir=DEAD;
          timer++; //skip a monster's turn
          if((--movesLeft==0)&&(twoPlayersMode)){  //retest because of the bomb skip...
            movesLeft=movesInit;
            p1Playing=!p1Playing;
            //timer+=3;
          }
        }
        else {
          controlMonsters();
        }
      }
      else if(timer==HOLD_THRESHOLD+1){
        controlMonsters();
        if (!checkMonsterCollision()){
          if((--movesLeft==0)&&(twoPlayersMode)){  //if changing turn, wait another few seconds
            movesLeft=movesInit;
            p1Playing=!p1Playing;
            timer+=3;
          }
          else {
            timer+=8;
            //hold=false;
          }
        }
      }
      else if (timer==HOLD_THRESHOLD+10){ //bomb has finished exploding 
        /*if ((DEAD!=p1.dir)&&(DEAD!=p2.dir)){ //check if someone died
          hold=false;
        }*/
        if ((DEAD==p1.dir)||((DEAD==p2.dir)&&(twoPlayersMode))) { //check who's dead and inc (or dec if coop) score
          if (twoPlayersMode){
            if (!versus){
              p1.lives--; 
              if(DEAD==p1.dir){
                p1.score-=10;
              }      
              else {                           
                p2.score-=10;   
              }
            }
            else{
              p1.lives--;
              if(DEAD==p1.dir)
                p2.score+=10;            
              else 
                p1.score+=10;
            }
          }
          else{
            p1.lives--;
          }
        }
        else {
          hold=false;
        }
      } 
      else if (timer>HOLD_THRESHOLD+40){
        timer=0;
        game=BETWEEN_GAMES;
      }
    }// end of "if Hold"
          
    drawMonsters();
    p1.drawBombs();
    if (twoPlayersMode){
      p2.drawBombs();
      p2.draw(false);
    }
    p1.draw(true);    
  }
  
  else if (BETWEEN_GAMES==game){
    bool GO=drawScore();
    if (timer>100){
      if (GO){
        game=MENU;    
      }
      else{
        game=MAZE;
        mazeInit();
      }
    }
  }
  else {
    ab.setCursor(0,0);
    ab.println("please recompile with");
    ab.println("this game included");
    ab.println("(#include xxx_h)");
  }
  ab.display();
}
