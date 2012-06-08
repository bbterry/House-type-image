#include "cv.h"
#include "highgui.h"
#include "math.h"
#include <vector>
#include<iostream>

using namespace std;

//#if 1
//uchar * element(IplImage * img, int x, int y);
//void cvThin( IplImage* src, IplImage* dst, int iterations);
// 
//int main(int argc, char** argv)
//{
//    IplImage *src = cvLoadImage( "1.jpg" , 1 );
//    IplImage *dst, *tmp;
//
//	if( !src )
//        return -1;
//
//    
//	dst=cvCreateImage(cvGetSize(src),8,1);
//	cvCvtColor( src, dst, CV_BGR2GRAY );
//	tmp=cvCreateImage(cvGetSize(dst),8,1);
//    
//	int patch = 7;
//	int threshold = 3000;
//	uchar *pElementTmp = 0;
//	uchar *pElementDst = 0;
//	int itera = 3;
//
//	while( itera > 0)
//	{
//		cvCopy(dst,tmp,0);
//		cvSmooth(dst,tmp,CV_GAUSSIAN,5);
//		for(int y=0; y<tmp->height; y++)
//		{
//			for(int x=0; x<tmp->width; x++)
//			{
//
//				pElementTmp = element(tmp,x,y);
//				pElementDst = element(dst,x,y);
//
//				if( y<patch/2 || y>=tmp->height-patch/2
//					|| x<patch/2 || x>=tmp->width-patch/2)
//				{
//					*pElementDst = 255;
//					continue;
//				}
//
//				int sum = 0;
//				for(int j=y-patch/2; j<=y+patch/2; j++)
//					for(int i=x-patch/2; i<x+patch/2; i++)
//					{
//						sum += *pElementTmp;
//					}
//
//				if(sum > threshold)
//					*pElementDst = 255;
//				else
//					*pElementDst = 0;
//
//			}
//		}
//		
//		--itera;
//	}
//
//	//
//
//	//
//	 //cvThin(dst,dst,1);
//	 cvErode( dst, dst, NULL, 1);                        //3*3腐蚀模板过滤，以去掉零星的噪声 
//     //cvDilate(dst, dst, NULL, 1); 
//	//cvLaplace(dst,dst,7) ;
//
//	cvNamedWindow( "src", 1 );
//    cvShowImage( "src", src );
// 
//    cvNamedWindow( "dst", 1 );
//    cvShowImage( "dst", dst );
//
//	/*cvNamedWindow( "tmp", 1 );
//    cvShowImage( "tmp", tmp );*/
//    cvSaveImage("2.jpg",dst);
//    cvWaitKey(0);
//	cvReleaseImage(&src);
//    cvReleaseImage(&dst);
//   cvReleaseImage(&tmp);
// 
//    return 0;
//
//
//}
//
//img为单通道灰度图
uchar * element(IplImage * img, int x, int y)
{
	return (uchar*)(img->imageData+y*img->widthStep+x*img->nChannels);
}
////细化
//void cvThin( IplImage* src, IplImage* dst, int iterations=1)
//{
//    CvSize size = cvGetSize(src);
//    cvCopy(src, dst);
//
//    for(int n=0; n<iterations; n++)
//    {
//        IplImage* t_image = cvCloneImage(dst);
//        for(int i=0; i<size.height;  i++)
//        {
//            for(int j=0; j<size.width; j++)
//            {
//                if(CV_IMAGE_ELEM(t_image,byte,i,j)==1)
//                {
//                    int ap=0;
//                    int p2 = (i==0)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j);
//                    int p3 = (i==0 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j+1);
//                    if (p2==0 && p3==1)
//                    {
//                        ap++;
//                    }
//                    int p4 = (j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i,j+1);
//                    if(p3==0 && p4==1)
//                    {
//                        ap++;
//                    }
//                    int p5 = (i==size.height-1 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j+1);
//                    if(p4==0 && p5==1)
//                    {
//                        ap++;
//                    }
//                    int p6 = (i==size.height-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j);
//                    if(p5==0 && p6==1)
//                    {
//                        ap++;
//                    }
//                    int p7 = (i==size.height-1 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j-1);
//                    if(p6==0 && p7==1)
//                    {
//                        ap++;
//                    }
//                    int p8 = (j==0)?0:CV_IMAGE_ELEM(t_image,byte,i,j-1);
//                    if(p7==0 && p8==1)
//                    {
//                        ap++;
//                    }
//                    int p9 = (i==0 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i-1,j-1);
//                    if(p8==0 && p9==1)
//                    {
//                        ap++;
//                    }
//                    if(p9==0 && p2==1)
//                    {
//                        ap++;
//                    }
//                    if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7)
//                    {
//                        if(ap==1)
//                        {
//                            if(!(p2&&p4&&p6))
//                            {
//                                if(!(p4&&p6&&p8))
//                                {
//                                    CV_IMAGE_ELEM(dst,byte,i,j)=0;
//                                }
//                            }
//                        }
//                    }
//               
//                }
//            }
//        }
//        cvReleaseImage(&t_image);
//        t_image = cvCloneImage(dst);
//        for(int i=0; i<size.height;  i++)
//        {
//            for(int j=0; j<size.width; j++)
//            {
//                if(CV_IMAGE_ELEM(t_image,byte,i,j)==1)
//                {
//                    int ap=0;
//                    int p2 = (i==0)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j);
//                    int p3 = (i==0 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j+1);
//                    if (p2==0 && p3==1)
//                    {
//                        ap++;
//                    }
//                    int p4 = (j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i,j+1);
//                    if(p3==0 && p4==1)
//                    {
//                        ap++;
//                    }
//                    int p5 = (i==size.height-1 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j+1);
//                    if(p4==0 && p5==1)
//                    {
//                        ap++;
//                    }
//                    int p6 = (i==size.height-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j);
//                    if(p5==0 && p6==1)
//                    {
//                        ap++;
//                    }
//                    int p7 = (i==size.height-1 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j-1);
//                    if(p6==0 && p7==1)
//                    {
//                        ap++;
//                    }
//                    int p8 = (j==0)?0:CV_IMAGE_ELEM(t_image,byte,i,j-1);
//                    if(p7==0 && p8==1)
//                    {
//                        ap++;
//                    }
//                    int p9 = (i==0 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i-1,j-1);
//                    if(p8==0 && p9==1)
//                    {
//                        ap++;
//                    }
//                    if(p9==0 && p2==1)
//                    {
//                        ap++;
//                    }
//                    if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7)
//                    {
//                        if(ap==1)
//                        {
//                            if(!(p2&&p4&&p8))
//                            {
//                                if(!(p2&&p6&&p8))
//                                {
//                                    CV_IMAGE_ELEM(dst, byte,i,j)=0;
//                                }
//                            }
//                        }
//                    }                    
//                }
//            
//            }
//         
//        }            
//        cvReleaseImage(&t_image);
//    }
//   
//}
//#else
//
//int main(int argc, char** argv)
//{
//    const char* filename = argc >= 2 ? argv[1] : "2.jpg";
//    IplImage* src = cvLoadImage( filename, 0 );
//    IplImage* dst;
//    IplImage* color_dst;
//    CvMemStorage* storage = cvCreateMemStorage(0);
//    CvSeq* lines = 0;
//    int i;
// 
//    if( !src )
//        return -1;
// 
//    dst = cvCreateImage( cvGetSize(src), 8, 1 );
//    color_dst = cvCreateImage( cvGetSize(src), 8, 3 );
// 
//    cvCanny( src, dst, 50, 200, 3 );
//    cvCvtColor( dst, color_dst, CV_GRAY2BGR );
//#if 0
//    lines = cvHoughLines2( dst, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 100, 0, 0 );
// 
//    for( i = 0; i < MIN(lines.total,100); i++ )
//    {
//        float* line = (float*)cvGetSeqElem(lines,i);
//        float rho = line[0];
//        float theta = line[1];
//        CvPoint pt1, pt2;
//        double a = cos(theta), b = sin(theta);
//        double x0 = a*rho, y0 = b*rho;
//        pt1.x = cvRound(x0 + 1000*(-b));
//        pt1.y = cvRound(y0 + 1000*(a));
//        pt2.x = cvRound(x0 - 1000*(-b));
//        pt2.y = cvRound(y0 - 1000*(a));
//        cvLine( color_dst, pt1, pt2, CV_RGB(255,0,0), 3, CV_AA, 0 );
//    }
//#else
//    lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 10, 3, 5 );
//    for( i = 0; i < lines.total; i++ )
//    {
//        CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
//		/*if(line[0].x!=line[1].x&&line[0].y!=line[1].y){
//			if((line[1].x-line[0].x)>){
//			cvSeqRemove(lines,i);
//			continue;
//			}
//		}*/
//        cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, CV_AA, 0 );
//    }
//#endif
//    cvNamedWindow( "Source", 1 );
//    cvShowImage( "Source", src );
// 
//    cvNamedWindow( "Hough", 1 );
//    cvShowImage( "Hough", color_dst );
// 
//    cvWaitKey(0);
// 
//    return 0;
//}
//
//#endif

#define LINE 1
#define LINE_LENGTH_SMALLEST 20
struct segment{
	CvPoint head;
	CvPoint tail;
	int length;
	int thickness;
	bool isTerminated;
	bool del;
};

int main()
{
	IplImage* src;
	if((src = cvLoadImage("1.jpg"))!=0)
	{
		IplImage* dst = cvCloneImage( src );
		vector<segment> lines;
		vector<segment> lineList;
		vector<segment> preList;
		vector<segment>::iterator line_iterator;
		vector<segment>::iterator pre_iterator;

		//detect horizontal line
		for(int i=0; i<src->height; i++)
		{
			if(i!=0)
				lineList.clear();
			bool hashead=false;
			struct segment sg;
			for(int j=0; j<src->width; j++)
			{
				if((int)*element(src,j,i)<128&&hashead==false)
				{						
					sg.head.x=j;
					sg.head.y=i;
					sg.thickness=1;
					sg.isTerminated=false;
					sg.del=false;
					hashead=true;


				}
				if((int)*element(src,j,i)>128&&hashead==true)
				{

					sg.tail.x=j-1;
					sg.tail.y=i;
					sg.length=sg.tail.x-sg.head.x+1;
					hashead=false;	
					if(sg.length>=LINE_LENGTH_SMALLEST)
						lineList.push_back(sg);
				}

			}
			

			//comparison
			int flag=0;
			pre_iterator=preList.begin();
			while( pre_iterator != preList.end() ) {
				boolean hasMatched=false;
				line_iterator=lineList.begin()+flag;
				while( line_iterator != lineList.end()&&hasMatched==false ) {
					if(/*abs(pre_iterator->length-line_iterator->length)<3&&abs(pre_iterator->head.x-line_iterator->head.x)<2*/
						(pre_iterator->tail.x>line_iterator->head.x)&&(0.8<(float)pre_iterator->length/line_iterator->length<1.2)
						||((pre_iterator->head.x==line_iterator->head.x)||(pre_iterator->tail.x==line_iterator->tail.x))&&
						((float)pre_iterator->length/line_iterator->length>0.5))
					{
						line_iterator->thickness+=pre_iterator->thickness;
						line_iterator->head.x=pre_iterator->length>line_iterator->length?pre_iterator->head.x:line_iterator->head.x;
						line_iterator->tail.x=pre_iterator->length>line_iterator->length?pre_iterator->tail.x:line_iterator->tail.x;
						line_iterator->length=pre_iterator->length>line_iterator->length?pre_iterator->length:line_iterator->length;
						hasMatched=true;
						flag++;
					}

				++line_iterator;

			}
				if(!hasMatched)
				pre_iterator->isTerminated=true;
				++pre_iterator;

		}

			//test if it is a valid line
			for(int k=0;k<preList.size();k++)
			{
				segment s=(segment)preList.at(k);
				if(s.isTerminated)
				{
					if(s.length<50)
					{
					if(s.thickness>=4)
					{
						lines.push_back(s);
					}
					}
					else 
					{
						if(s.thickness>=2)
							lines.push_back(s);
					}
				}
			}

			
			preList=lineList;

		}

		//clear
		preList.clear();
		lineList.clear();

		int num_horizontal=lines.size();

			//detect vertical line
		for(int i=0; i<src->width; i++)
		{
			if(i!=0)
				lineList.clear();
			bool hashead=false;
			struct segment sg;
			for(int j=0; j<src->height; j++)
			{
				if((int)*element(src,i,j)<128&&hashead==false)
				{						
					sg.head.x=i;
					sg.head.y=j;
					sg.thickness=1;
					sg.isTerminated=false;
					hashead=true;


				}
				if((int)*element(src,i,j)>128&&hashead==true)
				{

					sg.tail.x=i;
					sg.tail.y=j-1;
					sg.length=sg.tail.y-sg.head.y+1;
					hashead=false;	
					if(sg.length>=LINE_LENGTH_SMALLEST)
						lineList.push_back(sg);
				}

			}
			

			//comparison
			int flag=0;
			pre_iterator=preList.begin();
			while( pre_iterator != preList.end() ) {
				boolean hasMatched=false;
				line_iterator=lineList.begin()+flag;
				while( line_iterator != lineList.end()&&hasMatched==false ) {
					if(/*abs(pre_iterator->length-line_iterator->length)<3&&abs(pre_iterator->head.x-line_iterator->head.x)<2*/
						(pre_iterator->tail.y>line_iterator->head.y)&&(0.8<(float)pre_iterator->length/line_iterator->length<1.2)
						||((pre_iterator->head.y==line_iterator->head.y)||(pre_iterator->tail.y==line_iterator->tail.y))&&
						((float)pre_iterator->length/line_iterator->length>0.5))
					{
						line_iterator->thickness+=pre_iterator->thickness;
						line_iterator->head.y=pre_iterator->length>line_iterator->length?pre_iterator->head.y:line_iterator->head.y;
						line_iterator->tail.y=pre_iterator->length>line_iterator->length?pre_iterator->tail.y:line_iterator->tail.y;
						line_iterator->length=pre_iterator->length>line_iterator->length?pre_iterator->length:line_iterator->length;
						hasMatched=true;
						flag++;
					}

				++line_iterator;

			}
				if(!hasMatched)
				pre_iterator->isTerminated=true;
				++pre_iterator;

		}

			//test if it is a valid line
			for(int k=0;k<preList.size();k++)
			{
				segment s=(segment)preList.at(k);
				if(s.isTerminated)
				{
					if(s.length<50)
					{
					if(s.thickness>=4)
					{
						lines.push_back(s);
					}
					}
					else 
					{
						if(s.thickness>=2)
							lines.push_back(s);
					}
				}
			}

			
			preList=lineList;

		}

   // //delete duplicate  horizontal lines
   //       std::vector<segment>::iterator it = lines.begin();
		 // std::vector<segment>::iterator the = lines.begin();
		 // for(;it!=lines.begin()+num_horizontal;it++)
		 // {
			//  for(;the!=lines.begin()+num_horizontal;the++)
		 // {
			//  if(abs(it->head.y-the->head.y)<8)
			//  {
			//	  if(it->length>the->length)the->del=true;
			//	  else it->del=true;
			//  }

			//}
			//}


		 //  //delete duplicate  vertical lines
		 // for(it=lines.begin()+num_horizontal;it!=lines.begin();it++)
		 // {
			//  for(;the!=lines.begin()+num_horizontal;the++)
		 // {
			//  if(abs(it->head.y-the->head.y)<8)
			//  {
			//	  if(it->length>the->length)the->del=true;
			//	  else it->del=true;
			//  }

			//}
			//}
    
	for(int i=0;i<lines.size();i++)
	{
		segment line=(segment)lines.at(i);
		cvLine( dst, line.head, line.tail, CV_RGB(255,0,0), 1, CV_AA, 0 );
		cout<<line.head.x<<","<<line.head.y<<"."<<line.tail.x<<","<<line.tail.y<<endl;
	}

	 cvSaveImage("3.jpg",dst);
	cvNamedWindow( "src", 1 );
	cvShowImage( "src", src );

	cvNamedWindow( "dst", 1 );
	cvShowImage( "dst", dst );


}
cvWaitKey(0);
return 0;
}
