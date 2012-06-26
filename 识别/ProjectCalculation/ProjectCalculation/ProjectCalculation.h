#ifndef PROJECT_CALCULATION_H
#define PROJECT_CALCULATION_H

#include "tinyxml.h"
#include "tinystr.h"
#include <vector>
using namespace std;

struct point
{
	int x,y;
}

struct segment
{
	point p1;
	point p2;
}

class ProjectCalculation
{
//private:
public:
	vector<segment> segments;
	bool readXML(const char * filename);
	point * findContour(vector<segment> sgm);

public:
	ProjectCalculation();
	~ProjectCalculation();

	void calculate();
}

#endif