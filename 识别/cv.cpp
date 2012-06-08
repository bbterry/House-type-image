#include <cv.h>
#include <highgui.h>
#include <math.h>

uchar * element(IplImage * img, int x, int y);
 
int main(int argc, char** argv)
{

    IplImage *src = cvLoadImage( "E:\\terry\\2.jpg", 1); 
    IplImage *dst, *tmp;

	if( !src )
        return -1;

    
	dst=cvCreateImage(cvGetSize(src),8,1);
	cvCvtColor( src, dst, CV_BGR2GRAY );
	tmp=cvCreateImage(cvGetSize(dst),8,1);
    
	int patch = 7;
	int threshold = 2500;
	uchar *pElementTmp = 0;
	uchar *pElementDst = 0;
	int itera = 4;

	while( itera > 0)
	{
		cvCopy(dst,tmp,0);
		cvSmooth(dst,tmp,CV_GAUSSIAN,5);
		for(int y=0; y<tmp->height; y++)
		{
			for(int x=0; x<tmp->width; x++)
			{

				pElementTmp = element(tmp,x,y);
				pElementDst = element(dst,x,y);

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

	cvNamedWindow( "src", 1 );
    cvShowImage( "src", src );
 
    cvNamedWindow( "dst", 1 );
    cvShowImage( "dst", dst );

	/*cvNamedWindow( "tmp", 1 );
    cvShowImage( "tmp", tmp );*/
 
    cvWaitKey(0);
 
    return 0;


}

//img为单通道灰度图
uchar * element(IplImage * img, int x, int y)
{
	return (uchar*)(img->imageData+y*img->widthStep+x*img->nChannels);
}