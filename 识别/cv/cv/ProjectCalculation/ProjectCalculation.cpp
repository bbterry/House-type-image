#include "ProjectCalculation.h"

//比较两点大小
int compare(point p1, point p2)
{
	if(p1.x == p2.x && p1.y == p2.y) return 0;
	else
	{
		if(p1.x > p2.x) return 1;
		else if( p1.x == p2.x && p1.y > p2.y) return 1;
		else return -1;
	}
}

double sgmLength(segment sgm)
{
	return sqrt(pow(float(sgm.p1.x-sgm.p2.x),2)+pow(float(sgm.p1.y-sgm.p2.y),2));
}

ProjectCalculation::ProjectCalculation()
{

}

ProjectCalculation::~ProjectCalculation()
{

}

bool ProjectCalculation::readXML(const char * filename)
{
	

	//创建一个XML的文档对象
	TiXmlDocument *myDocument = NULL;
	myDocument = new TiXmlDocument();

	//if(myDocument == NULL) return false;
	if(!myDocument->LoadFile(filename)) return false;
	//获得根元素
	TiXmlElement *RootElement = myDocument->RootElement();
	//输出根元素名称
	//cout << RootElement->Value() << endl;
	segment s;
	TiXmlElement *Line = RootElement->FirstChildElement();
	while(Line){
		TiXmlElement *point=Line->FirstChildElement();
		//
		TiXmlElement *x = point->FirstChildElement();
		TiXmlElement *y = x->NextSiblingElement();
		//输出
		//cout << x->GetText()<< endl;
		//cout << y->GetText() << endl;
		//cout<<"//"<<endl;

		s.p1.x=atoi(x->GetText());
		s.p1.y=atoi(y->GetText());
		point = point->NextSiblingElement();
	
		x = point->FirstChildElement();
		y = x->NextSiblingElement();
		//cout << x->GetText()<< endl;
		//cout << y->GetText() << endl;
		//cout<<"//"<<endl;

		s.p2.x=atoi(x->GetText());
		s.p2.y=atoi(y->GetText());
		segments.push_back(s);
		Line = Line->NextSiblingElement();
	}
	return true;
}

vector<point> ProjectCalculation::findContour(vector<segment> sgm, vector<segment> &outSgm)
{
	int minOffset = 8;  //偏差值

	vector<point> points;
	vector<segment> segments;

///////////////////////////////////寻找初始外墙
	segment minSgm;
	point minPnt;
	int index;
	for(int i=0; i<sgm.size(); ++i)
	{
		if(i==0) 
		{
			minSgm = sgm[0];
			minPnt = minSgm.p1;
			index = 0;
		}
		else
		{
			if(abs(sgm[i].p1.y-sgm[i].p2.y)<3)
				if(compare(sgm[i].p1,minPnt)==-1)
				{
					minPnt = sgm[i].p1;
					minSgm = sgm[i];
					index = i;
				}
		}
	}
///////////////////////////////////

	points.push_back(minPnt);

	segment currSgm = minSgm;
	int curr = index;
	int  startPoint = 1;
	
    //寻找外轮廓
	while(1)
	{
		int local =-1;      //交点位置记录
		point endPnt;       //匹配segment终点
		int matchType = 0;  //匹配类型
		int nextSP = 0;     //初始点记录
		int k = -1;  //下标标志

		//交点
		point crsPnt;
		point bestPnt; //最佳匹配交点记录
		bestPnt.x = -1;

		for(int i=0; i<sgm.size(); ++i)
		{
			if(i==curr) continue;

			if(abs(currSgm.p1.y - currSgm.p2.y)<3)     //当前segment是横线
			{
				//无交点
				if(abs(sgm[i].p1.y - sgm[i].p2.y)<3)   //待匹配segment是横线
				{ 
					if(abs(currSgm.p1.y-sgm[i].p1.y) > minOffset || currSgm.p1.x-sgm[i].p2.x > minOffset 
						|| sgm[i].p1.x-currSgm.p2.x > minOffset)
						//|| sgm[i].p2.y-currSgm.p1.y > minOffset || currSgm.p1.y-sgm[i].p1.y > minOffset)
						continue;                     
				}
				else   //竖线
				{
					if(currSgm.p1.y<sgm[i].p1.y-minOffset || currSgm.p1.y>sgm[i].p2.y+minOffset 
						|| sgm[i].p1.x<currSgm.p1.x-minOffset || sgm[i].p2.x>currSgm.p2.x+minOffset)
						continue;
				}

				//交点为初始点
				if(startPoint == 1 && (currSgm.p1.x > sgm[i].p1.x-minOffset && currSgm.p1.x < sgm[i].p2.x+minOffset
					&& currSgm.p1.y > sgm[i].p1.y-minOffset && currSgm.p1.y < sgm[i].p2.y+minOffset) )
					continue;
				if(startPoint == 2 && (currSgm.p2.x > sgm[i].p1.x-minOffset && currSgm.p2.x < sgm[i].p2.x+minOffset
					&& currSgm.p2.y > sgm[i].p1.y-minOffset && currSgm.p2.y < sgm[i].p2.y+minOffset) )
					continue;

				//交点
				crsPnt.x = sgm[i].p1.x;
				crsPnt.y = currSgm.p1.y;
				if(startPoint == 2 && abs(sgm[i].p1.y-sgm[i].p2.y)<3)
					crsPnt.x = sgm[i].p2.x;


				if(startPoint == 1)  //初始点为p1
				{  
					//第一种匹配
					if(matchType == 0 && abs(sgm[i].p1.x - sgm[i].p2.x)<3 && abs(sgm[i].p1.x-currSgm.p2.x) <= minOffset
						&& abs(sgm[i].p1.y-currSgm.p2.y) <= minOffset)
					{
						bestPnt = crsPnt;
						local =  crsPnt.x;
						endPnt = sgm[i].p2;
						nextSP = 1;
						matchType = 1;
						k = i;
					}

					//第二种匹配
					if(matchType < 2 && abs(sgm[i].p1.y-sgm[i].p2.y)<3 && abs(sgm[i].p1.x-currSgm.p2.x) <= minOffset
						&& abs(sgm[i].p1.y-currSgm.p2.y) <= minOffset)
					{
						local = crsPnt.x;
						endPnt = sgm[i].p2;
						nextSP = 1;
						matchType = 2;
						k = i;
					}

					//第三种匹配
					if(crsPnt.y-sgm[i].p1.y > minOffset)
						if(matchType < 3)
						{
							bestPnt = crsPnt;
							local = crsPnt.x;
							endPnt = sgm[i].p1;
							nextSP = 2;
							matchType = 3;
							k = i;
						}
						else
						{
							if(crsPnt.x < local)
							{
								bestPnt = crsPnt;
								local = crsPnt.x;
							    endPnt = sgm[i].p1;
								nextSP = 2;
								k = i;
							}
						}
				}
				else   //初始点为p2
				{
					if(matchType == 0 && abs(sgm[i].p1.x - sgm[i].p2.x)<3 && abs(sgm[i].p2.x-currSgm.p1.x) <= minOffset
						&& abs(sgm[i].p2.y-currSgm.p1.y) <= minOffset)
					{
						bestPnt = crsPnt;
						local =  crsPnt.x;
						endPnt = sgm[i].p1;
						nextSP = 2;
						matchType = 1;
						k = i;
					}

					if(matchType < 2 && abs(sgm[i].p1.y-sgm[i].p2.y)<3 && abs(sgm[i].p2.x-currSgm.p1.x) <= minOffset)
					{
						local = crsPnt.x;
						endPnt = sgm[i].p1;
						nextSP = 2;
						matchType = 2;
						k = i;
					}

					if(sgm[i].p2.y-crsPnt.y > minOffset)
						if(matchType < 3)
						{
							bestPnt = crsPnt;
							local = crsPnt.x;
							endPnt = sgm[i].p2;
							nextSP = 1;
							matchType = 3;
							k = i;
						}
						else
						{
							if(crsPnt.x > local)
							{
								bestPnt = crsPnt;
								local = crsPnt.x;
							    endPnt = sgm[i].p2;
								nextSP = 1;
								k = i;
							}
						}
				}
			}

			if(abs(currSgm.p1.x - currSgm.p2.x)<3)   //竖线
			{
				//无交点
				if(abs(sgm[i].p1.x-sgm[i].p2.x)<3)
				{
					if(abs(currSgm.p1.x-sgm[i].p1.x)>minOffset || currSgm.p1.y-sgm[i].p2.y > minOffset 
						|| sgm[i].p1.y-currSgm.p2.y > minOffset)
						continue;
				}
				else
				{
					if(currSgm.p1.x < sgm[i].p1.x-minOffset || currSgm.p1.x > sgm[i].p2.x+minOffset 
						|| sgm[i].p1.y < currSgm.p1.y-minOffset || sgm[i].p1.y > currSgm.p2.y + minOffset)
						continue;
				}

				//交点为初始点
				if(startPoint == 1 && (currSgm.p1.x > sgm[i].p1.x-minOffset && currSgm.p1.x < sgm[i].p2.x+minOffset
					&& currSgm.p1.y > sgm[i].p1.y-minOffset && currSgm.p1.y < sgm[i].p2.y+minOffset) )
					continue;
				if(startPoint == 2 && (currSgm.p2.x > sgm[i].p1.x-minOffset && currSgm.p2.x < sgm[i].p2.x+minOffset
					&& currSgm.p2.y > sgm[i].p1.y-minOffset && currSgm.p2.y < sgm[i].p2.y+minOffset) )
					continue;

				//交点
				crsPnt.x = currSgm.p1.x;
				crsPnt.y = sgm[i].p1.y;
				if(startPoint == 2 && abs(sgm[i].p1.x-sgm[i].p2.x)<3)
					crsPnt.y = sgm[i].p2.y;

				if(startPoint == 1)   //初始点为p1
				{   
					//第一种匹配
					if(matchType == 0 && abs(sgm[i].p1.y - sgm[i].p2.y)<3 && abs(sgm[i].p2.y-currSgm.p2.y) <= minOffset
						&& abs(sgm[i].p2.x-currSgm.p2.x) <= minOffset)
					{
						bestPnt = crsPnt;
						local =  crsPnt.y;
						endPnt = sgm[i].p1;
						nextSP = 2;
						matchType = 1;
						k = i;
					}

					//第二种匹配
					 if(matchType < 2 && abs(sgm[i].p1.x-sgm[i].p2.x)<3 && abs(sgm[i].p1.x-currSgm.p2.x) <= minOffset)
					{
						local = crsPnt.y;
						endPnt = sgm[i].p2;
						nextSP = 1;
						matchType = 2;
						k = i;
					}

					 //第三种匹配
					 if(sgm[i].p2.x-crsPnt.x > minOffset)
						if(matchType < 3)
						{
							bestPnt = crsPnt;
							local = crsPnt.y;
							endPnt = sgm[i].p2;
							nextSP = 1;
							matchType = 3;
							k = i;
						}
						else
						{
							if(crsPnt.y < local)
							{
								bestPnt = crsPnt;
								local = crsPnt.y;
							    endPnt = sgm[i].p2;
								nextSP = 1;
								k = i;
							}
						}
				}
				else   //初始点为p2
				{   
					//第一种匹配
					if(matchType == 0 && abs(sgm[i].p1.y - sgm[i].p2.y)<3 && 
						abs(sgm[i].p1.y-currSgm.p1.y) <= minOffset && abs(sgm[i].p1.x-currSgm.p1.x) <= minOffset)
					{
						bestPnt = crsPnt;
						local =  crsPnt.y;
						endPnt = sgm[i].p2;
						nextSP = 1;
						matchType = 1;
						k = i;
					}

					//第二种匹配
					if(matchType < 2 && abs(sgm[i].p1.x-sgm[i].p2.x)<3 && abs(sgm[i].p2.y-currSgm.p1.y) <= minOffset)
					{
						local = crsPnt.y;
						endPnt = sgm[i].p1;
						nextSP = 2;
						matchType = 2;
						k = i;
					}

					//第三种匹配
					if(crsPnt.x - sgm[i].p1.x> minOffset)
						if(matchType < 3)
						{
							bestPnt = crsPnt;
							local = crsPnt.y;
							endPnt = sgm[i].p1;
							nextSP = 2;
							matchType = 3;
							k = i;
						}
						else
						{
							if(crsPnt.y > local)
							{
								bestPnt = crsPnt;
								local = crsPnt.y;
							    endPnt = sgm[i].p1;
								nextSP = 2;
								k = i;
							}
						}
				}
			}
		}// end of for loop

		//cout<<matchType<<"  "<<endl;
		if(matchType == 0 )
		{
			//cout<<currSgm.p1.x<<"  "<<currSgm.p1.y<<"  "<<currSgm.p2.x<<"  "<<currSgm.p2.y<<endl;
			break;
		}

		if(bestPnt.x!=-1) crsPnt = bestPnt;

		if(abs(crsPnt.x-minPnt.x)<=minOffset && abs(crsPnt.y-minPnt.y)<=minOffset) break;

//////////////////////////////////////////////////BUG
		if( compare(crsPnt,endPnt) == -1 )
		{
		    currSgm.p1 = crsPnt;
			currSgm.p2 = endPnt;
		}
		else if( compare(crsPnt,endPnt) == 1 )
		{
			currSgm.p1 = endPnt;
			currSgm.p2 = crsPnt;
		}
		else break;
////////////////////////////////////////////////

		if((abs(crsPnt.x-currSgm.p1.x)<=minOffset && abs(crsPnt.y-currSgm.p1.y)) 
			|| (abs(crsPnt.x-currSgm.p2.x) <= minOffset && abs(crsPnt.y-currSgm.p2.y) <= minOffset))
		{
			outSgm.push_back(currSgm);
		}
		else
		{
			/*segment sgm1,sgm2;
			sgm1.p1 = currSgm.p1;
			sgm1.p2 = sgm2.p1 = crsPnt;
			sgm2.p2 = currSgm.p2;
			outSgm.push_back(sgm1);
			outSgm.push_back(sgm2);*/
			segment s;
			if(startPoint == 1)
			{
				s.p1 = currSgm.p1;
				s.p2 = crsPnt;
			}
			else
			{
				s.p1 = crsPnt;
				s.p2 = currSgm.p2;
			}
			outSgm.push_back(s);
		}

        points.push_back(crsPnt);
		startPoint = nextSP;
		curr = k;
	}  

	return points;
}

void ProjectCalculation::calculate(const char * filename)
{
	vector<segment> walls;
	points = findContour(segments,walls);
	//for(int i=0; i<points.size(); ++i)
	//	cout<<points[i].x<<"  "<<points[i].y<<endl;

	double lengthSum1=0, lengthSum2=0;
	for(int i=0; i<segments.size(); ++i)
		lengthSum1 += sgmLength(segments[i]);
	for(int i=0; i<walls.size(); ++i)
		lengthSum2 += sgmLength(walls[i]);
    double wallArea = lengthSum1*2-lengthSum2;

	double s=polygon_area(points.size(),points);
	//    cout/*<<"area :"*/<<s<<"  "<<lengthSum1*2-lengthSum2<<endl;

	//创建一个XML的文档对象
	TiXmlDocument *myDocument = NULL;
	myDocument = new TiXmlDocument();

	//if(myDocument == NULL) return false;
	if(!myDocument->LoadFile(filename)) return;
	//获得根元素
	TiXmlElement *RootElement = myDocument->RootElement();

	TiXmlElement * FloorAreaElem = new TiXmlElement("floor_area");
	TiXmlElement * WallAreaElem = new TiXmlElement("wall_area");
	RootElement->LinkEndChild(FloorAreaElem);
	RootElement->LinkEndChild(WallAreaElem);
	char str[16];
	itoa(s,str,10);
	TiXmlText * FloorAreaContent = new TiXmlText(str);
	itoa(wallArea,str,10);
	TiXmlText * WallAreaContent = new TiXmlText(str);
	myDocument->SaveFile(filename);
}

// 点的叉乘: AB * AC
int ProjectCalculation::cross(const point &A, const point &B, const point &C) {
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

double ProjectCalculation::polygon_area(const int &n, vector<point> p)
{
    double area = 0.0;
    int i;
    point temp;

    temp.x = temp.y = 0;//原点
    for (i = 0; i < n-1; ++i){
        area += cross(temp, p[i], p[i+1]);
    }
    area += cross(temp, p[n-1], p[0]);//首尾相连
    area = area/2.0;        //注意要除以2
    return area > 0 ? area : -area;    //返回非负数
}
