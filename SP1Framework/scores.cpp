#include "scores.h"
#include <string>

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
