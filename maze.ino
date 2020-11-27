/*  Welcome to  ** Bomber Maze **. It might still evolve but for now it is a 2 players turn based Bomberman

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

      todo: 
        - check score...
*/

#include "menu.h"
#include "globals.h"
#include "function.h"
#include "robot.h"
#include "monster.h"

#define NBGAMES 8

uint8_t timeUnit = 5;
uint8_t test = 0;

void setup() { // SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS  Setup SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
  arduboy.begin();
  arduboy.clear();
  arduboy.setFrameRate(60);
  arduboy.initRandomSeed();
}

void mazeInit(void) {
  randomTiles(20, twoPlayersMode, true ); //symmetric if 2P //border
  //todo? random player starts
  movesLeft = movesInit / 2;
  monstersInit();
  //monsters[0].type=1; //test
  p1.init(true);
  if (twoPlayersMode) {
    p2.init(false);
  }
  else {
    p2.x = p2.y = 99;
  }
  for (uint8_t i=0; i<NB_BONUS_MAX; i++){
    bonus[i].type=BONUS_INACTIVE;
    mines[i].active=false; //nbecause they have the same limit
  }
  hold = false;
}
void loop() { // -------------------------  Init loop -------------------------------------------------------------------------

  if (!(arduboy.nextFrame()))
    return;

  arduboy.clear();
  timer++;
  arduboy.pollButtons();

  if (MENU == game) {
    if (mainMenu())
      mazeInit();
  }
  else if (OPTION == game) {
    optionMenu();
  }
  else if (CREDIT == game) {
    credit();
  }

  else if (MAZE == game) { // _____________________|     |___________| Maze |___________|    |______________________________|

/* Timer is a bit of a mess... it triggers a lot of things so I will try to explain it.
 *  When Hold is true (the player just did something):
 *  
 *                                 __HOLD_THRESHOLD
 *                                |
 *  0  1  2  3  4  5  6  7  8  9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30
 *        |     |           |  |  |                                    |   |                                       L__ end of turn
 *        |     |           |  |  |                                     \ /
 *        |     |           |  |  |                                      L____ moving monsters. if collision set Timer back to here: | otherwise, skip to EoT
 *        |     |           |  |  |    ______________________________________________________________________________________________|    
 *        |     |           |  |  |   |
 *        |     |           |  |  |   V
 *        |     |           |  |  |   x   x    x   x   x   x   x   x   _   _   _   _   _   _   _      (dead robot drawing)
 *        |     |           |  |  |
 *        |     |           |  |  |   O   O    O   O   @   @   @   @   @                              (exploding drawing)
 *        |     |           |  |  |                                ^
 *        |     |           |  |  L__ Check Bombs. if no: skip ____|
 *        |     |            \/
 *        |     |             L__ normal timer init
 *        |     |
 *        |     L__ Warp menu (Special case)
 *        L__ Nuke menu (Special case)
 * 
 * 
 */
    inGameMenu(false, 0, 0); //"test mode" if true

    arduboy.fillRect(leftBorder - 2, 0, 84, 64, 1);

    Player * pp= p1Playing? &p1:&p2;

    drawTiles();
    drawBonuses(); //mines included
    drawMonsters();
    p1.drawBombs();
    if (twoPlayersMode) {
      p2.drawBombs();
      p2.draw(false);
    }
    p1.draw(true);
    //SelectorManagment();
    //drawSelector(getIndice(cursX,cursY));

    if (!hold) {
      bool temp = true;
      controlRobot(pp); // check buttons and play robot's action
      for (uint8_t i = 0; i < monstersPlaying; i++) {
        if ((monsters[i].dir & DEAD) != DEAD) {
          temp = false;
          break;
        }
      }
      if (temp) {
        timer = 0;
        if (monstersPlaying < NB_MONSTER_MAX) {
          monstersPlaying++; //next level
        }
        game = BETWEEN_GAMES;
      }
    }
    else {
      checkMoving(); //finish the robot movement if needed
      checkCrush(0, ROBOT); //check for bonuses
      if (timer == HOLD_THRESHOLD) { //to prevent a player to play one move too much (and use one of his opponent's move)
        //hold=false;
        //ticks (robot has done one action)
        checkMonsterCollision();
        if (checkBombs()) {  // (in robot.h...)                 ************************  Bombs ***********
          //hold=true;
          for (uint8_t i = 0; i < monstersPlaying; i++) {
            uint8_t tempI = getIndice(monsters[i].x, monsters[i].y);
            if (TILE_EXPLODING == (tiles[tempI].walls & TILE_EXPLODING)) {
              if ((monsters[i].dir & 0x0F) != DEAD) {
                monsters[i].dir = DEAD;
                p1.score += SCORE_MONSTER;
                p2.score += SCORE_MONSTER;
                if (monsters[i].type == MONSTER_TYPE_BOMB) {
                  explode(tempI, BOMB_RANGE_MAX);
                  timer = HOLD_THRESHOLD-2; // that way there is another checkBombs and then it could lead to a chain reaction of bomb monster
                }
              }
              else
                checkCrush(tempI, EXPLOSION);              
            }
          }
          for (uint8_t i=0; i<NB_MINE_MAX; i++){
            uint8_t tempI=getIndice(mines[i].x,mines[i].y);
            if (mines[i].active&&(TILE_EXPLODING==(tiles[tempI].walls&TILE_EXPLODING))){
              mines[i].active=false;
              explode(tempI,1);
              timer = HOLD_THRESHOLD-2; 
              break;
            }
          }
          for (uint8_t i = 0; i < NB_BONUS_MAX; i++) { //destroy bonuses
            if (TILE_EXPLODING == (tiles[getIndice(bonus[i].x, bonus[i].y)].walls & TILE_EXPLODING)) { //(ind==getIndice(bonus[i].x,bonus[i].y)){
              bonus[i].type = BONUS_INACTIVE;
            }
          }
          if ((TILE_EXPLODING == (tiles[getIndice(p1.x, p1.y)].walls & TILE_EXPLODING)) && (INVINCIBILITY != (p1.dir & INVINCIBILITY)))
            p1.dir = DEAD;
          if ((TILE_EXPLODING == (tiles[getIndice(p2.x, p2.y)].walls & TILE_EXPLODING)) && (INVINCIBILITY != (p2.dir & INVINCIBILITY)))
            p2.dir = DEAD;
        }
        else {
          if (!((DEAD == p1.dir) || ((DEAD == p2.dir) && (twoPlayersMode))))
          timer+=8;         
        }
      }
      else if (timer == HOLD_THRESHOLD + 9) {
        if (!((DEAD == p1.dir) || ((DEAD == p2.dir) && (twoPlayersMode))))
          controlMonsters();
      }
      else if (timer == HOLD_THRESHOLD + 10) {
        if (!((DEAD == p1.dir) || ((DEAD == p2.dir) && (twoPlayersMode)))){
          controlMonsters();
          if (!checkMonsterCollision()) {  //no monster have stepped on a mine (otherwise timer changed)
            if (timer == HOLD_THRESHOLD + 10){
              if (!testEOT(pp))
                timer+=9;
            }
          }
          else{
            timer=HOLD_THRESHOLD+1; //that way there would be a robot dying animation
          }
        }
      }
      else if (timer == HOLD_THRESHOLD + 20) { //bomb has finished exploding
        /*if ((DEAD!=p1.dir)&&(DEAD!=p2.dir)){ //check if someone died
          hold=false;
          }*/
        if ((DEAD == p1.dir) || ((DEAD == p2.dir) && (twoPlayersMode))) { //check who's dead and inc (or dec if coop) score
          if (twoPlayersMode) {
            if (!versus) {
              if (DEAD == p1.dir) {
                p1.score -= 10;
                p1.lives--;
                //p1.dir=0;
              }
              else {
                p2.lives--;
                //p2.dir=0;
                p2.score -= 10;
              }
            }
            else {
              p1.lives--;
              if (DEAD == p1.dir)
                p2.score += 10;
              else
                p1.score += 10;
            }
          }
          else {
            p1.lives--;
          }
        }
        else {
          hold = false;
        }
      }
      else if (timer > HOLD_THRESHOLD + 40) {
        timer = 0;
        game = BETWEEN_GAMES;
      }
      else if (2 == timer) { //    ---------------------------- nuke ---------------->
        uint8_t dirTemp = 0;
        dirTemp = nukeMenu();
        if (dirTemp<10) { //fire !!!    
          pp= p1Playing? &p2:&p1;               
          uint8_t opponentI=getIndice(pp->x,pp->y);
          pp= p1Playing? &p1:&p2;
          pp->weapons-=0x10;
          pp->dir=pp->dir&0xF0;
          pp->dir|=dirTemp;
          Bonus shell= Bonus(pp->x,pp->y,NB_BONUS_TYPE+1+dirTemp);
          int tempI=getIndice(pp->x,pp->y);  //indice of the moving nuke          
          
          while((canGoTo(tempI,dirTemp,MONSTER))&&(tempI!=opponentI)){
            tempI=voisin(tempI,dirTemp);
            shell.draw(false);
            //if (0==i)
              pp->draw(p1Playing);
            switch (dirTemp) {
              case HAUT:
                shell.y -= 10;
                break;
              case BAS:
                shell.y += 10;
                break;
              case DROITE:
                shell.x += 10;
                break;
              case GAUCHE:
                shell.x -= 10;
                break;
            }
            shell.draw(true);
            arduboy.display();
            delay(40);
          }
          if (canGoTo(tempI,dirTemp,EXPLOSION)&&(tempI!=opponentI)){ // just another "step" if we can
            tempI=voisin(tempI,dirTemp);
          }
          tiles[tempI].walls = TILE_EXPLODING; //destroy walls
          imposeWall(tempI,true);
          for (uint8_t j = 0; j < 4; j++) {
            tiles[voisin(tempI, j)].walls |= TILE_EXPLODING;
            tiles[voisin(voisin(tempI, j), ((j+1) > 3) ? 0 : j+1)].walls |= TILE_EXPLODING;
          }
          timer=HOLD_THRESHOLD-1;
        }
        else if(BACK==dirTemp){
          hold=false;
        }
        else timer--;
      }
      else if (4 == timer) { //     warp
        uint8_t temp=warpMenu();
        SelectorManagment();
        drawSelector();
        if (BACK==temp){
          hold=false;
        }
        else timer--;
      }
    }// end of "if Hold"
  }

  else if (BETWEEN_GAMES == game) {
    bool GO = drawScore();
    if (timer > 130) {
      if (GO) {
        game = MENU;
      }
      else {
        game = MAZE;
        mazeInit();
      }
    }
  }
  else {
    arduboy.setCursor(0, 0);
    arduboy.println("please recompile with");
    arduboy.println("this game included");
    arduboy.println("(#include xxx_h)");
  }
  arduboy.display();
}
