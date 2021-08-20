#pragma once
#include <string>
class scores
{
private:
	std::string name;
	int score;
public:
	scores(void);
	void setScore(void);
	int getScore(void);
	std::string getName(void);
	void setName(std::string x);
};

