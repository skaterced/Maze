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

      known bug: -if you go on a monster, he will ev. move
*/

#include "menu.h" 
#include "globals.h"
#include "function.h" 
#include "robot.h"
#include "monster.h"

#define NBGAMES 8

uint8_t timeUnit=5;
uint8_t test=0;

void setup() { // SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS  Setup SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
  arduboy.begin();
  arduboy.clear();
  arduboy.setFrameRate(60);
  arduboy.initRandomSeed();  
}

void mazeInit(void){
  cursX=50;//need curs?
  cursY=8;
  randomTiles(20, twoPlayersMode, true ); //symmetric if 2P //border
  //todo random player starts
  movesLeft=movesInit/2;
  //monster.dir=0;  Todo reInit monsters
  //monster.x=31;
  monstersInit();
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

  if (!(arduboy.nextFrame()))
    return;
    
  arduboy.clear(); 
  timer++;
  arduboy.pollButtons();    
    
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
      //arduboy.clear();
     //test
    p1.score=90;
    p2.score=-10;   
    //inGameMenu(true, p1.score, p2.score); //"test mode" if true
    
    inGameMenu(false, 0,0); //"test mode" if true
    
    arduboy.fillRect(leftBorder-2,0,84,64,1);
  
    drawTiles();
  
    //SelectorManagment();
    //drawSelector(getIndice(cursX,cursY));
  
    if (!hold){    
      controlRobot(); // check buttons and play robot's action    
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
            if (TILE_EXPLODING==(tiles[getIndice(monsters[i].x,monsters[i].y)].walls&TILE_EXPLODING))
              monsters[i].dir=DEAD;
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
        else if (!controlMonsters()){ //check if false (all monsters dead)
            if (monstersPlaying<NB_MONSTER_MAX){
              monstersPlaying++;
            }
            mazeInit();
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
        if ((DEAD!=p1.dir)&&(DEAD!=p2.dir)){ //check if someone died
          hold=false;
        }
        else { //check who's dead anc inc (or dec if coop) score
          
        }
      }
      else if (timer>90){
        mazeInit();
      }
    }
    if ((timer>50)&&(timer<91)&&hold){
      drawScore();
    }
    else {  
      drawMonsters();
      p1.drawBombs();
      if (twoPlayersMode){
        p2.drawBombs();
        p2.draw(false);
      }
      p1.draw(true);  
    }
  }
  
  else {
    arduboy.setCursor(0,0);
    arduboy.println("please recompile with");
    arduboy.println("this game included");
    arduboy.println("(#include xxx_h)");
  }
  arduboy.display();
}
