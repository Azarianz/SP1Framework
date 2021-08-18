// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "scores.h"

double  g_dElapsedTime;
double  g_dDeltaTime;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;

// Game specific variables here
SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state

// Console object
Console g_Console(40, 30, "SP1 Framework");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
    // Set precision for floating point output
    g_dElapsedTime = 0.0;    

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 2;
    g_sChar.m_bActive = true;
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

void render2(void) // for rendering the menu
{
    clearScreen();
    renderMenu();
    //renderInputEvents();
    renderToScreen();
}

int renderMenu(void)
{
    int choice;
    COORD b, s, q, x, y, z;
    std::ostringstream bb, ss, qq, xx, yy, zz;

    b.X = 18; b.Y = 10;
    s.X = 18; s.Y = 15;
    q.X = 18; q.Y = 20;

    bb << "START";
    ss << "SCORE";
    qq << "QUIT";

    g_Console.writeToBuffer(b, bb.str(), 0xF0);
    g_Console.writeToBuffer(s, ss.str(), 0xF0);
    g_Console.writeToBuffer(q, qq.str(), 0xF0);

    if ((g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) && ((g_mouseEvent.mousePosition.X >= 18) && (g_mouseEvent.mousePosition.X <= 23)) && ((g_mouseEvent.mousePosition.Y >= 10) && (g_mouseEvent.mousePosition.Y <= 13)))
        return choice = 1;
    else if ((g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) && ((g_mouseEvent.mousePosition.X >= 18) && (g_mouseEvent.mousePosition.X <= 23)) && ((g_mouseEvent.mousePosition.Y >= 15) && (g_mouseEvent.mousePosition.Y <= 18)))
        return choice = 2;
    else if ((g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) && ((g_mouseEvent.mousePosition.X >= 18) && (g_mouseEvent.mousePosition.X <= 23)) && ((g_mouseEvent.mousePosition.Y >= 20) && (g_mouseEvent.mousePosition.Y <= 23)))
        return choice = 3;
}

void render3(void)
{
    clearScreen();
    //renderScore();
    renderToScreen();
}

void renderScore(void)
{
}

void render4(void)
{
    clearScreen();
    renderName();
    //renderInputEvents();
    renderToScreen();
}
std::string name = "";
bool renderName(void)
{
    bool conti = false;
    COORD a,b;
    a.X = 10; a.Y = 10;
    b.X = 7; b.Y = 16;

    g_Console.writeToBuffer(a, "Enter Player Name:", 0x0F);
    g_Console.writeToBuffer(b, "Press 'Enter' to continue", 0x0F);

    if (g_skKeyEvent[K_ENTER].keyReleased)    
    {

        return true;
    }
    else
    {
        COORD c;
        c.X = 13;
        c.Y = 13;

        if (g_skKeyEvent[K_BACK].keyDown)
        {
            std::string newn = name.substr(0, name.length() - 1);
            name = newn;
        }
        else if (name.length() == 12)
        {
            name = name;
        }
        else if (g_skKeyEvent[K_A].keyReleased)
        {
            name += "A";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_B].keyReleased)
        {
            name += "B";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_C].keyReleased)
        {
            name += "C";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_D].keyReleased)
        {
            name += "D";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_E].keyReleased)
        {
            name += "E";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_F].keyReleased)
        {
            name += "F";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_G].keyReleased)
        {
            name += "G";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_H].keyReleased)
        {
            name += "H";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_I].keyReleased)
        {
            name += "I";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_J].keyReleased)
        {
            name += "J";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_K].keyReleased)
        {
            name += "K";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_L].keyReleased)
        {
            name += "L";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_M].keyReleased)
        {
            name += "M";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_N].keyReleased)
        {
            name += "N";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_O].keyReleased)
        {
            name += "O";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_P].keyReleased)
        {
            name += "P";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_Q].keyReleased)
        {
            name += "Q";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_R].keyReleased)
        {
            name += "R";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_S].keyReleased)
        {
            name += "S";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_T].keyReleased)
        {
            name += "T";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_U].keyReleased)
        {
            name += "U";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_V].keyReleased)
        {
            name += "V";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_W].keyReleased)
        {
            name += "W";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_X].keyReleased)
        {
            name += "X";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_Y].keyReleased)
        {
            name += "Y";
            //scores.setName(add);
        }
        else if (g_skKeyEvent[K_Z].keyReleased)
        {
            name += "Z";
            //scores.setName(add);
        }
        std::ostringstream cc;
        cc << name;
        g_Console.writeToBuffer(c, cc.str(), 0x0F);
    }
}

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
    case VK_RETURN : key = K_ENTER; break;
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
}

void updateGame()       // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
}

void moveCharacter()
{    
    // Updating the location of the character based on the key release
    // providing a beep sound whenver we shift the character
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.Y > 1) // originally 0 and -1 but with additional 1, player will not be able to go into the borders
                                                                 // change this according to the UI to prevent player from going into the it.
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.Y--;       
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X > 1) // player move when key is down and stops if it releases, originally was move when released (single movements)
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
        g_sChar.m_cLocation.X++;        
    }
    if (g_skKeyEvent[K_SPACE].keyDown)
    {
        g_sChar.m_bActive = !g_sChar.m_bActive;        
    }

   
}
void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
        g_bQuitGame = true;    
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
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: renderSplashScreen();
        break;
    case S_GAME: renderGame();
        break;
    }
    renderFramerate();      // renders debug information, frame rate, elapsed time, etc
    renderInputEvents();    // renders status of input events
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
    g_Console.writeToBuffer(c, "The game will start in 3 seconds", 0x03);
    c.Y += 2;
    c.X = g_Console.getConsoleSize().X / 2 - 12;
    g_Console.writeToBuffer(c, "Use the Arrow keys to move", 0x09);
    c.Y += 2;
    c.X = g_Console.getConsoleSize().X / 2 - 9;
    g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x09);
}

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
}

void renderMap()
{
    // Set up sample colours, and output shadings
    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

    COORD c;
    for (int row = 0; row < 40; ++row)
    {
        for (int col = 0; col < 30; ++col)
        {
            if (col == 0 || col == 29)
            {
                c.X = row;
                c.Y = col;
                colour(colors[1]);
            }
            else if (row == 0 || row == 39)
            {
                c.X = row;
                c.Y = col;
                colour(colors[1]);
            }

            g_Console.writeToBuffer(c, " ", colors[3]);
        }
    }
}

void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x0C;
    if (g_sChar.m_bActive)
    {
        charColor = 0x0A;
    }
    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)3, charColor);
}



void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9; // can prob be taken out
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x0F);

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs"; // the position can be changed according to the UI
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x0F);
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
        case K_UP: key = "UP"; break;
        case K_DOWN: key = "DOWN"; break;
        case K_LEFT: key = "LEFT"; break;
        case K_RIGHT: key = "RIGHT"; break;
        case K_SPACE: key = "SPACE"; break;
        case K_ESCAPE: key = "ESCAPE"; break;
        case K_ENTER: key = "ENTER"; break;
        case K_BACK: key = "BACKSPACE"; break;
        case K_A: key = "A"; break;
        /*case K_B: key = "B"; break;
        case K_C: key = "C"; break;
        case K_D: key = "D"; break;
        case K_E: key = "E"; break;
        case K_F: key = "F"; break;
        case K_G: key = "G"; break;
        case K_H: key = "H"; break;
        case K_I: key = "I"; break;
        case K_J: key = "J"; break;
        case K_K: key = "K"; break;
        case K_L: key = "L"; break;
        case K_M: key = "M"; break;
        case K_N: key = "N"; break;
        case K_O: key = "O"; break;
        case K_P: key = "P"; break;
        case K_Q: key = "Q"; break;
        case K_R: key = "R"; break;
        case K_S: key = "S"; break;
        case K_T: key = "T"; break;
        case K_U: key = "U"; break;
        case K_V: key = "V"; break;
        case K_W: key = "X"; break;
        case K_X: key = "X"; break;
        case K_Y: key = "Y"; break;
        case K_Z: key = "Z"; break;*/
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



