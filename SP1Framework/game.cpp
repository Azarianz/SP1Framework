// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <time.h>
#include <algorithm>
#include "scores.h"

using namespace std;

double  g_dElapsedTime;
double  g_dDeltaTime;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;

// Game specific variables here
SGameChar   g_sChar;
SGameChar   Enemy[8];
SGameChar   Enemy2[8];
SGameChar   Enemy3[8];
SGameChar   Rock[8];
SGameChar   BulletTest;
SGameChar   Multishot[2];
SGameChar   BulletEnemy[8];     //Enemy2   Bullet
SGameChar   BulletEnemy2[8];    //Enemy3 L Bullet
SGameChar   BulletEnemy3[8];    //Enemy3 R Bullet
SGameChar   PowerUp;
SGameChar   Shield[8];
SGameChar   Boss;
SGameChar   BossBullet[3];
SGameChar   Destroyed[3];
SGameChar   p_Model[2];

double timeToMove = 5;
double timeToMove2 = 5;
double timeToMove3 = 5;
double timeToMove4 = 5; //not used no rocks
double timeToMove5 = 5;
double timeToMove6 = 5;
double BulletToMove = 5;
double BulletToMove2 = 5;
double BulletToMove3 = 5;
double DodgeCounter = 1;
int  ekilled = 0;
int  score = 0;
int  number = rand() % 37 - 2;
bool PowerEaten = false;
bool shoot = false;
bool pDodge = false;
bool destroyed[4] = { false, false, false, false };
bool play = PlaySound(TEXT("audioWater.wav"), NULL, SND_LOOP | SND_ASYNC);
bool sSound = false;
int  sCount = 0;

int Special;
string SpecialText;

int difficulty = 2; //value = number of enemies to spawn
double start_gameTime = 0;  //Elapsed Time when starting
int life = 10;

//Rock Health
int Rockhealth[8];
//Boss Variables
int bosshealth = 30;
bool moveRight;
bool bossCanMove = false;
double bossTime = 60;
double bossTimeMove = 60;
double bossBulletTime = 60;

//Bullet Spawn Points
COORD bulletPoints[3];

// Variables used for UI/UX
std::string name = ""; // takes in user input name
scores score1[6]; // no of scores that can be kept
int score2[5]; // used to write the score rankings into file
std::string name2[5]; // used to write the name rankings into file
int eventCount = 0; // mitigates the problem of keypresses being pressed twice
int result = 0; // used to determine whether player win or lose

EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state
    
// Console object
Console g_Console(60, 30, "Space Wars");
int xGameSpace = 30;    //X-axis Gamespace

#pragma region Startup

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
    destroyed[0] = false;
    destroyed[1] = false;
    destroyed[2] = false;
    destroyed[3] = false;

    
    sSound = false;
    start_gameTime = 0;
    score = 0;
    life = 3;
    difficulty = 2;
    timeToMove = 5;
    timeToMove2 = 5;
    timeToMove3 = 5;
    timeToMove4 = 5;
    timeToMove5 = 5;
    timeToMove6 = 5;
    BulletToMove = 5;
    BulletToMove2 = 5;
    BulletToMove3 = 5;
    play;   //For Sound

    bosshealth = 30;
    bossCanMove = false;
    bossTime = start_gameTime + 60;
    bossTimeMove = 60;
    bossBulletTime = 60;
    srand((unsigned)time(0));

    // Set precision for floating point output
    g_dElapsedTime = 0.0;    

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    #pragma region BossInit
    //Boss
    Boss.m_cLocation.X = xGameSpace / 2;
    Boss.m_cLocation.Y = g_Console.getConsoleSize().Y - 40;
    Boss.m_bActive = true;

    //Bullet Spawn Points
    BossBullet[0].m_cLocation.X = Boss.m_cLocation.X - 2;   //Left
    BossBullet[0].m_cLocation.Y = Boss.m_cLocation.Y + 4;
    BossBullet[0].m_bActive = false;

    BossBullet[1].m_cLocation.X = Boss.m_cLocation.X + 5;   //Center
    BossBullet[1].m_cLocation.Y = Boss.m_cLocation.Y + 4;
    BossBullet[1].m_bActive = false;

    BossBullet[2].m_cLocation.X = Boss.m_cLocation.X + 13;  //Right
    BossBullet[2].m_cLocation.Y = Boss.m_cLocation.Y + 4;
    BossBullet[2].m_bActive = false;

    for (int i = 0; i < 3; i++)
    {
        BossBullet[i].m_bActive = true;
    }
#pragma endregion
    #pragma region EnemyInit
    //Randomize starting spawn points
    for (int i = 0; i < (sizeof(Enemy) / sizeof(Enemy[0])); i++)
    {
        int number = rand() % (35 - 5) - 5; //from 4 - 34
        Enemy[i].m_cLocation.X = xGameSpace - number;
        Enemy[i].m_cLocation.Y = g_Console.getConsoleSize().Y - 29;

        if (Enemy[i].m_cLocation.X == Enemy2[i].m_cLocation.X)
        {
            Enemy[i].m_cLocation.X = xGameSpace - number;
        }

        if (Enemy[i].m_cLocation.X)

            if (i < difficulty)
            {
                Enemy[i].m_bActive = true;  //Spawns [number] enemy first
            }
            else
            {
                Enemy[i].m_bActive = false; //Hides the rest
            }
    }
    for (int i = 0; i < (sizeof(Enemy2) / sizeof(Enemy2[0])); i++)
    {
        int number = rand() % (35 - 5) - 5;
        Enemy2[i].m_cLocation.X = xGameSpace - number;
        Enemy2[i].m_cLocation.Y = g_Console.getConsoleSize().Y - 30;

        if (Enemy2[i].m_cLocation.X == Enemy[i].m_cLocation.X)
        {
            Enemy[i].m_cLocation.X = xGameSpace - number;
        }

        //if (i < difficulty)
        //{
        //    Enemy2[i].m_bActive = true;  //Spawns [number] enemy first
        //}
        //else
        //{
        //    Enemy2[i].m_bActive = false; //Hides the rest
        //}

        Enemy2[i].m_bActive = true;
        BulletEnemy[i].m_cLocation.X = Enemy2[i].m_cLocation.X;
        BulletEnemy[i].m_cLocation.Y = Enemy2[i].m_cLocation.Y + 1;
        BulletEnemy[i].m_bActive = true;

    }
    for (int i = 0; i < (sizeof(Enemy3) / sizeof(Enemy3[0])); i++)
    {
        int number = rand() % (35 - 5) - 5;
        int numberY = rand() % (40 - 30) - 30;
        Enemy3[i].m_cLocation.X = xGameSpace - number;
        Enemy3[i].m_cLocation.Y = g_Console.getConsoleSize().Y - numberY;

        //if (i < difficulty)
        //{
        //    Enemy3[i].m_bActive = false;  //Spawns [number] enemy first
        //}
        //else
        //{
        //    Enemy3[i].m_bActive = true; //Hides the rest
        //}
        Enemy3[i].m_bActive = true; 

        BulletEnemy2[i].m_cLocation.X = Enemy3[i].m_cLocation.X + 1;
        BulletEnemy2[i].m_cLocation.Y = Enemy3[i].m_cLocation.Y;
        BulletEnemy2[i].m_bActive = true;

        BulletEnemy3[i].m_cLocation.X = Enemy3[i].m_cLocation.X - 1;
        BulletEnemy3[i].m_cLocation.Y = Enemy3[i].m_cLocation.Y;
        BulletEnemy3[i].m_bActive = true;


    }
    for (int i = 0; i < (sizeof(Rock) / sizeof(Rock[0])); i++)
    {
        Rockhealth[i] = 3;
        int number = rand() % 36 - 2;
        Rock[i].m_cLocation.X = xGameSpace - number;
        Rock[i].m_cLocation.Y = g_Console.getConsoleSize().Y - 29;

        
        if (i < difficulty)
        {
            Rock[i].m_bActive = true;  //Spawns [number] enemy first
        }
        else
        {
            Rock[i].m_bActive = false; //Hides the rest
        }


    }
#pragma endregion
    #pragma region PlayerStuffInit
    g_sChar.m_cLocation.X = xGameSpace / 2;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y - 5;
    g_sChar.m_bActive = true;

    BulletTest.m_cLocation.X = g_sChar.m_cLocation.X;
    BulletTest.m_cLocation.Y = g_sChar.m_cLocation.Y - 1;
    BulletTest.m_bActive = false;

    PowerUp.m_bActive = false;

#pragma endregion

    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");

    // remember to set your keyboard handler, so that your functions can be notified of input events
    g_Console.setKeyboardHandler(keyboardHandler);
    g_Console.setMouseHandler(mouseHandler);
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();
}

void resetGame(void)
{
    sSound = false;
    start_gameTime = 0;
    score = 0;
    life = 3;
    difficulty = 2;
    timeToMove = 5;
    timeToMove2 = 5;
    timeToMove3 = 5;
    timeToMove4 = 5;
    timeToMove5 = 5;
    timeToMove6 = 5;
    BulletToMove = 5;
    BulletToMove2 = 5;
    BulletToMove3 = 5;
    play;   //For Sound
}

#pragma endregion

#pragma region Handlers

//--------------------------------------------------------------
// Purpose  : Get all the console input events
//            This function sets up the keyboard and mouse input from the console.
//            We will need to reset all the keyboard status, because no events will be sent if no keys are pressed.
//
//            Remember to set the handlers for keyboard and mouse events.
//            The function prototype of the handler is a function that takes in a const reference to the event struct
//            and returns nothing. 
//            void pfKeyboardHandler(const KEY_EVENT_RECORD&);
//            void pfMouseHandlerconst MOUSE_EVENT_RECORD&);
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput( void )
{
    // resets all the keyboard events
    memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
    // then call the console to detect input from user
    g_Console.readConsoleInput();    
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the keyboard input. Whenever there is a keyboard event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            The KEY_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any keyboard event in the Splashscreen state
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent - reference to a key event struct
// Output   : void
//--------------------------------------------------------------
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent)
{    
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: // don't handle anything for the splash screen
        break;
    case S_GAME: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the mouse input. Whenever there is a mouse event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            For the mouse event, if the mouse is not moved, no event will be sent, hence you should not reset the mouse status.
//            However, if the mouse goes out of the window, you would not be able to know either. 
//
//            The MOUSE_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any mouse event in the Splashscreen state
//            
// Input    : const MOUSE_EVENT_RECORD& mouseEvent - reference to a mouse event struct
// Output   : void
//--------------------------------------------------------------
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{    
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: // don't handle anything for the splash screen
        break;
    case S_GAME: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the keyboard handler in the game state. Whenever there is a keyboard event in the game state, this function will be called.
//            
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    // here, we map the key to our enums
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode)
    {
    case VK_UP: key = K_UP; break;
    case VK_DOWN: key = K_DOWN; break;
    case VK_LEFT: key = K_LEFT; break;
    case VK_RIGHT: key = K_RIGHT; break;
    case VK_SPACE: key = K_SPACE; break;
    case VK_ESCAPE: key = K_ESCAPE; break;
    case VK_RETURN: key = K_ENTER; break;
    case VK_BACK: key = K_BACK; break;
    case 'A': key = K_A; break;
    case 'B': key = K_B; break;
    case 'C': key = K_C; break;
    case 'D': key = K_D; break;
    case 'E': key = K_E; break;
    case 'F': key = K_F; break;
    case 'G': key = K_G; break;
    case 'H': key = K_H; break;
    case 'I': key = K_I; break;
    case 'J': key = K_J; break;
    case 'K': key = K_K; break;
    case 'L': key = K_L; break;
    case 'M': key = K_M; break;
    case 'N': key = K_N; break;
    case 'O': key = K_O; break;
    case 'P': key = K_P; break;
    case 'Q': key = K_Q; break;
    case 'R': key = K_R; break;
    case 'S': key = K_S; break;
    case 'T': key = K_T; break;
    case 'U': key = K_U; break;
    case 'V': key = K_V; break;
    case 'W': key = K_W; break;
    case 'X': key = K_X; break;
    case 'Y': key = K_Y; break;
    case 'Z': key = K_Z; break;
    case VK_SHIFT: key = K_LSHIFT; break;
    }
    // a key pressed event would be one with bKeyDown == true
    // a key released event would be one with bKeyDown == false
    // if no key is pressed, no event would be fired.
    // so we are tracking if a key is either pressed, or released
    if (key != K_COUNT)
    {
        g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the mouse handler in the game state. Whenever there is a mouse event in the game state, this function will be called.
//            
//            If mouse clicks are detected, the corresponding bit for that mouse button will be set.
//            mouse wheel, 
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
    {
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    }
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}
#pragma endregion

#pragma region Update Functions

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
            break;
        case S_GAME: updateGame(); // gameplay logic when we are in the game
            break;
    }
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 3.0) // wait for 3 seconds to switch to game mode, else do nothing
        g_eGameState = S_GAME;
        start_gameTime = g_dElapsedTime;
}

void updateGame()       // gameplay logic
{
    if (start_gameTime != 0) 
    {   
        //Spawn a new enemy everytime boss dies
        if (g_dElapsedTime >= start_gameTime + 45 && g_dElapsedTime < start_gameTime + 90)
        {
            difficulty = 3;
        }
        else if (g_dElapsedTime >= start_gameTime + 90 && g_dElapsedTime < start_gameTime + 135)
        {
            difficulty = 4;
        }
        else if (g_dElapsedTime >= start_gameTime + 135 && g_dElapsedTime < start_gameTime + 180)
        {
            difficulty = 6;
        }
        else if (g_dElapsedTime >= start_gameTime + 180)
        {
            difficulty = 8;
        }
    }

    if (g_dElapsedTime >= bossTime)
    {
        bossCanMove = true;
        bossTime = g_dElapsedTime + 60;
    }

    //Gonna clean this later on if got time
    bulletPoints[0].X = Boss.m_cLocation.X - 2;
    bulletPoints[0].Y = Boss.m_cLocation.Y + 4;

    bulletPoints[1].X = Boss.m_cLocation.X + 5;
    bulletPoints[1].Y = Boss.m_cLocation.Y + 4;

    bulletPoints[2].X = Boss.m_cLocation.X + 13;
    bulletPoints[2].Y = Boss.m_cLocation.Y + 4;

    //processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
    checkCollision();
    moveEnemy();
    moveEnemy2();
    moveEnemy3();
    RockMove();
    checkCollision();
    BulletMove();
    EnemyBMove();
    EnemyBMove2();
    EnemyBMove3();
    checkCollision();
    checkKilled();
    checkDestroyed();

    Dodge();
    movePowerUp();
    mMultishot();
    cMultishot();
    cShield();
    cHealth();
    cBomb();
    moveBoss();
    bossBulletMove();

    if (bosshealth <= 0) 
    {
        //Reset Boss Bullets
        for (int i = 0; i < sizeof(BossBullet) / sizeof(BossBullet[0]); i++)
        {
            BossBullet[i].m_bActive = false; // rendering
            BossBullet[i].m_cLocation = bulletPoints[i]; // resets the position to front of space ship
            BossBullet[i].m_bActive = true; // stops rendering
        }
    }
                        // sound can be played here too.
}

void moveCharacter()
{    
    // Updating the location of the character based on the key release
    // providing a beep sound whenver we shift the character
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.Y > 1)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.Y--;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X > 1)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.X--;        
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 2)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.Y++;        
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 2)
    {
        //Beep(1440, 30);
        
        if (g_sChar.m_cLocation.X == 38)
            g_sChar.m_cLocation.X = g_sChar.m_cLocation.X;
        else
            g_sChar.m_cLocation.X++;
    }
    if (g_skKeyEvent[K_SPACE].keyDown)
    {
        BulletTest.m_bActive = true;
        Multishot[0].m_bActive = true;
        Multishot[1].m_bActive = true;
        shoot = true;
    }
    if (g_skKeyEvent[K_SPACE].keyReleased)
    {
        sSound = true;
    }
    if (g_skKeyEvent[K_LSHIFT].keyReleased)
    {
        pDodge = true;
    }

   
}

void moveEnemy()
{
    if (g_dElapsedTime >= timeToMove) 
    {
        for (int i = 0; i < difficulty; i++) 
        {
            Enemy[i].m_cLocation.Y++;
        }
        timeToMove = g_dElapsedTime + 0.4;
    }
}

void moveEnemy2()
{
    if (g_dElapsedTime >= timeToMove3)
    {
        for (int i = 0; i < difficulty; i++)
        {
            if (difficulty >= 3)
            {
                Enemy2[i].m_cLocation.Y++;
            }
        }

        timeToMove3 = g_dElapsedTime + 0.5;
    }
}

void moveEnemy3()
{
    if (g_dElapsedTime >= timeToMove6)
    {
        for (int i = 0; i < difficulty; i++)
        {
            if (difficulty >= 4) 
            {
                Enemy3[i].m_cLocation.Y++;
            }
        }

        timeToMove6 = g_dElapsedTime + 0.5;
    }
}

void moveBoss()
{
    //Boss Patrol Route
    if (Boss.m_cLocation.X < 6)
    {
        moveRight = true;
    }
    else if (Boss.m_cLocation.X > 22)
    {
        moveRight = false;
    }

    if (g_dElapsedTime >= bossTimeMove)
    {
        bossTimeMove = g_dElapsedTime + 0.2;
        //Move Boss(Probably Relocate or make this function for all enemies)
        if (Boss.m_cLocation.Y > 5)
        {
            if (moveRight == true)
            {
                Boss.m_cLocation.X++;
            }
            else
            {
                Boss.m_cLocation.X--;
            }
        }
        else
        {
            if (bossCanMove == true)
            {
                Boss.m_cLocation.Y++;
            }
        }
    }
}

void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
    {
        g_bQuitGame = true;
    }
    
}

//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    consoleBG();
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: renderSplashScreen();
        break;
    case S_GAME: renderGame();
        break;
    }
    //renderFramerate();      // renders debug information, frame rate, elapsed time, etc
    //renderInputEvents();    // renders status of input events
    //renderGameInfo();
    renderToScreen();       // dump the contents of the buffer to the screen, one frame worth of game
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x1F);
}

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void renderSplashScreen()  // renders the splash screen
{
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - 16;
    g_Console.writeToBuffer(c, "The game will start in 3 seconds", 0x0F);
    c.Y += 2;
    c.X = g_Console.getConsoleSize().X / 2 - 12;
    g_Console.writeToBuffer(c, "Use the Arrow keys to move", 0x0F);
    c.Y += 2;
    c.X = g_Console.getConsoleSize().X / 2 - 9;
    g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x0F);

    c.X = g_Console.getConsoleSize().X / 2 - 8;
    c.Y += 3;
    g_Console.writeToBuffer(c, "Power Ups", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "M : Multishot", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "H : Health Regen", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "B : Bomb", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "S : Shield", 0x0F);
}

bool renderGame()
{
    renderMap();        // renders the map to the buffer first
    consoleBG();
    renderDifficulty();
    renderCharacter();  // renders the character into the buffer
    renderBullet();     //render spaceship bullet
    RbulletEnemy();
    RbulletEnemy2();
    RbulletEnemy3();
    renderEnemy();      //renders the enemy
    renderEnemy2();
    renderEnemy3();
    renderRock();
    rMultishot();
    checkDestroyed();
    rShield();
    renderSpecial();

    if (bossCanMove == true)
    {
        renderBoss();
        renderBossBullet();
    }
    renderUI();
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
    {
        result = 0;
        resetName();
        return false;
    }
    if (life == 0)
    {
        result = 1;
        return true;
    }
}

void renderMap()
{
    //Set up sample colours, and output shadings
    const WORD colors[] = 
    {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

    COORD c;
    //for (int row = 0; row < 40; ++row)
    //{
    //    for (int col = 0; col < 30; ++col) 
    //    {
    //        if (col == 0 || col == 29) 
    //        {
    //            c.X = row;
    //            c.Y = col;
    //            colour(colors[1]);
    //        }
    //        else if (row == 0 || row == 39) 
    //        {
    //            c.X = row;
    //            c.Y = col;
    //            colour(colors[1]);
    //        }

    //        g_Console.writeToBuffer(c, " ", colors[3]); //wat the shit is this
    //    }

    //}

    ifstream file_("Map.txt");
    string line_;
    int rows = 0;
    
    while (getline(file_, line_))
    {
        for (int columns = 0; columns < line_.size(); columns++)
        {
            if (line_[columns] == '1')
            {
                c.X = rows;
                c.Y = columns;
                g_Console.writeToBuffer(c, " ", colors[0]);
            }
            else if (line_[columns] == '0')
            {
                c.X = rows;
                c.Y = columns;
                g_Console.writeToBuffer(c, " ", colors[4]);
            }
        }
        rows++;
    }
    
    //Laser Sight
    int playerSight = g_Console.getConsoleSize().Y + (g_sChar.m_cLocation.Y - 1); //distance from (player.x - 1) to top screen
    for (int i = 0; i < (playerSight - 29); i++)
    {
        COORD thisPos;
        thisPos.X = g_sChar.m_cLocation.X;
        thisPos.Y = g_sChar.m_cLocation.Y - i;
        g_Console.writeToBuffer(thisPos, " ", colors[3]);
    }

}

void renderCharacter()
{
    
    // Draw the location of the character
    WORD charColor = 0x09;
    if (g_sChar.m_bActive)
    {
        charColor = 0x09;
    }
    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)29, charColor);

    rPModel();
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x59);
}

// this is an example of how you would use the input events
void renderInputEvents()
{
    // keyboard events
    COORD startPos = {50, 2};
    std::ostringstream ss;
    std::string key;
    for (int i = 0; i < K_COUNT; ++i)
    {
        ss.str("");
        switch (i)
        {
        case K_UP: key = "UP";
            break;
        case K_DOWN: key = "DOWN";
            break;
        case K_LEFT: key = "LEFT";
            break;
        case K_RIGHT: key = "RIGHT";
            break;
        case K_SPACE: key = "SPACE";
            break;
        default: continue;
        }
        if (g_skKeyEvent[i].keyDown)
            ss << key << " pressed";
        else if (g_skKeyEvent[i].keyReleased)
            ss << key << " released";
        else
            ss << key << " not pressed";

        COORD c = { startPos.X, startPos.Y + i };
        g_Console.writeToBuffer(c, ss.str(), 0x17);
    }

    // mouse events    
    ss.str("");
    ss << "Mouse position (" << g_mouseEvent.mousePosition.X << ", " << g_mouseEvent.mousePosition.Y << ")";
    g_Console.writeToBuffer(g_mouseEvent.mousePosition, ss.str(), 0x59);
    ss.str("");
    switch (g_mouseEvent.eventFlags)
    {
    case 0:
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            ss.str("Left Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 1, ss.str(), 0x59);
        }
        else if (g_mouseEvent.buttonState == RIGHTMOST_BUTTON_PRESSED)
        {
            ss.str("Right Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 2, ss.str(), 0x59);
        }
        else
        {
            ss.str("Some Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 3, ss.str(), 0x59);
        }
        break;
    case DOUBLE_CLICK:
        ss.str("Double Clicked");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 4, ss.str(), 0x59);
        break;        
    case MOUSE_WHEELED:
        if (g_mouseEvent.buttonState & 0xFF000000)
            ss.str("Mouse wheeled down");
        else
            ss.str("Mouse wheeled up");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 5, ss.str(), 0x59);
        break;
    default:        
        break;
    }

    
}

void renderEnemy()
{
    WORD Color = 0x0F;
    for (int i = 0; i < difficulty; i++) 
    {
        g_Console.writeToBuffer(Enemy[i].m_cLocation, (char)10,Color);
    }
}

void renderEnemy2()
{
    WORD Color = 0x0C;
    for (int i = 0; i < difficulty; i++)
    {
        g_Console.writeToBuffer(Enemy2[i].m_cLocation, (char)12,Color);
        if (difficulty >= 3)
        {
            //g_Console.writeToBuffer(Enemy2[i].m_cLocation, (char)20);
            BulletEnemy[i].m_bActive = true;
        }
    }
}

void renderEnemy3()
{
    WORD Color = 0x0E;
    for (int i = 0; i < difficulty; i++)
    {
        g_Console.writeToBuffer(Enemy3[i].m_cLocation, (char)18, Color);
        if (difficulty >= 4)
        {
            BulletEnemy2[i].m_bActive = true;
            BulletEnemy3[i].m_bActive = true;
            /*g_Console.writeToBuffer(Enemy3[i].m_cLocation, (char)19);*/
        }
    }
}

void renderRock()
{
    for (int i = 0; i < difficulty; i++)
    {
        string RockSprite[3] =
        {"  (*)  ",
         "(*****)",
         "  (*)  " };
        COORD RockPivot;
        RockPivot.X = Rock[i].m_cLocation.X;
        RockPivot.Y = Rock[i].m_cLocation.Y;

        if (Rockhealth[i] > 0)
        {
            for (int row = 0; row < sizeof(RockSprite) / sizeof(RockSprite[0]); row++)
            {
                for (int col = 0; col < RockSprite[row].size(); col++)
                {

                    g_Console.writeToBuffer(RockPivot, (RockSprite[row])[col]);

                    if (RockPivot.X == g_sChar.m_cLocation.X && RockPivot.Y == g_sChar.m_cLocation.Y)
                    {
                        life--;
                        g_sChar.m_bActive = false;
                        g_sChar.m_cLocation.X = xGameSpace / 2;
                        g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y - 5;
                        g_sChar.m_bActive = true;
                    }

                    //Player Bullet Collision Detection
                    if (BulletTest.m_bActive == true)
                    {
                        if (BulletTest.m_cLocation.X == RockPivot.X && BulletTest.m_cLocation.Y == RockPivot.Y)
                        {
                            //Damage Boss
                            Rockhealth[i] -= 1;

                            //Bullet Player Collision Reset
                            BulletTest.m_cLocation.X = g_sChar.m_cLocation.X;
                            BulletTest.m_cLocation.Y = g_sChar.m_cLocation.Y - 1; // resets the position to front of space ship
                            BulletTest.m_bActive = false; // stops rendering
        
                            sCount = 1;
                            sSound = false;
                            
                        }
                    }

                    //Multi Shot Power Collision Detection
                    if (PowerEaten == true && PowerUp.m_bActive == false && Special == 1)
                    {
                        for (int i = 0; i < 2; i++)
                        {
                            if (Multishot[i].m_bActive == true)
                            {
                                if ((Multishot[i].m_cLocation.X == RockPivot.X
                                    && Multishot[i].m_cLocation.Y == RockPivot.Y))
                                {
                                    Rockhealth[i] -= 1;

                                    if (i == 0)
                                    {
                                        Multishot[i].m_cLocation.X = BulletTest.m_cLocation.X - 2;
                                        Multishot[i].m_cLocation.Y = BulletTest.m_cLocation.Y - 1;
                                    }
                                    else if (i == 1)
                                    {
                                        Multishot[i].m_cLocation.X = BulletTest.m_cLocation.X + 2;
                                        Multishot[i].m_cLocation.Y = BulletTest.m_cLocation.Y - 1;
                                    }

                                    Multishot[i].m_bActive = false;
                                    sndPlaySound(L"destroy.wav", SND_FILENAME | SND_ASYNC);
                                }

                            }
                        }
                    }

                    RockPivot.X++;
                }

                RockPivot.X = Rock[i].m_cLocation.X;
                RockPivot.Y++;
            }
        }
        else
        {
            Rock[i].m_bActive = false;
            Rock[i].m_cLocation.X = xGameSpace / 2;
            Rock[i].m_cLocation.Y = g_Console.getConsoleSize().Y - 40;
            ekilled++;
            score += 1;
            Rockhealth[i] = 3;

            Rock[i].m_bActive = true;
        }
    }

}

void renderBoss()
{
    string bossSprite[4] =
    {"     (00000000)     ",
     "  (0000UUUUUU0000)  ",
     "(00(  )0(UU)0(  )00)",
     " (U)   (0UU0)   (U) " };

    COORD bossPivot;
    bossPivot.X = Boss.m_cLocation.X - 4;
    bossPivot.Y = Boss.m_cLocation.Y;

    if (bosshealth > 0)
    {
        WORD Color = 0x0D;
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < bossSprite[row].size(); col++)
            {
                g_Console.writeToBuffer(bossPivot, (bossSprite[row])[col], Color);
                bossPivot.X++;

                if (bossPivot.X == g_sChar.m_cLocation.X && bossPivot.Y == g_sChar.m_cLocation.Y) 
                {
                    life--;
                    g_sChar.m_bActive = false;
                    g_sChar.m_cLocation.X = xGameSpace / 2;
                    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y - 5;
                    g_sChar.m_bActive = true;
                }

                //Player Bullet Collision Detection
                if (BulletTest.m_bActive == true)
                {
                    if (BulletTest.m_cLocation.X == bossPivot.X && BulletTest.m_cLocation.Y == bossPivot.Y)
                    {
                        //Damage Boss
                        bosshealth--;

                        //Bullet Player Collision Reset
                        BulletTest.m_cLocation.X = g_sChar.m_cLocation.X;
                        BulletTest.m_cLocation.Y = g_sChar.m_cLocation.Y - 1; // resets the position to front of space ship
                        BulletTest.m_bActive = false; // stops rendering
                        if (sSound == true && sCount == 0)
                        {
                            sndPlaySound(L"shoot.wav", SND_FILENAME | SND_ASYNC);
                            sCount = 1;
                            sSound = false;
                        }
                    }
                }

                //Multi Shot Power Collision Detection
                if (PowerEaten == true && PowerUp.m_bActive == false && Special == 1)
                {
                    for (int i = 0; i < 2; i++)
                    {
                        if (Multishot[i].m_bActive == true)
                        {
                            if ((Multishot[i].m_cLocation.X == bossPivot.X
                                && Multishot[i].m_cLocation.Y == bossPivot.Y))
                            {
                                bosshealth -= 1;

                                if (i == 0)
                                {
                                    Multishot[i].m_cLocation.X = BulletTest.m_cLocation.X - 2;
                                    Multishot[i].m_cLocation.Y = BulletTest.m_cLocation.Y - 1;
                                }
                                else if (i == 1)
                                {
                                    Multishot[i].m_cLocation.X = BulletTest.m_cLocation.X + 2;
                                    Multishot[i].m_cLocation.Y = BulletTest.m_cLocation.Y - 1;
                                }

                                Multishot[i].m_bActive = false;
                            }

                        }
                    }
                }
            }

            bossPivot.X = Boss.m_cLocation.X - 4;
            bossPivot.Y++;
        }
    }
    else
    {
        //Reset Boss Bullets
        for (int i = 0; i < sizeof(BossBullet)/sizeof(BossBullet[0]); i++) 
        {
            BossBullet[i].m_bActive = false; // rendering
            BossBullet[i].m_cLocation = bulletPoints[i]; // resets the position to front of space ship
            BossBullet[i].m_bActive = true; // stops rendering
        }

        //Reset Boss
        Boss.m_bActive = false;
        Boss.m_cLocation.X = xGameSpace / 2;
        Boss.m_cLocation.Y = g_Console.getConsoleSize().Y - 40;
        ekilled++;
        score += 10;
        bosshealth = 30;
        bossCanMove = false;
        Boss.m_bActive = true;
    }
}

void renderBullet()
{
    WORD Color = 0x09;
    if (BulletTest.m_bActive == true)
    {
        if (sSound == true && sCount == 0)
        {
            sndPlaySound(L"shoot.wav", SND_FILENAME | SND_ASYNC);
            sCount = 1;
            sSound = false;
        }
        g_Console.writeToBuffer(BulletTest.m_cLocation, (char)6, Color);
        //BulletTest.m_cLocation.Y++;
    }
}

void renderBossBullet()
{
    WORD Color = 0x0D;
    if (bosshealth > 0)
    {
        for (int i = 0; i < 3; i++)
        {
            if (BossBullet[i].m_bActive == true)
            {
                g_Console.writeToBuffer(BossBullet[i].m_cLocation, (char)6, Color);
                //BossBullet[i].m_cLocation.Y += 25 * g_dElapsedTime;
            }
        }
    }
}

void RbulletEnemy()
{
    for (int i = 0; i < difficulty; i++)
    {
        WORD Color = 0x0C;
        if (difficulty >= 3)
        {
            if (BulletEnemy[i].m_bActive == true)
            {
                g_Console.writeToBuffer(BulletEnemy[i].m_cLocation, (char)25, Color);
                //BulletEnemy[i].m_cLocation.Y += 25 * g_dDeltaTime;
            }
        }
    }
}

void RbulletEnemy2()
{
    WORD Color = 0x0E;
    for (int i = 0; i < difficulty; i++)
    {
        if (difficulty >= 4)
        {
            if (BulletEnemy2[i].m_bActive == true)
            {
                g_Console.writeToBuffer(BulletEnemy2[i].m_cLocation, (char)6,Color);
                //BulletEnemy2[i].m_cLocation.X += 25 * g_dDeltaTime;
            }
        }
    }
}

void RbulletEnemy3()
{
    WORD Color = 0x0E;
    for (int i = 0; i < difficulty; i++)
    {
        if (difficulty >= 4)
        {
            if (BulletEnemy3[i].m_bActive == true)
            {
                g_Console.writeToBuffer(BulletEnemy3[i].m_cLocation, (char)6, Color);
                //BulletEnemy3[i].m_cLocation.X -= 25 * g_dDeltaTime;
            }
        }
    }
}

void bossBulletMove()
{
    if (g_dElapsedTime >= bossBulletTime)
    {
        bossBulletTime = g_dElapsedTime + 0.1;
        if (bosshealth > 0)
        {
            //Normal Enemy Bullet Move
            for (int i = 0; i < 3; i++)
            {
                if (BossBullet[i].m_bActive == true)
                {
                    BossBullet[i].m_cLocation.Y++;

                    if (BossBullet[i].m_cLocation.Y >= 29) //Border Hit
                    {
                        BossBullet[i].m_bActive = false; // rendering
                        BossBullet[i].m_cLocation = bulletPoints[i]; // resets the position to front of space ship
                        BossBullet[i].m_bActive = true; // stops rendering
                    }
                }
                else
                {
                    BossBullet[i].m_cLocation = bulletPoints[i];
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < 3; ++i)
        {
            BossBullet[i].m_cLocation = bulletPoints[i];
        }
    }

}

void EnemyBMove()
{
    if (g_dElapsedTime >= BulletToMove)
    {
        for (int i = 0; i < difficulty; i++)
        {
            if (difficulty >= 3)
            {
                if (BulletEnemy[i].m_bActive == true)
                {
                    BulletEnemy[i].m_cLocation.Y++;

                    if (BulletEnemy[i].m_cLocation.Y >= 29)
                    {
                        // resets the position to front of space ship
                        BulletEnemy[i].m_cLocation.X = Enemy2[i].m_cLocation.X;
                        BulletEnemy[i].m_cLocation.Y = Enemy2[i].m_cLocation.Y + 1; 
                        //BulletEnemy[i].m_bActive = false; // stops renderin
                    }
                }
                else
                {
                    BulletEnemy[i].m_cLocation.X = Enemy2[i].m_cLocation.X;
                    BulletEnemy[i].m_cLocation.Y = Enemy2[i].m_cLocation.Y + 1;
                }
            }
        }

        BulletToMove = g_dElapsedTime + 0.18;
    }
}

void EnemyBMove2()
{
    if (g_dElapsedTime >= BulletToMove2)
    {
        for (int i = 0; i < difficulty; i++)
        {
            if (difficulty >= 4)
            {
                if (BulletEnemy2[i].m_bActive == true)
                {
                    BulletEnemy2[i].m_cLocation.X++;

                    if (BulletEnemy2[i].m_cLocation.X >= 39)
                    {
                        // resets the position to right of space ship
                        BulletEnemy2[i].m_cLocation.X = Enemy3[i].m_cLocation.X + 1;
                        BulletEnemy2[i].m_cLocation.Y = Enemy3[i].m_cLocation.Y; 
                        //BulletEnemy2[i].m_bActive = false; // stops renderin
                    }
                }
                else
                {
                    BulletEnemy2[i].m_cLocation.X = Enemy3[i].m_cLocation.X + 1;
                    BulletEnemy2[i].m_cLocation.Y = Enemy3[i].m_cLocation.Y;
                }
            }
        }

        BulletToMove2 = g_dElapsedTime + 0.15;
    }

}

void EnemyBMove3()
{
    if (g_dElapsedTime >= BulletToMove3)
    {
        for (int i = 0; i < difficulty; i++)
        {
            if (difficulty >= 4)
            {
                if (BulletEnemy3[i].m_bActive == true)
                {
                    BulletEnemy3[i].m_cLocation.X--;

                    if (BulletEnemy3[i].m_cLocation.X <= 0)
                    {
                        BulletEnemy3[i].m_cLocation.X = Enemy3[i].m_cLocation.X - 1;
                        BulletEnemy3[i].m_cLocation.Y = Enemy3[i].m_cLocation.Y; // resets the position to left of space ship
                        //BulletEnemy3[i].m_bActive = false; // stops renderin
                    }
                }
                else
                {
                    BulletEnemy3[i].m_cLocation.X = Enemy3[i].m_cLocation.X - 1;
                    BulletEnemy3[i].m_cLocation.Y = Enemy3[i].m_cLocation.Y;
                }
            }

        }

        BulletToMove3 = g_dElapsedTime + 0.15;
    }
}

void BulletMove()
{
    if (BulletTest.m_bActive == true)
    {
        BulletTest.m_cLocation.Y--;

        if (BulletTest.m_cLocation.Y == 0)
        {

            BulletTest.m_cLocation.X = g_sChar.m_cLocation.X;
            BulletTest.m_cLocation.Y = g_sChar.m_cLocation.Y - 1; // resets the position to front of space ship
            BulletTest.m_bActive = false; // stops rendering
            sCount = 0;
        }

    }
    else
    {
        BulletTest.m_cLocation.X = g_sChar.m_cLocation.X;
        BulletTest.m_cLocation.Y = g_sChar.m_cLocation.Y - 1;
    }
  
}
void RockMove()
{
    if (g_dElapsedTime >= timeToMove4)
    {
        timeToMove4 = g_dElapsedTime + 0.8;

        for (int i = 0; i < difficulty; i++)
        {
            Rock[i].m_cLocation.Y++;


        }

    }
}
void checkCollision()
{
    WORD Color = 0x1AF;
    //for player bullets
    if (BulletTest.m_bActive == true)
    {
        for (int i = 0; i < difficulty; i++)
        {
            if (BulletTest.m_cLocation.X == Enemy[i].m_cLocation.X && BulletTest.m_cLocation.Y == Enemy[i].m_cLocation.Y)
            {
                destroyed[0] = true;

                Destroyed[0].m_cLocation.X = Enemy[i].m_cLocation.X;
                Destroyed[0].m_cLocation.Y = Enemy[i].m_cLocation.Y;



                Enemy[i].m_bActive = false;
                Enemy[i].m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5; //from 4 - 34
                Enemy[i].m_cLocation.Y = g_Console.getConsoleSize().Y - 29;
                Enemy[i].m_bActive = true;

                BulletTest.m_cLocation.X = g_sChar.m_cLocation.X;
                BulletTest.m_cLocation.Y = g_sChar.m_cLocation.Y - 1; // resets the position to front of space ship
                BulletTest.m_bActive = false; // stops rendering

                ekilled++;

                if (PowerUp.m_bActive == false && PowerEaten == true && Special == 4)
                {
                    ekilled--;
                }

                
                score++;
                sCount = 0;

                sndPlaySound(L"destroy.wav", SND_FILENAME | SND_ASYNC);
            }
            if (BulletTest.m_cLocation.X == Enemy2[i].m_cLocation.X && BulletTest.m_cLocation.Y == Enemy2[i].m_cLocation.Y)
            {
                destroyed[1] = true;

                Destroyed[1].m_cLocation.X = Enemy2[i].m_cLocation.X;
                Destroyed[1].m_cLocation.Y = Enemy2[i].m_cLocation.Y;

                Enemy2[i].m_bActive = false;
                Enemy2[i].m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5;
                Enemy2[i].m_cLocation.Y = g_Console.getConsoleSize().Y - 32;
                Enemy2[i].m_bActive = true;

                BulletTest.m_cLocation.X = g_sChar.m_cLocation.X;
                BulletTest.m_cLocation.Y = g_sChar.m_cLocation.Y - 1; // resets the position to front of space ship
                BulletTest.m_bActive = false; // stops rendering

                ekilled++;
                score++;
                sCount = 0;

                if (PowerUp.m_bActive == false && PowerEaten == true && Special == 4)
                {
                    ekilled--;
                }

                sndPlaySound(L"destroy.wav", SND_FILENAME | SND_ASYNC);
            }
            if (BulletTest.m_cLocation.X == Enemy3[i].m_cLocation.X && BulletTest.m_cLocation.Y == Enemy3[i].m_cLocation.Y)
            {
                destroyed[2] = true;

                Destroyed[2].m_cLocation.X = Enemy3[i].m_cLocation.X;
                Destroyed[2].m_cLocation.Y = Enemy3[i].m_cLocation.Y;

                Enemy3[i].m_bActive = false;
                Enemy3[i].m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5;
                Enemy3[i].m_cLocation.Y = g_Console.getConsoleSize().Y - rand() % (30 - 25) - 25;
                Enemy3[i].m_bActive = true;

                BulletTest.m_cLocation.X = g_sChar.m_cLocation.X;
                BulletTest.m_cLocation.Y = g_sChar.m_cLocation.Y - 1; // resets the position to front of space ship
                BulletTest.m_bActive = false; // stops rendering

                ekilled++;

                if (PowerUp.m_bActive == false && PowerEaten == true && Special == 4)
                {
                    ekilled--;
                }

                score++;
                sndPlaySound(L"destroy.wav", SND_FILENAME | SND_ASYNC);
            }

            checkDestroyed();
        }

    }       

    //for enemy bullet hit player collision
    for (int i = 0; i < difficulty; i++)
    {
        if (difficulty >= 3)
        {
            if (BulletEnemy[i].m_bActive == true)
            {
                if ((BulletEnemy[i].m_cLocation.X == g_sChar.m_cLocation.X && BulletEnemy[i].m_cLocation.Y == g_sChar.m_cLocation.Y) ||
                    (BulletEnemy[i].m_cLocation.X == g_sChar.m_cLocation.X - 1 && BulletEnemy[i].m_cLocation.Y == g_sChar.m_cLocation.Y) ||
                    (BulletEnemy[i].m_cLocation.X == g_sChar.m_cLocation.X + 1 && BulletEnemy[i].m_cLocation.Y == g_sChar.m_cLocation.Y))
                {
                    life -= 1;
                    g_sChar.m_bActive = false;
                    g_sChar.m_cLocation.X = xGameSpace / 2;
                    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y - 5;
                    g_sChar.m_bActive = true;

                    BulletEnemy[i].m_bActive = false;
                    BulletEnemy[i].m_cLocation.X = Enemy2[i].m_cLocation.X;
                    BulletEnemy[i].m_cLocation.Y = Enemy2[i].m_cLocation.Y + 1;
                    BulletEnemy[i].m_bActive = true;
                }
            }
            if (BulletEnemy2[i].m_bActive == true)
            {
                if ((BulletEnemy2[i].m_cLocation.X == g_sChar.m_cLocation.X && BulletEnemy2[i].m_cLocation.Y == g_sChar.m_cLocation.Y) ||
                    (BulletEnemy2[i].m_cLocation.X == g_sChar.m_cLocation.X - 1 && BulletEnemy2[i].m_cLocation.Y == g_sChar.m_cLocation.Y) ||
                    (BulletEnemy2[i].m_cLocation.X == g_sChar.m_cLocation.X + 1 && BulletEnemy2[i].m_cLocation.Y == g_sChar.m_cLocation.Y))
                {
                    life -= 1;
                    g_sChar.m_bActive = false;
                    g_sChar.m_cLocation.X = xGameSpace / 2;
                    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y - 5;
                    g_sChar.m_bActive = true;


                    BulletEnemy2[i].m_bActive = false;
                    BulletEnemy2[i].m_cLocation.X = Enemy2[i].m_cLocation.X;
                    BulletEnemy2[i].m_cLocation.Y = Enemy2[i].m_cLocation.Y + 1;
                    BulletEnemy2[i].m_bActive = true;
                }
            }
            if (BulletEnemy3[i].m_bActive == true)
            {
                if ((BulletEnemy3[i].m_cLocation.X == g_sChar.m_cLocation.X && BulletEnemy3[i].m_cLocation.Y == g_sChar.m_cLocation.Y) ||
                    (BulletEnemy3[i].m_cLocation.X == g_sChar.m_cLocation.X - 1 && BulletEnemy3[i].m_cLocation.Y == g_sChar.m_cLocation.Y) ||
                    (BulletEnemy3[i].m_cLocation.X == g_sChar.m_cLocation.X + 1 && BulletEnemy3[i].m_cLocation.Y == g_sChar.m_cLocation.Y))
                {
                    life -= 1;
                    g_sChar.m_bActive = false;
                    g_sChar.m_cLocation.X = xGameSpace / 2;
                    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y - 5;
                    g_sChar.m_bActive = true;

                    BulletEnemy3[i].m_bActive = false;
                    BulletEnemy3[i].m_cLocation.X = Enemy2[i].m_cLocation.X;
                    BulletEnemy3[i].m_cLocation.Y = Enemy2[i].m_cLocation.Y + 1;
                    BulletEnemy3[i].m_bActive = true;
                }
            }
        }
    }

    //Enemy Out-Of-Bounds
    for (int i = 0; i < difficulty; i++)
    {
        if (Enemy[i].m_cLocation.Y >= g_Console.getConsoleSize().Y - 1)
        {
            Enemy[i].m_bActive = false;
            Enemy[i].m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5; //from 4 - 34
            Enemy[i].m_cLocation.Y = g_Console.getConsoleSize().Y - 29;
            Enemy[i].m_bActive = true;
        }

        if (Enemy2[i].m_cLocation.Y >= g_Console.getConsoleSize().Y - 1)
        {
            Enemy2[i].m_bActive = false;
            Enemy2[i].m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5;
            Enemy2[i].m_cLocation.Y = g_Console.getConsoleSize().Y - 32;
            Enemy2[i].m_bActive = true;
        }

        if (Enemy3[i].m_cLocation.Y >= g_Console.getConsoleSize().Y - 1)
        {
            //Enemy3[i].m_bActive = false;
            Enemy3[i].m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5; //from 4 - 34
            Enemy3[i].m_cLocation.Y = g_Console.getConsoleSize().Y - rand() % (35 - 30) - 30;
            //Enemy3[i].m_bActive = true;
        }

    }

    //Boss Bullet Player Collision
    for (int i = 0; i < sizeof(BossBullet) / sizeof(BossBullet[0]); i++)
    {
        if (BossBullet[i].m_cLocation.X == g_sChar.m_cLocation.X && BossBullet[i].m_cLocation.Y == g_sChar.m_cLocation.Y)
        {
            life--;
            g_sChar.m_bActive = false;
            g_sChar.m_cLocation.X = xGameSpace / 2;
            g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y - 5;
            g_sChar.m_bActive = true;
            BossBullet[i].m_bActive = false;
            BossBullet[i].m_cLocation = bulletPoints[i]; //reset to bullet spawn point
            BossBullet[i].m_bActive = true;
        }
    }  

    //Player Collision with Enemy
    for (int i = 0; i < difficulty; i++)
    {
        if ((g_sChar.m_cLocation.X == Enemy[i].m_cLocation.X && g_sChar.m_cLocation.Y == Enemy[i].m_cLocation.Y)||
            (g_sChar.m_cLocation.X - 1 == Enemy[i].m_cLocation.X && g_sChar.m_cLocation.Y == Enemy[i].m_cLocation.Y)||
            (g_sChar.m_cLocation.X + 1 == Enemy[i].m_cLocation.X && g_sChar.m_cLocation.Y == Enemy[i].m_cLocation.Y))
        {
            life -= 1;
            g_sChar.m_bActive = false;
            g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
            g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y - 5;
            g_sChar.m_bActive = true;
            sndPlaySound(L"pdeath.wav", SND_FILENAME | SND_ASYNC);
        }
        if ((g_sChar.m_cLocation.X == Enemy2[i].m_cLocation.X && g_sChar.m_cLocation.Y == Enemy2[i].m_cLocation.Y)||
            (g_sChar.m_cLocation.X - 1 == Enemy2[i].m_cLocation.X && g_sChar.m_cLocation.Y == Enemy2[i].m_cLocation.Y)||
            (g_sChar.m_cLocation.X + 1 == Enemy2[i].m_cLocation.X && g_sChar.m_cLocation.Y == Enemy2[i].m_cLocation.Y))
        {
            if (Enemy2[i].m_bActive == true)
            {
                life -= 1;
                g_sChar.m_bActive = false;
                g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
                g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y - 5;
                g_sChar.m_bActive = true;
                sndPlaySound(L"pdeath.wav", SND_FILENAME | SND_ASYNC);
            }
        }
        if ((g_sChar.m_cLocation.X == Enemy3[i].m_cLocation.X && g_sChar.m_cLocation.Y == Enemy3[i].m_cLocation.Y)||
            (g_sChar.m_cLocation.X - 1 == Enemy3[i].m_cLocation.X && g_sChar.m_cLocation.Y == Enemy3[i].m_cLocation.Y)||
            (g_sChar.m_cLocation.X + 1 == Enemy3[i].m_cLocation.X && g_sChar.m_cLocation.Y == Enemy3[i].m_cLocation.Y))
        {
            if (Enemy3[i].m_bActive == true)
            {
                life -= 1;
                g_sChar.m_bActive = false;
                g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
                g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y - 5;
                g_sChar.m_bActive = true;
                sndPlaySound(L"pdeath.wav", SND_FILENAME | SND_ASYNC);
            }
        }

        //Rock Collision is inside Rock Function
    }
}

void renderGameInfo()
{
    COORD info;

    info.X = g_Console.getConsoleSize().X - 15;
    info.Y = g_Console.getConsoleSize().Y - 1;
    g_Console.writeToBuffer(info, "Life: " + to_string(life));

    info.X = g_Console.getConsoleSize().X;
    info.Y = g_Console.getConsoleSize().Y - 2;
    g_Console.writeToBuffer(info, "Difficulty: " + to_string(difficulty));

    info.X = g_Console.getConsoleSize().X;
    info.Y = g_Console.getConsoleSize().Y - 3;
    g_Console.writeToBuffer(info, "Boss Health: " + to_string(bosshealth));

    info.X = g_Console.getConsoleSize().X;
    info.Y = g_Console.getConsoleSize().Y - 4;
    g_Console.writeToBuffer(info, "Score: " + to_string(score));


    info.X = g_Console.getConsoleSize().X;
    info.Y = g_Console.getConsoleSize().Y - 5;
    g_Console.writeToBuffer(info, "BulletEnemy[0] Y: " + to_string(BulletEnemy[0].m_cLocation.Y));

}

void destroyEnemy()
{
    for (unsigned int e = 0; e < 8; ++e)
    {
        Enemy[e].m_bActive = false;
        Enemy2[e].m_bActive = false;
        BulletEnemy[e].m_bActive = false;
        Enemy3[e].m_bActive = false;
        BulletEnemy2[e].m_bActive = false;
        BulletEnemy3[e].m_bActive = false;
        Rock[e].m_bActive = false;


        Enemy[e].m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5;
        Enemy2[e].m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5;
        BulletEnemy[e].m_cLocation.X = Enemy2[e].m_cLocation.X;
        Enemy3[e].m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5;
        BulletEnemy[e].m_cLocation.X = Enemy3[e].m_cLocation.X;
        BulletEnemy[e].m_cLocation.X = Enemy3[e].m_cLocation.X;
        Rock[e].m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5;

        BulletEnemy[e].m_cLocation.Y = Enemy2[e].m_cLocation.Y - 1;
        Enemy[e].m_cLocation.Y = g_Console.getConsoleSize().Y - 30;
        Enemy2[e].m_cLocation.Y = g_Console.getConsoleSize().Y - 30;
        Enemy3[e].m_cLocation.Y = g_Console.getConsoleSize().Y - 30;
        Rock[e].m_cLocation.Y = g_Console.getConsoleSize().Y - 30;

        Enemy[e].m_bActive = true;
        Enemy2[e].m_bActive = true;
        Enemy3[e].m_bActive = true;
        Rock[e].m_bActive = true;
        BulletEnemy[e].m_bActive = true;
    }
    Boss.m_bActive = true;
    Boss.m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5;
    Boss.m_cLocation.Y = g_Console.getConsoleSize().Y - 30;
    Boss.m_bActive = false;
    for (unsigned int x = 0; x < 3; ++x)
    {
        BossBullet[x].m_bActive = true;
        BossBullet[x].m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5;
        BossBullet[x].m_cLocation.Y = g_Console.getConsoleSize().Y - 30;
        BossBullet[x].m_bActive = false;
    }
}

void rMultishot()
{
    WORD Color = 0x09;
    for (int i = 0; i < 2; i++)
    {
        if (Multishot[i].m_bActive == true && PowerEaten == true && PowerUp.m_bActive == false && Special == 1)
        {
            g_Console.writeToBuffer(Multishot[i].m_cLocation, (char)6,Color);

        }
    }
}

void mMultishot()
{
    if (PowerEaten == true && PowerUp.m_bActive == false && Special == 1)
    {
        for (int i = 0; i < 2; i++)
        {
            if (Multishot[i].m_bActive == true && shoot == true)
            {
                Multishot[i].m_cLocation.Y--;
                if (Multishot[i].m_cLocation.Y == 0 || Multishot[i].m_cLocation.Y >= 29)
                {
                    Multishot[i].m_bActive = false;
                    if (i == 0)
                    {
                        Multishot[i].m_cLocation.X = g_sChar.m_cLocation.X - 2;
                        Multishot[i].m_cLocation.Y = g_sChar.m_cLocation.Y - 1;
                    }
                    else if (i == 1)
                    {
                        Multishot[i].m_cLocation.X = g_sChar.m_cLocation.X - 2;
                        Multishot[i].m_cLocation.Y = g_sChar.m_cLocation.Y - 1;
                    }
                }

            }
            else
            {
                if (i == 0)
                {
                    Multishot[i].m_cLocation.X = g_sChar.m_cLocation.X - 2;
                    Multishot[i].m_cLocation.Y = g_sChar.m_cLocation.Y - 1;
                }
                else if (i == 1)
                {
                    Multishot[i].m_cLocation.X = g_sChar.m_cLocation.X + 2;
                    Multishot[i].m_cLocation.Y = g_sChar.m_cLocation.Y - 1;
                }
            }

        }
    }
}

void cMultishot()
{
    if (PowerEaten == true && PowerUp.m_bActive == false && Special == 1)
    {
        for (int i = 0; i < 2; i++)
        {
            if (Multishot[i].m_bActive == true)
            {
                for (int e = 0; e < difficulty; e++)
                {
                    if ((Multishot[i].m_cLocation.X == Enemy[e].m_cLocation.X && Multishot[i].m_cLocation.Y == Enemy[e].m_cLocation.Y) || Multishot[i].m_cLocation.Y <= 0||
                    (Multishot[i].m_cLocation.X == Enemy2[e].m_cLocation.X && Multishot[i].m_cLocation.Y == Enemy2[e].m_cLocation.Y)||
                    (Multishot[i].m_cLocation.X == Enemy3[e].m_cLocation.X && Multishot[i].m_cLocation.Y == Enemy3[e].m_cLocation.Y)||
                    (Multishot[i].m_cLocation.X == Rock[e].m_cLocation.X && Multishot[i].m_cLocation.Y == Rock[e].m_cLocation.Y))                             
                    {
                        if (Multishot[i].m_cLocation.X == Enemy[e].m_cLocation.X && Multishot[i].m_cLocation.Y == Enemy[e].m_cLocation.Y)
                        {
                            destroyed[0] = true;

                            Destroyed[0].m_cLocation.X = Enemy[e].m_cLocation.X;
                            Destroyed[0].m_cLocation.Y = Enemy[e].m_cLocation.Y;

                            Enemy[e].m_bActive = false;

                            Enemy[e].m_cLocation.X = xGameSpace - rand() % 38 - 2;
                            Enemy[e].m_cLocation.Y = g_Console.getConsoleSize().Y - 29;
                            Enemy[e].m_bActive = true;

                            ekilled++;
                            score++;
                            sndPlaySound(L"destroy.wav", SND_FILENAME | SND_ASYNC);
                        }

                        if (Multishot[i].m_cLocation.X == Enemy2[e].m_cLocation.X && Multishot[i].m_cLocation.Y == Enemy2[e].m_cLocation.Y)
                        {
                            destroyed[1] = true;

                            Destroyed[1].m_cLocation.X = Enemy2[e].m_cLocation.X;
                            Destroyed[1].m_cLocation.Y = Enemy2[e].m_cLocation.Y;

                            Enemy2[e].m_bActive = false;

                            Enemy2[e].m_cLocation.X = xGameSpace - rand() % 38 - 2;
                            Enemy2[e].m_cLocation.Y = g_Console.getConsoleSize().Y - 29;
                            Enemy2[e].m_bActive = true;

                            ekilled++;
                            score++;
                            sndPlaySound(L"destroy.wav", SND_FILENAME | SND_ASYNC);

                        }

                        if (Multishot[i].m_cLocation.X == Enemy3[e].m_cLocation.X && Multishot[i].m_cLocation.Y == Enemy3[e].m_cLocation.Y)
                        {
                            destroyed[2] = true;

                            Destroyed[2].m_cLocation.X = Enemy3[e].m_cLocation.X;
                            Destroyed[2].m_cLocation.Y = Enemy3[e].m_cLocation.Y;

                            Enemy3[e].m_bActive = false;

                            Enemy3[e].m_cLocation.X = xGameSpace - rand() % 38 - 2;
                            Enemy3[e].m_cLocation.Y = g_Console.getConsoleSize().Y - 29;
                            Enemy3[e].m_bActive = true;

                            ekilled++;
                            score++;
                            sndPlaySound(L"destroy.wav", SND_FILENAME | SND_ASYNC);
                        }
                        if (Multishot[i].m_cLocation.X == Rock[e].m_cLocation.X && Multishot[i].m_cLocation.Y == Rock[e].m_cLocation.Y)
                        {
                            Rock[e].m_bActive = false;

                            Rock[e].m_cLocation.X = xGameSpace - rand() % 38 - 2;
                            Rock[e].m_cLocation.Y = g_Console.getConsoleSize().Y - 29;
                            Rock[e].m_bActive = true;

                            ekilled++;
                            score++;
                            sndPlaySound(L"destroy.wav", SND_FILENAME | SND_ASYNC);
                        }

                        if (i == 0)
                        {
                            Multishot[i].m_cLocation.X = BulletTest.m_cLocation.X - 2;
                            Multishot[i].m_cLocation.Y = BulletTest.m_cLocation.Y - 1;
                        }
                        else if (i == 1)
                        {
                            Multishot[i].m_cLocation.X = BulletTest.m_cLocation.X + 2;
                            Multishot[i].m_cLocation.Y = BulletTest.m_cLocation.Y - 1;
                        }


                        Multishot[i].m_bActive = false;

                    }
                }

            }
            checkDestroyed();
        }
    }
    else
    {
        Multishot[0].m_cLocation.X = BulletTest.m_cLocation.X - 2;
        Multishot[0].m_cLocation.Y = BulletTest.m_cLocation.Y - 1;

        Multishot[1].m_cLocation.X = BulletTest.m_cLocation.X + 2;
        Multishot[1].m_cLocation.Y = BulletTest.m_cLocation.Y - 1;

    }
}

void rShield()
{
    WORD Color = 0x0E;
    if (PowerEaten == true && PowerUp.m_bActive == false && Special == 4)
    {
        Shield[0].m_cLocation.X = g_sChar.m_cLocation.X - 1;
        Shield[0].m_cLocation.Y = g_sChar.m_cLocation.Y - 1;

        Shield[1].m_cLocation.X = g_sChar.m_cLocation.X;
        Shield[1].m_cLocation.Y = g_sChar.m_cLocation.Y - 1;

        Shield[2].m_cLocation.X = g_sChar.m_cLocation.X + 1;
        Shield[2].m_cLocation.Y = g_sChar.m_cLocation.Y - 1;

        Shield[3].m_cLocation.X = g_sChar.m_cLocation.X - 1;
        Shield[3].m_cLocation.Y = g_sChar.m_cLocation.Y;

        Shield[4].m_cLocation.X = g_sChar.m_cLocation.X + 1;
        Shield[4].m_cLocation.Y = g_sChar.m_cLocation.Y;

        Shield[5].m_cLocation.X = g_sChar.m_cLocation.X - 1;
        Shield[5].m_cLocation.Y = g_sChar.m_cLocation.Y + 1;

        Shield[6].m_cLocation.X = g_sChar.m_cLocation.X;
        Shield[6].m_cLocation.Y = g_sChar.m_cLocation.Y + 1;

        Shield[7].m_cLocation.X = g_sChar.m_cLocation.X + 1;
        Shield[7].m_cLocation.Y = g_sChar.m_cLocation.Y + 1;

        for (int i = 0; i < 8; i++)
        {
            g_Console.writeToBuffer(Shield[i].m_cLocation, 'X',Color);
        }
    }

}

void cShield()
{
    if (PowerEaten == true && PowerUp.m_bActive == false && Special == 4)
    {
        for (int s = 0; s < 8; s++)
        {
            for (int e = 0; e < difficulty; e++)
            {
                if (Shield[s].m_cLocation.X == Enemy[e].m_cLocation.X && Shield[s].m_cLocation.Y == Enemy[e].m_cLocation.Y)
                {
                    destroyed[0] = true;

                    Destroyed[0].m_cLocation.X = Enemy[e].m_cLocation.X;
                    Destroyed[0].m_cLocation.Y = Enemy[e].m_cLocation.Y;

                    Enemy[e].m_bActive = false;

                    Enemy[e].m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5;
                    Enemy[e].m_cLocation.Y = g_Console.getConsoleSize().Y - 30;
                    Enemy[e].m_bActive = true;

                    ekilled++;
                    score++;
                    sndPlaySound(L"deflect.wav", SND_FILENAME | SND_ASYNC);
                }

                if (Shield[s].m_cLocation.X == Enemy2[e].m_cLocation.X && Shield[s].m_cLocation.Y == Enemy2[e].m_cLocation.Y)
                {
                    destroyed[1] = true;

                    Destroyed[1].m_cLocation.X = Enemy2[e].m_cLocation.X;
                    Destroyed[1].m_cLocation.Y = Enemy2[e].m_cLocation.Y;

                    Enemy2[e].m_bActive = false;

                    Enemy2[e].m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5;
                    Enemy2[e].m_cLocation.Y = g_Console.getConsoleSize().Y - 30;
                    Enemy2[e].m_bActive = true;

                    ekilled++;
                    score++;
                    sndPlaySound(L"deflect.wav", SND_FILENAME | SND_ASYNC);
                }

                if (Shield[s].m_cLocation.X == Enemy3[e].m_cLocation.X && Shield[s].m_cLocation.Y == Enemy3[e].m_cLocation.Y)
                {
                    destroyed[2] = true;

                    Destroyed[2].m_cLocation.X = Enemy3[e].m_cLocation.X;
                    Destroyed[2].m_cLocation.Y = Enemy3[e].m_cLocation.Y;

                    Enemy3[e].m_bActive = false;

                    Enemy3[e].m_cLocation.X = xGameSpace - rand() % (35-5) - 5;
                    Enemy3[e].m_cLocation.Y = g_Console.getConsoleSize().Y - 32;
                    Enemy3[e].m_bActive = true;

                    ekilled++;
                    score++;
                    sndPlaySound(L"deflect.wav", SND_FILENAME | SND_ASYNC);
                }

                if (Shield[s].m_cLocation.X == BulletEnemy[e].m_cLocation.X && Shield[s].m_cLocation.Y == BulletEnemy[e].m_cLocation.Y)
                {
                    BulletEnemy[e].m_bActive = false;

                    BulletEnemy[e].m_cLocation.X = Enemy2[e].m_cLocation.X;
                    BulletEnemy[e].m_cLocation.Y = Enemy2[e].m_cLocation.Y - 1;                    
                    BulletEnemy[e].m_bActive = true;

                    ekilled++;
                    sndPlaySound(L"deflect.wav", SND_FILENAME | SND_ASYNC);
                }
                if (Shield[s].m_cLocation.X == Rock[e].m_cLocation.X && Shield[s].m_cLocation.Y == Rock[e].m_cLocation.Y)
                {
                    Rock[e].m_bActive = false;

                    Rock[e].m_cLocation.X = Enemy2[e].m_cLocation.X;
                    Rock[e].m_cLocation.Y = Enemy2[e].m_cLocation.Y - 1;
                    Rock[e].m_bActive = true;

                    ekilled++;
                    sndPlaySound(L"deflect.wav", SND_FILENAME | SND_ASYNC);
                }
            }
            for (int i = 0; i < sizeof(BossBullet) / sizeof(BossBullet[0]); i++)
            {
                if (Shield[s].m_cLocation.X == BossBullet[i].m_cLocation.X
                    && Shield[s].m_cLocation.Y == BossBullet[i].m_cLocation.Y)
                {
                    BossBullet[i].m_bActive = false;
                    BossBullet[i].m_cLocation = bulletPoints[i]; // resets the position to front of space ship
                    BossBullet[i].m_bActive = true; // stops rendering

                    ekilled++;
                    sndPlaySound(L"deflect.wav", SND_FILENAME | SND_ASYNC);
                }
            }
        }
    }
}

void cHealth()
{
    if (PowerEaten == true && Special == 2)
    {
        life = 3;
        ekilled = 0;
        Beep(320, 30);
        Beep(400, 30);
        PowerEaten = false;
    }
}

void cBomb()
{
    if (PowerEaten == true && Special == 3)
    {
        for (int e = 0; e < difficulty; e++)
        {
            destroyed[3] = true;
            Enemy[e].m_bActive = false;
            Enemy2[e].m_bActive = false;
            BulletEnemy[e].m_bActive = false;
            Enemy3[e].m_bActive = false;
            BulletEnemy2[e].m_bActive = false;
            BulletEnemy3[e].m_bActive = false;
            Rock[e].m_bActive = false;
            

            Enemy[e].m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5;
            Enemy2[e].m_cLocation.X = xGameSpace - rand() % (35 - 5)- 5;
            BulletEnemy[e].m_cLocation.X = Enemy2[e].m_cLocation.X;
            Enemy3[e].m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5;
            BulletEnemy[e].m_cLocation.X = Enemy3[e].m_cLocation.X;
            BulletEnemy[e].m_cLocation.X = Enemy3[e].m_cLocation.X;
            Rock[e].m_cLocation.X = xGameSpace - rand() % (35 - 5) - 5;


            BulletEnemy[e].m_cLocation.Y = Enemy2[e].m_cLocation.Y - 1;
            Enemy[e].m_cLocation.Y = g_Console.getConsoleSize().Y - 30;
            Enemy2[e].m_cLocation.Y = g_Console.getConsoleSize().Y - 30;
            Enemy3[e].m_cLocation.Y = g_Console.getConsoleSize().Y - 30;
            Rock[e].m_cLocation.Y = g_Console.getConsoleSize().Y - 30;

            Enemy[e].m_bActive = true;
            Enemy2[e].m_bActive = true;
            Enemy3[e].m_bActive = true;
            Rock[e].m_bActive = true;
            BulletEnemy[e].m_bActive = true;
        }

        if (Boss.m_cLocation.Y < g_Console.getConsoleSize().Y - 40) 
        {
            bosshealth -= 5;
        }

        PowerEaten = false;
        ekilled = 0;
        sndPlaySound(L"dodge.wav", SND_FILENAME | SND_ASYNC);
    }
}

void rPModel()
{
    WORD Color = 0x09;

    p_Model[0].m_cLocation.X = g_sChar.m_cLocation.X - 1; //Left Wing
    p_Model[0].m_cLocation.Y = g_sChar.m_cLocation.Y;

    p_Model[1].m_cLocation.X = g_sChar.m_cLocation.X + 1; // Right Wing
    p_Model[1].m_cLocation.Y = g_sChar.m_cLocation.Y;

    if (g_sChar.m_bActive == true)
    {
        for (int i = 0; i < 2; i++)
        {
            g_Console.writeToBuffer(p_Model[i].m_cLocation, (char)18, Color);
        }
    }
}

void checkDestroyed()
{
    WORD Color = 0x1Ad;

    if (destroyed[0] == true)
    {
        g_Console.writeToBuffer(Destroyed[0].m_cLocation, "*", Color);
        destroyed[0] = false;
    }

    if (destroyed[1] == true)
    {
        g_Console.writeToBuffer(Destroyed[1].m_cLocation, "*", Color);
        destroyed[1] = false;
    }
    if (destroyed[2] == true)
    {
        g_Console.writeToBuffer(Destroyed[2].m_cLocation, "*", Color);
        destroyed[2] = false;
    }
    if (destroyed[3] == true)
    {
        for (int i = 0; i < difficulty; i++)
        {
            g_Console.writeToBuffer(Enemy[i].m_cLocation, "*", Color);
            g_Console.writeToBuffer(Enemy2[i].m_cLocation, "*", Color);
            g_Console.writeToBuffer(Enemy3[i].m_cLocation, "*", Color);
        }
        destroyed[3] = false;
    }
}

void Dodge()
{
    COORD text;
    text.X = g_Console.getConsoleSize().X - 29;
    text.Y = g_Console.getConsoleSize().Y - 2;

    g_Console.writeToBuffer(text, "Dodge Meter: " + to_string(DodgeCounter));

    if (DodgeCounter == 1)
    {
        if (g_skKeyEvent[K_UP].keyDown && pDodge == true)
        {
            if ((g_sChar.m_cLocation.Y - 1) > 1)
            {
                g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 2;
            }
            else
            {
                g_sChar.m_cLocation.Y = 1;
            }
            DodgeCounter = 0;
            pDodge = false;
            sndPlaySound(L"dodge.wav", SND_FILENAME | SND_ASYNC);
        }
        if (g_skKeyEvent[K_DOWN].keyDown && pDodge == true)
        {
            if ((g_sChar.m_cLocation.Y + 1) < 28)
            {
                g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y + 2;
            }
            else
            {
                g_sChar.m_cLocation.Y = 28;
            }
            DodgeCounter = 0;
            pDodge = false;
            sndPlaySound(L"dodge.wav", SND_FILENAME | SND_ASYNC);
        }
        if (g_skKeyEvent[K_RIGHT].keyDown && pDodge == true)
        {
            if ((g_sChar.m_cLocation.X + 1) < 38)
            {
                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 3;
            }
            else
            {
                g_sChar.m_cLocation.X = 38;
            }
            DodgeCounter = 0;
            pDodge = false;
            sndPlaySound(L"dodge.wav", SND_FILENAME | SND_ASYNC);
        }
        if (g_skKeyEvent[K_LEFT].keyDown && pDodge == true)
        {
            if ((g_sChar.m_cLocation.X - 1) > 1)
            {
                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 3;
            }
            else
            {
                g_sChar.m_cLocation.X = 1;
            }
            DodgeCounter = 0;
            pDodge = false;
            sndPlaySound(L"dodge.wav", SND_FILENAME | SND_ASYNC);
        }
    }
    else
    {
        if (DodgeCounter > 1)
        {
            DodgeCounter = 1;
        }
        else
        {
            DodgeCounter = DodgeCounter + 0.006;
        }

    }
}

void renderDifficulty(void)
{
    COORD c;
    c.X = 10;
    c.Y = g_Console.getConsoleSize().Y / 4;
    if ((g_dElapsedTime > 45.0) && (g_dElapsedTime < 55.0))
    {
        g_Console.writeToBuffer(c, "Difficulty increased", 0x0F);
    }
    if ((g_dElapsedTime > 90.0) && (g_dElapsedTime < 100.0))
    {
        g_Console.writeToBuffer(c, "Difficulty increased", 0x0F);
    }
    if ((g_dElapsedTime > 135.0) && (g_dElapsedTime < 145.0))
    {
        g_Console.writeToBuffer(c, "Difficulty increased", 0x0F);
    }
    if ((g_dElapsedTime > 180.0) && (g_dElapsedTime < 190.0))
    {
        c.X -= 1;
        g_Console.writeToBuffer(c, "Max difficulty reached", 0x0F);
    }
}

void movePowerUp()
{
    if (g_dElapsedTime >= timeToMove2)
    {
        timeToMove2 = g_dElapsedTime + 0.5;
        PowerUp.m_cLocation.Y++;

        if (PowerUp.m_bActive == true)
        {
            if (Special == 4)
            {
                Special = 1;
            }
            else
            {
                Special++;
            }
        }


    }

    if ((PowerUp.m_cLocation.Y == g_sChar.m_cLocation.Y && PowerUp.m_cLocation.X == g_sChar.m_cLocation.X) || PowerUp.m_cLocation.Y == 29 ||
        (PowerUp.m_cLocation.Y == g_sChar.m_cLocation.Y && PowerUp.m_cLocation.X == g_sChar.m_cLocation.X - 1) ||
        (PowerUp.m_cLocation.Y == g_sChar.m_cLocation.Y && PowerUp.m_cLocation.X == g_sChar.m_cLocation.X + 1))
    {
        if (PowerUp.m_cLocation.Y == 29)
        {
            SpecialText = "";
        }
        else
        {
            PowerEaten = true;
        }
        PowerUp.m_bActive = false;
    }
}

void renderSpecial()
{
    if (PowerUp.m_bActive == true)
    {
        if (Special == 1)
        {
            WORD Color = 0x0C;
            SpecialText = "Multishot";
            g_Console.writeToBuffer(PowerUp.m_cLocation, 'M',Color);
        }
        else if (Special == 2)
        {
            WORD Color = 0x02;
            SpecialText = "Health";
            g_Console.writeToBuffer(PowerUp.m_cLocation, 'H',Color);
        }
        else if (Special == 3)
        {
            WORD Color = 0x0E;
            SpecialText = "Bomb";
            g_Console.writeToBuffer(PowerUp.m_cLocation, 'B',Color);
        }
        else if (Special == 4)
        {
            WORD Color = 0x01;
            SpecialText = "Shield";
            g_Console.writeToBuffer(PowerUp.m_cLocation, 'S',Color);
        }

    }


}

void checkKilled()
{
    if ((ekilled % 10 == 0) && (ekilled > 0) && PowerUp.m_bActive == false)
    {
        number = rand() % (35 - 5) - 5;
        PowerUp.m_cLocation.X = (xGameSpace - number);
        for (int i = 0; i < difficulty; i++)
        {
            if ((PowerUp.m_cLocation.X == Enemy[i].m_cLocation.X) || (PowerUp.m_cLocation.X == Enemy2[i].m_cLocation.X))
            {
                number = rand() % (35 - 5) - 5;
                PowerUp.m_cLocation.X = (xGameSpace- number);
                while (PowerUp.m_cLocation.X > 39 || PowerUp.m_cLocation.Y < 1)
                {
                    PowerUp.m_cLocation.X = (xGameSpace - (rand() % (35 - 5) - 5));
                }
            }
        }

        PowerUp.m_cLocation.Y = g_Console.getConsoleSize().Y - 29;
        ekilled = 0;
        PowerUp.m_bActive = true;

    }
}

void consoleBG(void)
{
    COORD c;
    for (unsigned int rows = 0; rows < 60; ++rows)
    {
        for (unsigned int cols = 0; cols < 30; ++cols)
        {
            c.X = rows;
            c.Y = cols;
            g_Console.writeToBuffer(c, " ", 0x00);
        }
    }
}

void update2(double dt)
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: splashScreenWait2(); // game logic for the splash screen
        break;
    case S_GAME: updateGame(); // gameplay logic when we are in the game
        break;
    }
}

void splashScreenWait2(void)
{
    if (g_dElapsedTime > 0.1) // wait for 3 seconds to switch to game mode, else do nothing
        g_eGameState = S_GAME;
}

void resetTimer(void)
{
    g_dElapsedTime = 0.0;
    g_eGameState = S_SPLASHSCREEN;
}

void resetName(void)
{
    name = "";
}

void resetClass(void)
{
    int reset = 0;
    for (unsigned int x = 0; x < 6; ++x)
    {
        score1[x].setName("");
        score1[x].setScore(reset);
    }
}

void setInfo(void)
{

    for (unsigned int x = 0; x < 6; ++x) // 6 as there are 6 class, 5 being used for the leaderboard if there are any saved scores and last being the player's
    {
        if (score1[x].getScore() == 0)
        {
            score1[x].setName(name);
            score1[x].setScore(score); // set to score
            break;
        }
    }
}

void render2(void) // for rendering the menu
{
    clearScreen();
    consoleBG();
    renderMenu();
    renderToScreen();
}

void gameTitle(void)
{
    COORD c;
    c.X = 1;
    c.Y = 1;
    g_Console.writeToBuffer(c, "                           db", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "                          d88b", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "  d8888b   8888888b      d8888b       d8888b   8888888888", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "d88P  Y88b 888888888b   d888888b    d88888888b 8888888888", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "Y88b       888    88b  d88    88b  888P    Y8P 888", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "  Y88b     888    88P  888    888  88P         8888888888", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "     Y88b  888888888P  8888888888  88b         8888888888", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "       888 88888888P  d8888888888b 888b    d8b 888", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "Y88b  d88P 888       d8888    8888b Y8888888P  8888888888", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "  Y8888P   888         8888888888     Y8888P   8888888888", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "                         88  88", 0x0F);

    c.X += 1;
    c.Y += 2;
    g_Console.writeToBuffer(c, "                               8", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "     d88     db     88b    db d8b db    88888888b", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "    d888    d88b    888b  d888888888b   888888888b", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "    Y88p   d8888b   d88P d88 Y888P 88b  888    888b", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "     Y88bd88888888bd88P   Y8   Y   8P   888888888P", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "      Y8888P    Y8888P   d88888888888b  888888888P", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "       Y88P      Y88P    888  Y8P  888  888   Y88b", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "        qp        qp     888   8   888  888    Y88b", 0x0F);
}

int renderMenu(void)
{
    int choice;
    COORD c;

    start_gameTime = 0;
    PowerEaten = false;
    ekilled = 0;
    score = 0;
    life = 3;

    gameTitle();
    c.X = g_Console.getConsoleSize().X / 2 - 3; 
    c.Y = 23;
    g_Console.writeToBuffer(c, "START", 0x0F);
    
    c.Y += 2;
    g_Console.writeToBuffer(c, "SCORE", 0x0F);

    c.Y += 2;
    g_Console.writeToBuffer(c, "QUIT", 0x0F);
    
    c.X = 2;
    c.Y = 28;
    g_Console.writeToBuffer(c, "Group 11", 0x0F);
    if (g_skKeyEvent[K_ESCAPE].keyDown)
        return choice = 2;
    if ((g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) && ((g_mouseEvent.mousePosition.X >= g_Console.getConsoleSize().X / 2 - 3) && (g_mouseEvent.mousePosition.X <= g_Console.getConsoleSize().X / 2 + 4)) && ((g_mouseEvent.mousePosition.Y >= 22) && (g_mouseEvent.mousePosition.Y <= 23)))
        return choice = 1;
    else if ((g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) && ((g_mouseEvent.mousePosition.X >= g_Console.getConsoleSize().X / 2 - 3) && (g_mouseEvent.mousePosition.X <= g_Console.getConsoleSize().X / 2 + 4)) && ((g_mouseEvent.mousePosition.Y >= 24) && (g_mouseEvent.mousePosition.Y <= 25)))
        return choice = 2;
    else if ((g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) && ((g_mouseEvent.mousePosition.X >= g_Console.getConsoleSize().X / 2 - 3) && (g_mouseEvent.mousePosition.X <= g_Console.getConsoleSize().X / 2 + 4)) && ((g_mouseEvent.mousePosition.Y >= 26) && (g_mouseEvent.mousePosition.Y <= 27)))
        return choice = 3;
}

void render3(void)
{
    clearScreen();
    consoleBG();
    renderScore();
    renderToScreen();
}

bool renderScore(void)
{
    COORD c;
    std::ostringstream ss;

    c.X = g_Console.getConsoleSize().X / 2 - 7;
    c.Y = g_Console.getConsoleSize().Y / 4 - 2;

    g_Console.writeToBuffer(c, "LEADERBOARD");
    c.X -= 4;
    c.Y += 2;
    g_Console.writeToBuffer(c, "RANK     NAME  SCORE");
    c.X += 1;
    c.Y += 2;
    g_Console.writeToBuffer(c, "1");
    c.Y += 2;
    g_Console.writeToBuffer(c, "2");
    c.Y += 2;
    g_Console.writeToBuffer(c, "3");
    c.Y += 2;
    g_Console.writeToBuffer(c, "4");
    c.Y += 2;
    g_Console.writeToBuffer(c, "5");

    c.X -= 4;
    c.Y += 6;
    g_Console.writeToBuffer(c, "Press Esc to return to menu");

    std::string arr2[6];
    int arr[] = { score1[0].getScore(), score1[1].getScore(), score1[2].getScore(),
                  score1[3].getScore(), score1[4].getScore(), score1[5].getScore() };
    int n = sizeof(arr) / sizeof(arr[0]);

    std::sort(arr, arr + n, std::greater<int>());
    for (unsigned int x = 0; x < 6; ++x)
    {
        for (unsigned int y = 0; y < 6; ++y)
        {
            if (arr[x] == score1[y].getScore())
            {
                arr2[x] = score1[y].getName(); // have this rewrite into the new file
            }
        }
    }

    resetClass();
    c.Y = g_Console.getConsoleSize().Y / 4;
    for (int i = 0; i < 5; ++i)
    {
        c.X = g_Console.getConsoleSize().X / 2 - 2;

        if (arr2[i] == "")
        {
            c.X += 5;
            c.Y += 2;
            score1[i].setName(arr2[i]);
            score1[i].setScore(arr[i]);
            ss.str("");
            ss << score1[i].getName() << "   " << score1[i].getScore();
            g_Console.writeToBuffer(c, ss.str());
        }
        else
        {
            c.Y += 2;
            score1[i].setName(arr2[i]);
            score1[i].setScore(arr[i]);
            ss.str("");
            ss << score1[i].getName() << "   " << score1[i].getScore();
            g_Console.writeToBuffer(c, ss.str());
        }
        name2[i] = arr2[i];
        score2[i] = arr[i];
    }
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
    {
        return false;
    }

}

void render4(void)
{
    clearScreen();
    consoleBG();
    renderName();
    renderToScreen();
}

int renderName(void)
{
    bool conti = false;
    COORD a, b, c;

    a.X = g_Console.getConsoleSize().X / 2 - 16; a.Y = 10;
    b.X = g_Console.getConsoleSize().X / 2 - 14; b.Y = 16;
    c.X = g_Console.getConsoleSize().X / 2 - 12; c.Y = 18;
    g_Console.writeToBuffer(a, "Enter Player  Name (5 letters):", 0x0F);
    g_Console.writeToBuffer(b, "Press 'Enter'  to continue", 0x0F);
    g_Console.writeToBuffer(c, "Press 'Esc' to go back", 0x0F);

    if (g_skKeyEvent[K_ESCAPE].keyReleased)
    {
        resetName();
        return 0;
    }
    else if ((g_skKeyEvent[K_ENTER].keyReleased) && (name.length() == 5))
    {
        return 1;
    }
    else
    {
        COORD c;
        c.X = g_Console.getConsoleSize().X / 2 - 3;
        c.Y = 13;

        if (g_skKeyEvent[K_BACK].keyDown)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                std::string newn = name.substr(0, name.length() - 1);
                name = newn;
                eventCount = 0;
            }
        }
        else if (name.length() == 5)
        {
            name = name;
        }
        else if (g_skKeyEvent[K_A].keyReleased)
        {

            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "A";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_B].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "B";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_C].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "C";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_D].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "D";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_E].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "E";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_F].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "F";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_G].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "G";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_H].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "H";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_I].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "I";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_J].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "J";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_K].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "K";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_L].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "L";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_M].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "M";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_N].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "N";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_O].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "O";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_P].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "P";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_Q].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "Q";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_R].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "R";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_S].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "S";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_T].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "T";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_U].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "U";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_V].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "V";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_W].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "W";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_X].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "X";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_Y].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "Y";
                eventCount = 0;
            }
        }
        else if (g_skKeyEvent[K_Z].keyReleased)
        {
            if (eventCount != 2)
            {
                ++eventCount;
            }
            else
            {
                name += "Z";
                eventCount = 0;
            }
        }
        g_Console.writeToBuffer(c, name, 0x0F);
    }
}

void renderUI(void)
{
    COORD c;

    // this part renders the player's name
    {
        c.X = 45;
        c.Y = 2;
        g_Console.writeToBuffer(c, "Player Name");

        c.X += 3;
        c.Y += 1;
        g_Console.writeToBuffer(c, name);

    }

    // this part outputs the lives
    {
        c.X = 47;
        c.Y = 7;
        g_Console.writeToBuffer(c, "Lives");
        c.X += 2;
        c.Y += 1;
        g_Console.writeToBuffer(c, to_string(life));
    }

    // this part outputs the score
    {
        c.X = 47;
        c.Y = 12;
        g_Console.writeToBuffer(c, "Score");
        c.X += 2;
        c.Y += 1;
        g_Console.writeToBuffer(c, to_string(score));
    }

    // this part outputs the time
    {
        c.X = 47;
        c.Y = 17;
        g_Console.writeToBuffer(c, "Dodge");

        c.X += 2;
        c.Y += 1;
        if (DodgeCounter != 1)
        {
            c.X -= 5;
            g_Console.writeToBuffer(c, "Cooling Down");
        }
        else
        {
            c.X -= 2;
            g_Console.writeToBuffer(c, "Ready");
        }
    }

    // this part outputs the powerup being used
    {
        c.X = 45;
        c.Y = 22;
        g_Console.writeToBuffer(c, "PowerUps");
        c.Y += 2;
        c.X += 1;
        if ((Special == 1) && (PowerEaten == true))
        {
            c.X -= 1;
            g_Console.writeToBuffer(c, "Multi-Shot", 0x0F);
        }
        else if ((Special == 4) && (PowerEaten == true))
        {
            g_Console.writeToBuffer(c, "Shield", 0x0F);
        }
        else
        {
            g_Console.writeToBuffer(c, "", 0x0F);
        }
    }

    // this part outputs the UI borders
    for (unsigned int x = 0; x < 3; ++x)
    {
        for (unsigned int y = 0; y < 30; ++y)
        {
            if (x == 2)
                {c.X = 59; c.Y = y;}
            else
                {c.X = x * 39; c.Y = y;}
            g_Console.writeToBuffer(c, " ", 0xF0);
        }
    }
    for (unsigned int x = 0; x < 2; ++x)
    {
        for (unsigned int y = 0; y < 60; ++y)
        {
            c.X = y;
            c.Y = x * 29;
            g_Console.writeToBuffer(c, " ", 0xF0);
        }
    }
    for (unsigned int x = 1; x < 5; ++x)
    {
        for (unsigned int y = 40; y < 60; ++y)
        {
            c.X = y;
            c.Y = 5 * x;
            g_Console.writeToBuffer(c, " ", 0xF0);
        }
    }
    for (unsigned int x = 1; x < 5; ++x)
    {
        for (unsigned int y = 40; y < 60; ++y)
        {
            c.X = y;
            c.Y = 25 + x;
            g_Console.writeToBuffer(c, " ", 0xF0);
        }
    }
    {
        c.X = 44;
        c.Y = 27;
        g_Console.writeToBuffer(c, "Press  'Esc'", 0xF0);
        c.X += 2;
        c.Y += 1;
        g_Console.writeToBuffer(c, "to quit", 0xF0);
    }
}

void render5(void)
{
    clearScreen();
    consoleBG();
    renderResult();
    renderToScreen();
}

bool renderResult(void)
{
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
    {
        result = 0;
        return true;
    }

    destroyEnemy();

    COORD c;
    std::ostringstream ss;
    c.X = g_Console.getConsoleSize().X / 2;
    c.Y = g_Console.getConsoleSize().Y / 3;

    if (result == 1)
    {
        c.X -= 4;
        g_Console.writeToBuffer(c, "Game Over");

        c.X -= 9;
        c.Y += 10;
        g_Console.writeToBuffer(c, "Press 'Esc' to return to menu");

        c.X = g_Console.getConsoleSize().X / 2 - 5;
        c.Y = g_Console.getConsoleSize().Y / 3 + 4;

        ss << "Name: " << name;
        g_Console.writeToBuffer(c, ss.str());

        c.Y += 2;
        ss.str("");
        ss << "Score: " << score;
        g_Console.writeToBuffer(c, ss.str());
    }
}

void initScore(void)
{
    std::ofstream outScore("score.txt");
    for (unsigned int x = 0; x < 5; ++x)
    {
        outScore << score2[x] << std::endl;
    }
    outScore.close();

    std::ofstream outName("name.txt");
    for (unsigned int y = 0; y < 5; ++y)
    {
        outName << name2[y] << std::endl;
    }
    outName.close();
}

void outScore(void)
{
    std::ifstream inScore, inName;
    inScore.open("score.txt");

    if (inScore.fail())
    {
        std::cerr << "Error, could not open file" << std::endl;
        exit(1);
    }

    int num[6];

    inScore >> num[0] >> num[1] >> num[2] >> num[3] >> num[4];

    for (unsigned int x = 0; x < 5; ++x)
    {
        if (num[x] < 0)
        {
            int nu = 0;
            score1[x].setScore(nu);
        }
        else
        {
            int nu = num[x];
            score1[x].setScore(nu);
        }
    }
    inScore.close();

    inName.open("name.txt");

    if (inName.fail())
    {
        std::cerr << "Error, could not open file" << std::endl;
        exit(1);
    }

    std::string nam[5];
    inName >> nam[0] >> nam[1] >> nam[2] >> nam[3] >> nam[4];

    for (unsigned int y = 0; y < 5; ++y)
    {
        std::string na = nam[y];
        score1[y].setName(na);
    }
    inName.close();

    // this resets the txt file
    std::ofstream outScore("score.txt");
    for (unsigned int x = 0; x < 5; ++x)
    {
        outScore << 0 << std::endl;
    }
    outScore.close();

    std::ofstream outName("name.txt");
    for (unsigned int y = 0; y < 5; ++y)
    {
        outName << "" << std::endl;
    }
    outName.close();
}

#pragma endregion



