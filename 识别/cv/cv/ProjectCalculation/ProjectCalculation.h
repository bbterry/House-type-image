#ifndef PROJECT_CALCULATION_H
#define PROJECT_CALCULATION_H

#include<iostream>
#include<string.h>
#include "tinyxml.h"
#include "tinystr.h"
#include <vector>
#include<math.h>
#include<fstream>
using namespace std;

struct point
{
	int x,y;
};

int compare(point p1, point p2);  //�Ƚ������С

struct segment
{
	point p1;
	point p2;
	double height;
	double length;
};

double sgmLength(segment sgm);  //segment����

class ProjectCalculation
{
private:
    vector<segment> segments;
	vector<point> points;

public:
	bool readXML(const char * filename);      //��xml�ļ���������
	vector<point> findContour(vector<segment> sgm, vector<segment> &outSgm);     //��������
	int cross(const point &A, const point &B, const point &C);   // ��Ĳ��: AB * AC
	double polygon_area(const int &n, vector<point> p);          //�������
public:
	ProjectCalculation();
	~ProjectCalculation();

	void calculate(const char * filename);
};

#endif