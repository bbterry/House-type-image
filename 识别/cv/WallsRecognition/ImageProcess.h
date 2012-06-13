
#ifndef _WALLRECOGNITION_H_
#define _WALLRECOGNITION_H_

#include "cv.h"
#include "cxcore.h"
#include "highgui.h" 
 #include <algorithm>
#include <functional>

#define DISTANCE_BLUR 4
#define DOOR_LENGTH_BLUR 3
typedef struct
{
	double x1,y1;
	double x2,y2;
}line;

typedef struct
{
	double x,y;
	int length;
}door;

class ImageProcess
{
private:
	IplImage *src;     //输入图像
	IplImage * preImg;  //预处理输出图像
	int patch;         //预处理中使用的窗口大小
	int threshold;     //预处理中使用的阈值
	int itera;         //预处理中使用的迭代次数
	int joindist;      //线段合并最大距离
	int minLineCount;  //存在线段的最小统计数
	int maxSeek;       //搜索线段的最小距离
	int minLineLength; //线段的最小长度

	vector<line> walls;
	vector<door> v;

	uchar * pixel(IplImage * img, int x, int y)      //取像素值
	{ return (uchar*)(img->imageData+y*img->widthStep+x*img->nChannels);}

	//边缘提取
	void edge(IplImage * img, IplImage * edges);

	//图像预处理
	void preProcess(IplImage *img);

	void recognition(IplImage *img);
	void writeToXML(vector<line> w,const char * filename);

public:
	//迭代器，访问walls容器
	vector<line>::const_iterator iter;
	vector<line>::iterator vec_begin() { return walls.begin();}
	vector<line>::iterator vec_end() { return walls.end();}

	//iterator for doors
	vector<door>::const_iterator iter1;
	vector<door>::iterator flag,flag2,flag3;
	vector<door>::iterator vec_begin1() { return v.begin();}
	vector<door>::iterator vec_end1() { return v.end();}
	int max,max2,max3;


	ImageProcess();
	~ImageProcess();

	void setPatch(int p) {patch = p;}
	void setThreshold(int t) {threshold = t;}
	void setItera(int i) {itera=i;}
	void setJoindist(int j) {joindist = j;}
	//void setMinLineCount(int c) {minLineCount = c;}
	void setMaxSeek(int s) {maxSeek = s;}
	void setminLineLength(int l) {minLineLength = l;}
	const IplImage * getSrcImage() { return (const IplImage * )src;}
	 IplImage * getPreImage() { return ( IplImage * )preImg;}

	void processImage(const char * imgfile, const char * xmlfile);
	void findDoor(vector<line> w);
};

#endif