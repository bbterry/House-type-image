#include "StdAfx.h"
#include "ImageProcess.h"
#include<iomanip>
#include<fstream>

ImageProcess::ImageProcess()
{
	src = 0;
	preImg = 0;
	patch = 7;
	threshold = 4800;
	itera = 1;
	joindist = 12;
	minLineCount = 48;
	maxSeek = 2;
	minLineLength = 16;
}

ImageProcess::~ImageProcess()
{
	if(src!=0) cvReleaseImage(&src);
	if(preImg != 0) cvReleaseImage(&preImg);
}

void ImageProcess::edge(IplImage * img, IplImage * edges)
{
	for(int y=0; y<img->height; y++)
	{
		for(int x=0; x<img->width; x++)
		{
			if(y==0||y==img->height||x==0||x==img->width || *pixel(img,x,y)!=0)
			{
				*pixel(edges,x,y)=0;
				continue;
			}
			
			int r=abs(*pixel(img,x+1,y)-*pixel(img,x-1,y)) + abs(*pixel(img,x,y+1)-*pixel(img,x,y-1));
			if(r>200) *pixel(edges,x,y)=255;
			else *pixel(edges,x,y)=0;
		}
	}
}

void ImageProcess::preProcess(IplImage *dst)
{
    IplImage  *tmp;
	tmp=cvCreateImage(cvGetSize(dst),8,1);
    
	uchar *pElementTmp = 0;
	uchar *pElementDst = 0;

	while( itera > 0)
	{
		cvCopy(dst,tmp,0);
		cvSmooth(dst,tmp,CV_GAUSSIAN,7);
		for(int y=0; y<tmp->height; y++)
		{
			for(int x=0; x<tmp->width; x++)
			{

				pElementTmp = pixel(tmp,x,y);
				pElementDst = pixel(dst,x,y);

				if( y<patch/2 || y>=tmp->height-patch/2
					|| x<patch/2 || x>=tmp->width-patch/2)
				{
					*pElementDst = 255;
					continue;
				}

				int sum = 0;
				for(int j=y-patch/2; j<=y+patch/2; j++)
					for(int i=x-patch/2; i<x+patch/2; i++)
					{
						sum += *pElementTmp;
					}

				if(sum > threshold)
					*pElementDst = 255;
				else
					*pElementDst = 0;

			}
		}
		
		--itera;
	}
	cvReleaseImage(&tmp);
}

void ImageProcess::recognition(IplImage * img)
{
	//像素统计量
	/*int *xColle = new int[img->width];     
	int *yColle = new int[img->height];

	for(int x=0; x<img->width; x++)	
		xColle[x]=0;
	for(int y=0; y<img->height; y++)
		yColle[y]=0;

	for(int y=0; y<img->height; y++)
	{
		for(int x=0; x<img->width; x++)
		{
			if(*pixel(img,x,y) > 128)
			{
				++xColle[x];
				++yColle[y];
			}
		}
	}*/

	//线段容器
	vector<line> xLinesSegme;
	vector<line> yLinesSegme;

		//识别水平线
		for(int y=0; y<img->height; y++)
		{
			//if(yColle[y]>minLineCount)
			//{
				bool seek = false;
                int minX=0;
				int maxX=0;
				for(int x=0; x<img->width; x++)
				{
					if(!seek)
					{
						while(x<img->width && *pixel(img,x,y)!=0)
							++x;
						if(x<img->width)
						{
							minX=x;
							seek=true;
						}
					}

					if(seek)
					{
						if(*pixel(img,x,y)==0)
							maxX=x;
						else
						{
							bool flag = false;
							for(int i=1; i<=maxSeek; i++)
							{
								if(x+i<img->width && *pixel(img,x,y)==0)
								{
									x=x+i;
									maxX=x;
									flag=true;
									break;
								}
							}
							if(!flag)
							{
								seek = false;
								if( maxX-minX >= minLineLength )
								{
									line l;
									l.x1=minX;
									l.x2=maxX;
									l.y1=l.y2=y;
									xLinesSegme.push_back(l);
								}
							}
						}
					}
				}
			//}
		}

		int *remove = new int[xLinesSegme.size()];
		for(int i=0; i<xLinesSegme.size(); i++)
			remove[i]=0;

		for(int i=0; i<xLinesSegme.size(); i++)
		{
			if(remove[i]) continue;

			line megline=xLinesSegme[i];
			bool flag = true;
			vector<int> info;  //保存信息，用于计算线段精确的y坐标

			while(flag)
			{
				flag = false;
				for(int j=i; j<xLinesSegme.size(); j++)
				{
					if(remove[j]) continue;

					line l=xLinesSegme[j];
					if( abs(megline.y1-l.y1)>joindist || l.x1-megline.x2>joindist 
						|| megline.x1-l.x2>joindist)
					{
						continue;
					}

					//合并线段
					if(l.x1-megline.x2>=0 && l.x1-megline.x2<=joindist)
					{
						megline.x2 = l.x2;
						//megline.y1 = megline.y2 = (megline.y1 + l.y1) / 2;
					}
					else if(megline.x1-l.x2>=0 && megline.x1-l.x2<=joindist)
					{
						megline.x1 = l.x1;
						//megline.y1 = megline.y2 = (megline.y1 + l.y1) / 2;
					}
					else if(l.x1>megline.x1 && l.x1<megline.x2)
					{
						megline.x2 = (megline.x2<l.x2) ? l.x2 : megline.x2;
						//megline.y1 = megline.y2 = (megline.y1 + l.y1) / 2;
					}
					else if(l.x2<megline.x2)
					{
						megline.x1=l.x1;
						//megline.y1 = megline.y2 = (megline.y1 + l.y1) / 2;
					}
					else
					{
						megline.x1=l.x1;
						megline.x2=l.x2;
						//megline.y1 = megline.y2 = (megline.y1 + l.y1) / 2;
					}
					megline.y1 = megline.y2 = megline.y1 + (l.y1-megline.y1) * megline.y1 / (megline.y1 + l.y1);  //y的近似坐标
					remove[j]=1;
                    flag = true;

					//信息格式：（像素个数，y坐标）
					info.push_back(l.x2-l.x1+1);
					info.push_back (l.y1);

					
				}
			}
			////////////////计算y的精确坐标
			double sum=0;
			for(int i=0; i<info.size(); i+=2)
				sum += info[i];

			if(sum < minLineCount) continue;  //墙体最少像素约束

			sum = sum / 2;
			for(int i=0; i<info.size(); i+=2)
			{
				sum -= info[i];
				if(sum <=0) 
				{
					megline.y1 = megline.y2 = info[i+1];
				    break;
				}
			}
			//////////////////

			walls.push_back(megline);
		}
		delete [] remove;


		//识别垂直线
		for(int x=0; x<img->width; x++)
		{
			//if(xColle[x]>minLineCount)
			//{
				bool seek = false;
                int minY=0;
				int maxY=0;
				for(int y=0; y<img->height; y++)
				{
					if(!seek)
					{
						while(y<img->height && *pixel(img,x,y)!=0)
							++y;
						if(y<img->height)
						{
							minY=y;
							seek=true;
						}
					}

					if(seek)
					{
						if(*pixel(img,x,y)==0)
							maxY=y;
						else
						{
							bool flag = false;
							for(int i=1; i<=4; i++)
							{
								if(y+i<img->height && *pixel(img,x,y)==0)
								{
									y=y+i;
									maxY=y;
									flag=true;
									break;
								}
							}
							if(!flag)
							{
								seek = false;
								if( maxY-minY >= minLineLength )
								{
									line l;
									l.y1=minY;
									l.y2=maxY;
									l.x1=l.x2=x;
									yLinesSegme.push_back(l);
								}
							}
						}
					}
				}
			//}
		}

		remove = new int[yLinesSegme.size()];
		for(int i=0; i<yLinesSegme.size(); i++)
			remove[i]=0;

		for(int i=0; i<yLinesSegme.size(); i++)
		{
			if(remove[i]) continue;

			line megline=yLinesSegme[i];
			bool flag = true;
			vector<int> info;   //保存信息，用于计算线段精确的x坐标

			while(flag)
			{
				flag = false;
				for(int j=i; j<yLinesSegme.size(); j++)
				{
					if(remove[j]) continue;

					line l=yLinesSegme[j];
					if( abs(megline.x1-l.x1)>joindist || l.y1-megline.y2>joindist 
						|| megline.y1-l.y2>joindist)
					{
						continue;
					}

					//合并线段
					if(l.y1-megline.y2>=0 && l.y1-megline.y2<=joindist)
					{
						megline.y2 = l.y2;
						//megline.x1 = megline.x2 = (megline.x1 + l.x1) / 2;
					}
					else if(megline.y1-l.y2>=0 && megline.y1-l.y2<=joindist)
					{
						megline.y1 = l.y1;
						//megline.x1 = megline.x2 = (megline.x1 + l.x1) / 2;
					}
					else if(l.y1>megline.y1 && l.y1<megline.y2)
					{
						megline.y2 = (megline.y2<l.y2) ? l.y2 : megline.y2;
						//megline.x1 = megline.x2 = (megline.x1 + l.x1) / 2;
					}
					else if(l.y2<megline.y2)
					{
						megline.y1=l.y1;
						//megline.x1 = megline.x2 = (megline.x1 + l.x1) / 2;
					}
					else
					{
						megline.y1=l.y1;
						megline.y2=l.y2;
						//megline.x1 = megline.x2 = (megline.x1 + l.x1) / 2;
					}
					megline.x1 = megline.x2 = megline.x1+(l.x1-megline.x1)*megline.x1/(megline.x1 + l.x1); //x的近似坐标
					remove[j]=1;
					info.push_back(l.y2-l.y1+1);
					info.push_back(l.x1);
					flag = true;
				}
			}
			////////////////计算x的精确坐标
			double sum = 0;
			for(int i=0; i<info.size(); i+=2)
				sum += info[i];

			if(sum < minLineCount) continue;

			sum /= 2;
			for(int i=0; i<info.size(); i+=2)
			{
				sum -= info[i];
				if(sum <= 0)
				{
					megline.x1=megline.x2=info[i+1];
					break;
				}
			}
			/////////////////

			walls.push_back(megline);
		}
		delete [] remove;

		//delete [] xColle;
		//delete [] yColle;
}

void ImageProcess::writeToXML(vector<line> w,const char * filename)
{
	ofstream of(filename);
	of<<"<root>";
	for(int i=0; i<w.size(); i++)
	{
		of<<"<line>";
		  of<<"<point>";
		    of<<"<x>"<<w[i].x1<<"</x>";
			of<<"<y>"<<w[i].y1<<"</y>";
		  of<<"</point>";
		  of<<"<point>";
		    of<<"<x>"<<w[i].x2<<"</x>";
			of<<"<y>"<<w[i].y2<<"</y>";
		  of<<"</point>";
		of<<"</line>";
	}
	of<<"</root>";
	of.close();
}

void ImageProcess::processImage(const char * imgfile, const char * xmlfile)
{
	if(src!=0) cvReleaseImage(&src);
	src = cvLoadImage(imgfile,1);

	if(preImg!=0) cvReleaseImage(&preImg);
	preImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);

	cvCvtColor( src, preImg, CV_BGR2GRAY );

	preProcess(preImg);

	//IplImage * edgeImg = cvCreateImage(cvGetSize(preImg), IPL_DEPTH_8U, 1);
	//edge(preImg,edgeImg);
	//recognition(edgeImg);
	recognition(preImg);

	writeToXML(walls,xmlfile);
	//cvReleaseImage(&edgeImg);
	//cvReleaseImage(&preImg);
}