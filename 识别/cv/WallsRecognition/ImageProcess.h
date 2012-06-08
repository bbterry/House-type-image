
#ifndef _WALLRECOGNITION_H_
#define _WALLRECOGNITION_H_

#include "cv.h"
#include "cxcore.h"
#include "highgui.h" 


typedef struct
{
	double x1,y1;
	double x2,y2;
}line;

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
	const IplImage * getPreImage() { return (const IplImage * )preImg;}

	void processImage(const char * imgfile, const char * xmlfile);
};

#endif