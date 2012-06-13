
#ifndef _WALLRECOGNITION_H_
#define _WALLRECOGNITION_H_

#include "cv.h"
#include "cxcore.h"
#include "highgui.h" 
 #include <algorithm>
#include <functional>

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
	IplImage *src;     //����ͼ��
	IplImage * preImg;  //Ԥ�������ͼ��
	int patch;         //Ԥ������ʹ�õĴ��ڴ�С
	int threshold;     //Ԥ������ʹ�õ���ֵ
	int itera;         //Ԥ������ʹ�õĵ�������
	int joindist;      //�߶κϲ�������
	int minLineCount;  //�����߶ε���Сͳ����
	int maxSeek;       //�����߶ε���С����
	int minLineLength; //�߶ε���С����

	vector<line> walls;
	vector<door> v;

	uchar * pixel(IplImage * img, int x, int y)      //ȡ����ֵ
	{ return (uchar*)(img->imageData+y*img->widthStep+x*img->nChannels);}

	//��Ե��ȡ
	void edge(IplImage * img, IplImage * edges);

	//ͼ��Ԥ����
	void preProcess(IplImage *img);

	void recognition(IplImage *img);
	void writeToXML(vector<line> w,const char * filename);

public:
	//������������walls����
	vector<line>::const_iterator iter;
	vector<line>::iterator vec_begin() { return walls.begin();}
	vector<line>::iterator vec_end() { return walls.end();}

	//
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