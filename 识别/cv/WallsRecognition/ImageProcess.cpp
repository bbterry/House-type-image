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
	findDoorAndWindow(walls);
	writeToXML(walls,xmlfile);
	//cvReleaseImage(&edgeImg);
	//cvReleaseImage(&preImg);
}


//these functions are prepared for solving sort problems in a vector
  bool   lessmark(const   door&   s1,const   door&   s2)   
  {   
      return   s1.length   <   s2.length;   
  } 

  bool   greatermark(const   door&   s1,const   door&   s2)   
  {   
      return   s1.length   >   s2.length;   
  }

   bool   lessX(const   line&   s1,const   line&   s2)   
  {   
      return   s1.x1   <   s2.x1;   
  }

    bool   lessY(const   line&   s1,const   line&   s2)   
  {   
      return   s1.y1   <   s2.y1;   
  }

void ImageProcess::findDoorAndWindow(vector<line> w )
{
	int tempL=0;
	int tempId=0,tempId2=0,tempId3=0;
	max=0;
	max2=0;
	max3=0;
	int count=0;
	door pos;  //variable ,used to maintain gap information
	int brk=0; //break point

	//find the break point where the horizontal line is before and vertical line is after
	vector<line>::iterator it;
	for(it=walls.begin();it!=walls.end();it++)
	{
		if(it->y1!=it->y2)
			break;
		brk++;
	}
	//sort every set of lines with the approximate same x,or y in order
	int trace=0;
	int i;
	 for(i=0;i<brk-1;i++)
	{
		while(i<brk-1&&abs(walls[i+1].y1-walls[i].y1)<DISTANCE_BLUR)
		{trace++;
		 i++;
		}
		sort(walls.begin()+i-trace,walls.begin()+i+1,lessX);
		trace=0;
	}

	for(;i<walls.size()-1;i++)
	{
		while(i<walls.size()-1&&abs(walls[i+1].x1-walls[i].x1)<DISTANCE_BLUR)
		{trace++;
		 i++;
		}
		sort(walls.begin()+i-trace,walls.begin()+i+1,lessY);
		trace=0;
	}
	//get gap line size and store in a vector v.
	for(int i=0;i<brk-1;i++)
	{
				if(abs(walls[i].y1-walls[i+1].y1)<DISTANCE_BLUR)
				{
					int l=walls[i+1].x1-walls[i].x2;
					pos.length=l;
					pos.x=walls[i].x2+l/2.0;
					pos.y=walls[i].y1;
					if(testWindH(pos.x,pos.y)&&pos.length<150)
						z.push_back(pos);
					else
					v.push_back(pos);
				}

	}
	//
	for(int i=brk;i<walls.size()-1;i++)
	{

				if(abs(walls[i].x1-walls[i+1].x1)<DISTANCE_BLUR)
				{
					int l=walls[i+1].y1-walls[i].y2;
					pos.length=l;
					pos.y=walls[i].y2+l/2.0;
					pos.x=walls[i].x1;
					if(testWindV(pos.x,pos.y)&&pos.length<150)
						z.push_back(pos);
					else
					v.push_back(pos);
					
				}

	}

	//sort the gap line in order,in order to find door in a more convenient way.
   sort(v.begin(),v.end(),lessmark);
   //find the doors ,whose quantity can be the max or next max.
	for(int i=0;i<v.size();i++)
	{
		if(abs(v[i].length-tempL)<DOOR_LENGTH_BLUR)
					{
						count++;
						tempL=v[i].length;
					}
					else 
					{
						
						if(tempL<60)
						{
						if(max<count)
						{
						max3=max2;
						max2=max;
						max=count;
						tempId3=tempId2;
						tempId2=tempId;
						tempId=i;
						}
						else if(max2<count)
						{
							max3=max2;
							tempId3=tempId2;
						max2=count;
						tempId2=i;
						}
						else if(max3<count)
						{
							
							max3=count;
							tempId3=i;
						}
						count=1;
						tempL=v[i].length;
						}
					}
	}
    //remember the place of doors in vector.
	flag=v.begin()+tempId;
	if(max2>1)
	flag2=v.begin()+tempId2;
	if(max3>1)
	flag3=v.begin()+tempId3;

}

bool ImageProcess::testWindH(int x,int y)
{   
	int j=0;
	int sum=0;
	int temp=128;
	IplImage* img=cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	cvCvtColor( src, img, CV_BGR2GRAY );
	//	while(j<11)
	//{
	//	/*if(abs(temp-*pixel(img,x,y-5+j))>80)
	//	{
	//		sum++;
	//	}*/
	//	sum+=abs(temp-*pixel(img,x,y-5+j));
	//	temp=*pixel(img,x,y-5+j);
	//	j++;
	//}
	for(j=0;j<20;j++)
	{
		for(int i=0;i<10;i++)
		{
			sum+=255-*pixel(img,x-5+i,y-10+j);
		}
	}
	if(sum>10*200*4)
		return true;
	else 
		return false;
}

bool ImageProcess::testWindV(int x,int y)
{
	int j=0;
	int sum=0;
	int temp=128;
	IplImage* img=cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	cvCvtColor( src, img, CV_BGR2GRAY );
	//	while(j<10)
	//{
	//	/*if(abs(temp-*pixel(img,x-5+j,y))>80)
	//	{
	//		sum++;
	//	}*/
	//	/*sum+=abs(temp-*pixel(img,x-5+j,y));
	//	temp=*pixel(img,x-5+j,y);*/
	//	j++;
	//}
	for(j=0;j<10;j++)
	{
		for(int i=0;i<20;i++)
		{
			sum+=255-*pixel(img,x-10+i,y-5+j);
		}
	}
	if(sum>10*200*4)
		return true;
	else 
		return false;
}

