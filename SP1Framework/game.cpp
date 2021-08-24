// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "scores.h"
#include <algorithm>
#include <fstream>

double  g_dElapsedTime;
double  g_dDeltaTime;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;

// Game specific variables here
SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state

// Console object
Console g_Console(60, 30, "SP1 Framework");

// Variables used for UI/UX
std::string name = ""; // takes in user input name
scores score[6]; // no of scores that can be kept
int score2[5]; // used to write the score rankings into file
std::string name2[5]; // used to write the name rankings into file
int eventCount = 0; // mitigates the problem of keypresses being pressed twice
int result = 0; // used to determine whether player win or lose

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

    g_sChar.m_cLocation.X = 20;
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
        score[x].setName("");
        score[x].setScore(reset);
    }
}

void setInfo(void)
{
    
    for (unsigned int x = 0; x < 6; ++x) // 6 as there are 6 class, 5 being used for the leaderboard if there are any saved scores and last being the player's
    {
        if (score[x].getScore() == 0)
        {
            score[x].setName(name);
            
            srand((int)time(0));
            int ran = rand() % 100 + 1;
            score[x].setScore(ran);
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
    COORD b, s, q, x, y, z;
    std::ostringstream bb, ss, qq, xx, yy, zz;

    gameTitle();

    b.X = g_Console.getConsoleSize().X / 2 - 3; b.Y = 23;
    s.X = g_Console.getConsoleSize().X / 2 - 3; s.Y = 25;
    q.X = g_Console.getConsoleSize().X / 2 - 3; q.Y = 27;

    bb << "START";
    ss << "SCORES";
    qq << "QUIT";

    g_Console.writeToBuffer(b, bb.str(), 0x0F);
    g_Console.writeToBuffer(s, ss.str(), 0x0F);
    g_Console.writeToBuffer(q, qq.str(), 0x0F);

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
    int arr[] = { score[0].getScore(), score[1].getScore(), score[2].getScore(), 
                  score[3].getScore(), score[4].getScore(), score[5].getScore() };
    int n = sizeof(arr) / sizeof(arr[0]);
    
    std::sort(arr, arr + n, std::greater<int>());
    for (unsigned int x = 0; x < 6; ++x)
        {
            for (unsigned int y = 0; y < 6; ++y)
            {
                if (arr[x] == score[y].getScore())
                {
                    arr2[x] = score[y].getName(); // have this rewrite into the new file
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
            score[i].setName(arr2[i]);
            score[i].setScore(arr[i]);
            ss.str("");
            ss << score[i].getName() << "   " << score[i].getScore();
        g_Console.writeToBuffer(c, ss.str());
        }
        else 
        {
            c.Y += 2;
            score[i].setName(arr2[i]);
            score[i].setScore(arr[i]);
            ss.str("");
            ss << score[i].getName() << "   " << score[i].getScore();
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
    COORD a,b,c;

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
    std::ostringstream ss;
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
        ss.str("");
        c.X = 47;
        c.Y = 7;
        g_Console.writeToBuffer(c, "Lives");
    }

    // this part outputs the score
    {
        ss.str("");
        c.X = 47;
        c.Y = 12;
        g_Console.writeToBuffer(c, "Score");
    }

    // this part outputs the time
    {
        ss.str("");
        c.X = 48;
        c.Y = 17;
        g_Console.writeToBuffer(c, "Time");

        c.X += 1;
        c.Y += 1;
        ss << std::fixed << std::setprecision(0) << g_dElapsedTime;
        g_Console.writeToBuffer(c, ss.str());

    }

    // this part outputs the powerup being used
    {
        ss.str("");
        c.X = 46;
        c.Y = 22;
        g_Console.writeToBuffer(c, "PowerUps");
    }

    // this part outputs the UI borders
    for (unsigned int x = 1; x < 29; ++x)
    {
        c.X = 40;
        c.Y = x;
        g_Console.writeToBuffer(c, " ", 0xF0);
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
        ss << "Score: " /*<< score*/;
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
            score[x].setScore(nu);
        }
        else
        {
            int nu = num[x];
            score[x].setScore(nu);
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
        score[y].setName(na);
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
    //processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
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
    consoleBG();
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: renderSplashScreen();
        break;
    case S_GAME: renderGame();
        break;
    }
    // renderFramerate();      // renders debug information, frame rate, elapsed time, etc
    // renderInputEvents();    // renders status of input events
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
}

bool renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    renderUI();
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
    {
        result = 1;
        return true;
    }
    //else if (/*add condition of player live == 0*/)
    //{
    //    result = 1;
    //    true;
    //}
}

void renderMap()
{
    // Set up sample colours, and output shadings
    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0xF0, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

    COORD c;
    for (int row = 0; row < 60; ++row)
    {
        for (int col = 0; col < 30; ++col)
        {
            if (col == 0 || col == 29)
            {
                c.X = row;
                c.Y = col;
                colour(colors[1]);
            }
            else if (row == 0 || row == 59)
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