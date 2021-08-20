#include "scores.h"
#include <string>
#include <stdlib.h>
#include <time.h>


// most likely, the score and name will only be taken if the player either dies, or wins
// not if they leave mid-game.

scores::scores()
{
	name = "";
	score = 0;
}

void scores::setScore(void)
{
	srand((int)time(0));
	int ran = rand() % 100;
	score += ran;
	// the enemies killed will be put in here
}

int scores::getScore(void)
{
	return score;
}

std::string scores::getName(void)
{
	return name;
}

void scores::setName(std::string x)
{
	name += x;
}
