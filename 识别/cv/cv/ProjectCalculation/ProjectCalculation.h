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

int compare(point p1, point p2);  //比较两点大小

struct segment
{
	point p1;
	point p2;
	double height;
	double length;
};

double sgmLength(segment sgm);  //segment长度

class ProjectCalculation
{
private:
    vector<segment> segments;
	vector<point> points;

public:
	bool readXML(const char * filename);      //从xml文件读入数据
	vector<point> findContour(vector<segment> sgm, vector<segment> &outSgm);     //找外轮廓
	int cross(const point &A, const point &B, const point &C);   // 点的叉乘: AB * AC
	double polygon_area(const int &n, vector<point> p);          //面积计算
public:
	ProjectCalculation();
	~ProjectCalculation();

	void calculate(const char * filename);
};

#endif