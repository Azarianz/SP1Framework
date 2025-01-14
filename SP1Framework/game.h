#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"

extern CStopWatch g_swTimer;
extern bool g_bQuitGame;

// struct to store keyboard events
// a small subset of KEY_EVENT_RECORD
struct SKeyEvent
{
    bool keyDown;
    bool keyReleased;
};

// struct to store mouse events
// a small subset of MOUSE_EVENT_RECORD
struct SMouseEvent
{
    COORD mousePosition;
    DWORD buttonState;
    DWORD eventFlags;
};

// Enumeration to store the control keys that your game will have
enum EKEYS
{
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_ESCAPE,
    K_SPACE,
    K_ENTER,
    K_BACK,
    K_A, K_B, K_C, K_D, K_E, K_F, K_G, K_H, K_I,
    K_J, K_K, K_L, K_M, K_N, K_O, K_P, K_Q, K_R,
    K_S, K_T, K_U, K_V, K_W, K_X, K_Y, K_Z,
    K_LSHIFT,
    K_COUNT
};

// Enumeration for the different screen states
enum EGAMESTATES
{
    S_SPLASHSCREEN,
    S_GAME,
    S_COUNT,
};

// struct for the game character
struct SGameChar
{
    COORD m_cLocation;
    bool  m_bActive;
};

void init        ( void );      // initialize your variables, allocate memory, etc
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory

void resetGame(void);

void splashScreenWait();    // waits for time to pass in splash screen
void updateGame();          // gameplay logic
void moveCharacter();       // moves the character, collision detection, physics, etc
void moveEnemy();
void moveEnemy2();
void moveEnemy3();
void renderRock();
void Srock();
void moveBoss();
void processUserInput();    // checks if you should change states or do something else with the game, e.g. pause, exit
void clearScreen();         // clears the current screen and draw from scratch 
void renderSplashScreen();  // renders the splash screen
bool renderGame();          // renders the game stuff
void renderMap();           // renders the map to the buffer first
void renderCharacter();     // renders the character into the buffer
void renderEnemy();
void renderEnemy2();
void renderEnemy3();
void renderBoss();
void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game
void renderInputEvents();   // renders the status of input events
void renderEnemy();         // renders the enemy into the buffer
void Enemymovement();       // moves enemy, collision detection, physics
void renderBullet();
void renderBossBullet();
void RbulletEnemy();
void RbulletEnemy2();
void RbulletEnemy3();
void EnemyBMove();
void EnemyBMove2();
void EnemyBMove3();
void BulletMove();
void RockMove();
void bossBulletMove();
void checkCollision();
void renderGameInfo();
void destroyEnemy();

void renderSpecial();
void movePowerUp();
void checkKilled();

//

void rMultishot();
void mMultishot();
void cMultishot();

//Shield
void rShield();
void cShield();

//Health
void cHealth();

//Bomb
void cBomb();

//Player Model
void rPModel();

//Destruction
void checkDestroyed();

//Dodge
void Dodge();

void renderDifficulty(void);
// here are the added functions for UI/UX

void consoleBG(void); // this changes the console color

void update2(double dt); //zero waiting time for menu, name input, and score page
void splashScreenWait2(void);

void resetTimer(void); // resets the timer and scrn state for when the user enters a new game from menu
void resetName(void); // resets the name input : used when user exits name menu/game and win/lose
void resetClass(void); // this is used to reset the 6th class so the user can use it again
void setInfo(void); // initialises the into into a score class

void render2(void); // for rendering menu : functions for menu
void gameTitle(void); // prints out the game title
int renderMenu(void); // will be inside render2
void render3(void); // for rendering score : functions for scores
bool renderScore(void); // will be inside render3
void render4(void); // for rendering the page for player to input name
int renderName(void); // will be inside render4 : functions for name page
void renderUI(void); // renders out the UI (name, time, etc)
void render5(void); // this renders out the win/lose screen
bool renderResult(void); //this renders the win/lose screen

void initScore(void); // this is used to write into the file for name and score
void outScore(void); // this is used to read from the file


// keyboard and mouse input event managers
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent);  // define this function for the console to call when there are keyboard events
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);      // define this function for the console to call when there are mouse events

void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent);   // handles keyboard events for gameplay 
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent); // handles mouse events for gameplay 


#endif // _GAME_H