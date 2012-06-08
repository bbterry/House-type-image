#define T_CPP
#ifdef T_CPP
#include "cv.h"
#include "cxcore.h"
#include "highgui.h" 
using namespace std;

typedef struct
{
	double x1,y1;
	double x2,y2;
}line;

void edge(IplImage * img, IplImage * edges);
uchar * pixel(IplImage * img, int x, int y);

int main( int argc, char** argv ) 
{
	    //声明IplImage指针
         IplImage* pImg = NULL; 
	    IplImage* pCannyImg = NULL; 
		IplImage *dst =NULL;
	    //载入图像，强制转化为Gray
		pImg = cvLoadImage( "E:\\terry\\2.jpg", 0); 
		//为canny边缘图像申请空间
		pCannyImg = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
		dst=cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 3);
		//canny边缘检测
		edge(pImg, pCannyImg); 
	    cvCvtColor( pCannyImg, dst, CV_GRAY2BGR );
		/*vector<CvPoint> *xColle = new vector<CvPoint>[pCannyImg->width];
		vector<CvPoint> *yColle = new vector<CvPoint>[pCannyImg->height];*/

		int *xColle = new int[pCannyImg->width];
		int *yColle = new int[pCannyImg->height];

	    for(int x=0; x<pCannyImg->width; x++)	
			xColle[x]=0;
		for(int y=0; y<pCannyImg->height; y++)
			yColle[y]=0;

		for(int y=0; y<pCannyImg->height; y++)
		{
			for(int x=0; x<pCannyImg->width; x++)
			{
				if(*pixel(pCannyImg,x,y) > 128)
				{
					++xColle[x];
					++yColle[y];
				}
			}
		}

		vector<line> xLinesSegme;
		vector<line> yLinesSegme;
		vector<line> wallLines;

		//识别水平线
		for(int y=0; y<pCannyImg->height; y++)
		{
			if(yColle[y]>20)
			{
				bool seek = false;
                int minX=0;
				int maxX=0;
				for(int x=0; x<pCannyImg->width; x++)
				{
					if(!seek)
					{
						while(x<pCannyImg->width && *pixel(pCannyImg,x,y)!=255)
							++x;
						if(x<pCannyImg->width)
						{
							minX=x;
							seek=true;
						}
					}

					if(seek)
					{
						if(*pixel(pCannyImg,x,y)==255)
							maxX=x;
						else
						{
							bool flag = false;
							for(int i=1; i<=4; i++)
							{
								if(x+i<pCannyImg->width && *pixel(pCannyImg,x,y)==255)
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
								if( maxX-minX >= 6 )
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
			}
		}

		int joindist=8;
		int *remove = new int[xLinesSegme.size()];
		for(int i=0; i<xLinesSegme.size(); i++)
			remove[i]=0;

		for(int i=0; i<xLinesSegme.size(); i++)
		{
			if(remove[i]) continue;

			line megline=xLinesSegme[i];
			bool flag = true;
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
						megline.y1 = megline.y2 = (megline.y1 + l.y1) / 2;
					}
					else if(megline.x1-l.x2>=0 && megline.x1-l.x2<=joindist)
					{
						megline.x1 = l.x1;
						megline.y1 = megline.y2 = (megline.y1 + l.y1) / 2;
					}
					else if(l.x1>megline.x1 && l.x1<megline.x2)
					{
						megline.x2 = (megline.x2<l.x2) ? l.x2 : megline.x2;
						megline.y1 = megline.y2 = (megline.y1 + l.y1) / 2;
					}
					else if(l.x2<megline.x2)
					{
						megline.x1=l.x1;
						megline.y1 = megline.y2 = (megline.y1 + l.y1) / 2;
					}
					else
					{
						megline.x1=l.x1;
						megline.x2=l.x2;
						megline.y1 = megline.y2 = (megline.y1 + l.y1) / 2;
					}
					remove[j]=1;
					flag = true;
				}
			}
			wallLines.push_back(megline);
		}
		delete [] remove;


		//识别垂直线
		for(int x=0; x<pCannyImg->width; x++)
		{
			if(xColle[x]>20)
			{
				bool seek = false;
                int minY=0;
				int maxY=0;
				for(int y=0; y<pCannyImg->height; y++)
				{
					if(!seek)
					{
						while(y<pCannyImg->height && *pixel(pCannyImg,x,y)!=255)
							++y;
						if(y<pCannyImg->height)
						{
							minY=y;
							seek=true;
						}
					}

					if(seek)
					{
						if(*pixel(pCannyImg,x,y)==255)
							maxY=y;
						else
						{
							bool flag = false;
							for(int i=1; i<=4; i++)
							{
								if(y+i<pCannyImg->height && *pixel(pCannyImg,x,y)==255)
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
								if( maxY-minY >= 6 )
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
			}
		}

		remove = new int[yLinesSegme.size()];
		for(int i=0; i<yLinesSegme.size(); i++)
			remove[i]=0;

		for(int i=0; i<yLinesSegme.size(); i++)
		{
			if(remove[i]) continue;

			line megline=yLinesSegme[i];
			bool flag = true;
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
						megline.x1 = megline.x2 = (megline.x1 + l.x1) / 2;
					}
					else if(megline.y1-l.y2>=0 && megline.y1-l.y2<=joindist)
					{
						megline.y1 = l.y1;
						megline.x1 = megline.x2 = (megline.x1 + l.x1) / 2;
					}
					else if(l.y1>megline.y1 && l.y1<megline.y2)
					{
						megline.y2 = (megline.y2<l.y2) ? l.y2 : megline.y2;
						megline.x1 = megline.x2 = (megline.x1 + l.x1) / 2;
					}
					else if(l.y2<megline.y2)
					{
						megline.y1=l.y1;
						megline.x1 = megline.x2 = (megline.x1 + l.x1) / 2;
					}
					else
					{
						megline.y1=l.y1;
						megline.y2=l.y2;
						megline.x1 = megline.x2 = (megline.x1 + l.x1) / 2;
					}
					remove[j]=1;
					flag = true;
				}
			}
			wallLines.push_back(megline);
		}
		delete [] remove;


		for(int i=0; i<wallLines.size(); i++)
		{
			CvPoint p1;
			p1.x=wallLines[i].x1;
			p1.y=wallLines[i].y1;
			//cout<<p1.x<<"  "<<p1.y<<endl;
			CvPoint p2;
			p2.x=wallLines[i].x2;
			p2.y=wallLines[i].y2;
			//cout<<p2.x<<"  "<<p2.y<<endl;
			cvLine( dst, p1, p2, CV_RGB(255,0,0), 2, CV_AA, 0 );
			//cvCircle(dst,p1,2,CV_RGB(255,0,0),2,CV_AA,0);
			//cvCircle(dst,p2,2,CV_RGB(255,0,0),2,CV_AA,0);
		}

		//创建窗口
		cvNamedWindow("src", 1);
		cvNamedWindow("canny",1);
		//cvNamedWindow("dst",1);
		//显示图像
		cvShowImage( "src", pImg ); 
		cvShowImage( "canny", dst ); 
		cvSaveImage("dst.png",dst);
		//cvShowImage("dst",dst);
		//等待按键
		cvWaitKey(0);
		//销毁窗口
		cvDestroyWindow( "src" ); 
		cvDestroyWindow( "canny" );
		//cvDestroyWindow( "dst" );
		//释放图像
		cvReleaseImage( &pImg ); 
		cvReleaseImage( &pCannyImg );
		cvReleaseImage( &dst );
		delete [] xColle;
		delete [] yColle;
		return 0;
	
}

//img为单通道灰度图
uchar * pixel(IplImage * img, int x, int y)
{
	return (uchar*)(img->imageData+y*img->widthStep+x*img->nChannels);
}
#endif

void edge(IplImage * img, IplImage * edges)
{
	for(int y=0; y<img->height; y++)
	{
		for(int x=0; x<img->width; x++)
		{
			if(y==0||y==img->height||x==0||x==img->width || *pixel(img,x,y)>128)
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