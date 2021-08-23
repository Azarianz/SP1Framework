#pragma once
#include <string>
class scores
{
private:
	std::string name;
	int score;
public:
	scores(void);
	void setScore(int x);
	int getScore(void);
	std::string getName(void);
	void setName(std::string x);
};

