// This is the main file to hold everthing together

#include "Framework\timer.h"
#include "game.h"

CStopWatch g_Timer;                            // Timer function to keep track of time and the frame rate
bool g_bQuitGame = false;                    // Set to true if you want to quit the game
const unsigned char gc_ucFPS = 30;                // FPS of this game
const unsigned int gc_uFrameTime = 1000 / gc_ucFPS;    // time for each frame

//main loop declaration
void mainLoop( void );

// TODO:
// Bug in waitUntil. it waits for the time from getElapsedTime to waitUntil, but should be insignificant.

// main function - starting function
// You should not be modifying this unless you know what you are doing
int main( void )
{
    init();      // initialize your variables
    mainLoop();  // main loop
    shutdown();  // do clean up, if any. free memory.
    
    return 0;
}

//--------------------------------------------------------------
// Purpose  : This main loop calls functions to get input, 
//            update and render the game at a specific frame rate
//            You should not be modifying this unless you know what you are doing.
// Input    : void
// Output   : void
//--------------------------------------------------------------
void mainLoop( void )
{
    outScore();
    g_Timer.startTimer();    // Start timer to calculate how long it takes to render this frame
    while (!g_bQuitGame)      // run this loop until user wants to quit 
    {
        getInput();
        render2();
        update2(g_Timer.getElapsedTime());
        g_Timer.waitUntil(gc_uFrameTime);
        if (renderMenu() == 1)
        {
            resetTimer();
            g_Timer.startTimer();
            while (true)
            {
                getInput();
                render4();
                update2(g_Timer.getElapsedTime());
                g_Timer.waitUntil(gc_uFrameTime);
                if ((renderName() == 1) || (renderName() == 0))
                    break;
            }
            resetTimer();
            //resetGame();
            init();
            g_Timer.startTimer();
            while (true)
            {
                if (renderName() == 0)
                    break;
                else
                    getInput();
                    
                    render();
                    update(g_Timer.getElapsedTime());
                    g_Timer.waitUntil(gc_uFrameTime);

                if (renderGame() == true)
                {
                    resetTimer();
                    g_Timer.startTimer();
                    setInfo();
                    while (true)
                    {
                        getInput();
                        render5();
                        update2(g_Timer.getElapsedTime());
                        g_Timer.waitUntil(gc_uFrameTime);
                        if (renderResult() == true)
                        {
                            break;
                        }
                    }
                    resetName();
                    break;
                }
                else if (renderGame() == false)
                {
                    break;
                }
            }
        }
        else if (renderMenu() == 2)
        {
            resetTimer();
            g_Timer.startTimer();
            while (true)
            {
                getInput();
                render3();
                update2(g_Timer.getElapsedTime());
                g_Timer.waitUntil(gc_uFrameTime);
                if (renderScore() == false)
                    break;
            }
        }
        else if (renderMenu() == 3)
        {
            initScore();
            g_bQuitGame = true;
        }
    }
}
