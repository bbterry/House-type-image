#include<iostream>
using namespace std;

struct Point { // 点结构体
    int x, y;
};

// 点的叉乘: AB * AC
int cross(const Point &A, const Point &B, const Point &C) {
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

double polygon_area(const int &n, Point p[])
{
    double area = 0.0;
    int i;
    Point temp;

    temp.x = temp.y = 0;//原点
    for (i = 0; i < n-1; ++i){
        area += cross(temp, p[i], p[i+1]);
    }
    area += cross(temp, p[n-1], p[0]);//首尾相连
    area = area/2.0;        //注意要除以2
    return area > 0 ? area : -area;    //返回非负数
}

void main()
{
	Point p[8];
	Point p0;
	p0.x=0;
	p0.y=0;
	Point p1;
	p1.x=0;
	p1.y=2;
	Point p2;
	p2.x=1;
	p2.y=2;
	Point p3;
	p3.x=1;
	p3.y=1;
	Point p4;
	p4.x=2;
	p4.y=1;
	Point p5;
	p5.x=2;
	p5.y=3;
	Point p6;
	p6.x=3;
	p6.y=3;
	Point p7;
	p7.x=3;
	p7.y=0;
	p[0]=p0;
	p[1]=p1;
	p[2]=p2;
	p[3]=p3;
	p[4]=p4;
	p[5]=p5;
	p[6]=p6;
	p[7]=p7;
	double s=polygon_area(8,p);
	cout<<"hello,area is:"<<s<<endl;
	system("pause");
}