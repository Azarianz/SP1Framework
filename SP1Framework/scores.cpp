#include "scores.h"
#include <string>

// most likely, the score and name will only be taken if the player either dies, or wins
// not if they leave mid-game.

scores::scores()
{
	name = "";
}

void scores::getScore(void)
{
	score = 1;
	// the enemies killed will be put in here
}

int scores::setScore(void)
{
	return score;
}

std::string scores::getName(void)
{
	return name;
}

void scores::setName(std::string x)
{
	name = x;
}
