#pragma once
#include <string>
class scores
{
private:
	std::string name;
	int score;
public:
	scores(void);
	void getScore(void);
	int setScore(void);
	std::string setName(std::string x);
};

