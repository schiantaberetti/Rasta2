#include "template_extractor.h"
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "stdio.h"


IplImage* getCentredROI(const IplImage *img,int width,int height,CvPoint *offset)
/*Crop the center of the image of dimension width*height. */
{
	IplImage *cropped = NULL;
	int x,y;
	if ((img->width)>width)
		x=((img->width)-width)/2;
	else
	{
		x=0;
		width = (img->width);
	}
	if ((img->height)>height)
		y=((img->height)-height)/2;
	else
	{
		y=0;
		height=(img->height);
	}
	
	
	cropped = img_crop(img,cvRect(x,y,width,height));
	
	offset->x=x;
	offset->y=y;
	return cropped;
}
IplImage* cleanUpRedComponent(IplImage* img)
{
	IplImage* bn_redMask = NULL;
	IplImage* bn_image = NULL;
	IplImage* bn_dark_pixels= NULL;
	IplImage* dark_pixels= NULL;
	IplImage* filtered = NULL;
	
	bn_redMask = getABPixelsMap(img,LAB_A_THRES,LAB_B_THRES); //mask of the red pixels
	
	/*OTSU Thresholding for b/w image*/
	bn_dark_pixels = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	bn_image = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	cvCvtColor(img,bn_image,CV_BGR2GRAY);
	cvThreshold(bn_image,bn_dark_pixels,90,255,CV_THRESH_OTSU);

	/*Get back to color image*/
	dark_pixels = cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
	cvCvtColor(bn_dark_pixels,dark_pixels,CV_GRAY2BGR);
	
	/*Remove the red pixels*/
	filtered=remove_mask(bn_dark_pixels,bn_redMask);
	
	
	cvReleaseImage(&bn_image);
	cvReleaseImage(&dark_pixels);
	cvReleaseImage(&bn_dark_pixels);
	cvReleaseImage(&bn_redMask);
	return filtered;
}

IplImage* getCircledTemplate(const IplImage* img)
{
	IplImage* bn_redMask = NULL;
	IplImage* bn_filtered = NULL;
	IplImage* bn_pattern = NULL;
	IplImage* pattern= NULL;
	
	CvPoint br,tl;

	bn_redMask = getABPixelsMap(img,LAB_A_THRES,LAB_B_THRES);//getHuePixelsMap(img,HSV_HUE_RED,cvGetSize(img),HUE_EPS,SAT_THRES);//get the gray map of red pixels

	show_scaled_image_and_stop(bn_redMask,600,400);
	bn_get_containing_box_coordinates(bn_redMask, &tl,&br);//get the ROI of the red pixels
	
	bn_filtered=get_black_pixels_without_mask(img,bn_redMask);//Obtain the image "cleaned"

	bn_pattern = img_crop(bn_filtered,cvRect(tl.x,tl.y,br.x-tl.x,br.y-tl.y));//Extract the target pattern from the image
	
	bn_closure(bn_pattern,1);//Closure to help in reconstruct the pattern
	
	pattern=cvCreateImage(cvGetSize(bn_pattern),bn_pattern->depth,3);
	cvCvtColor( bn_pattern,pattern, CV_GRAY2BGR);//Convert the pattern to BGR in order to make confrontations
	
	cvReleaseImage(&bn_filtered);

	cvReleaseImage(&bn_redMask);
	cvReleaseImage(&bn_pattern);

	return pattern;
}
void getRedAreaCoords(const IplImage* img,CvPoint *tl,CvPoint *br)
{
	IplImage* bn_redMask = NULL;

	bn_redMask = getABPixelsMap(img,LAB_A_THRES,LAB_B_THRES);//getHuePixelsMap(img,HSV_HUE_RED,cvGetSize(img),HUE_EPS,SAT_THRES);//get the gray map of red pixels

	//show_scaled_image_and_stop(bn_redMask,600,400);
	bn_get_containing_box_coordinates(bn_redMask, tl,br);//get the ROI of the red pixels
	
	

	cvReleaseImage(&bn_redMask);

}


IplImage* pattern_matching(const IplImage* img,const IplImage* pattern)
/*Return the probability map of the pattern in the img*/
{
	CvSize input_size= cvGetSize(img);
	CvSize pattern_size= cvGetSize(pattern);
	IplImage*  probability_map=cvCreateImage(cvSize(input_size.width-pattern_size.width+1,input_size.height-pattern_size.height+1),IPL_DEPTH_32F,1);
	cvMatchTemplate(img,pattern,probability_map,CV_TM_CCOEFF_NORMED);
	return probability_map;
}
void bn_closure(IplImage* img,int n)
{
	bn_reverse(img);
	cvDilate(img,img,NULL,n);
	cvErode(img,img,NULL,n);
	bn_reverse(img);
}
IplImage* img_crop(IplImage* img,CvRect rect)
/*Return the cropped image i.e. the portion of image inside rect*/
{
	cvSetImageROI(img,rect);
	IplImage* patt = cvCreateImage(cvGetSize(img),
                               img->depth,
                               img->nChannels);
    cvCopyImage(img,patt);
    cvResetImageROI(img);
    return patt;
}
IplImage* getDarkerPixelsMap(const IplImage* img,int threshold)
{
	IplImage* elabMask = NULL;
	IplImage* bitMask = NULL;
	int i,j;
	CvScalar pixel;

	bitMask = cvCreateImage( cvGetSize(img),IPL_DEPTH_8U,1);
	elabMask =  cvCreateImage( cvGetSize(img),img->depth,img->nChannels);
	cvCopyImage(img,elabMask);
	cvCvtColor( elabMask, elabMask, CV_BGR2HSV );

	for(i=0;i<elabMask->height;i++)
		for(j=0;j<elabMask->width;j++)
		{
			pixel = cvGet2D(elabMask,i,j);
			if(pixel.val[2]>threshold)
				cvSet2D(bitMask,i,j,cvScalar(255,0,0,0));
		}
	cvReleaseImage(&elabMask);
	return bitMask;
}
void bn_reverse(IplImage* img)
{
	int i,j;
	CvScalar pixel;
	for(i=0;i<img->height;i++)
		for(j=0;j<img->width;j++)
		{
			pixel=(cvGet2D(img,i,j));
			pixel.val[0]=255-pixel.val[0];
			cvSet2D(img,i,j,pixel);
		}
}
IplImage* remove_mask(const IplImage* img,const IplImage* mask)
/*Return the image computed deleting the mask component     */
{
	IplImage* filtered=cvCreateImage( cvGetSize(img),img->depth,img->nChannels);
	cvCopyImage(img,filtered);

	CvScalar pixel;
	int i,j; 
	unsigned char *data=((uchar*)filtered->imageData); 
	int step=filtered->widthStep; 
	for(i=0;i<filtered->height;i++, 
		data+=step)  
		for(j=0;j<(filtered->width) * (filtered->nChannels); 
			pixel=cvGet2D(mask,i,j/filtered->nChannels),j+=filtered->nChannels) 
		if(pixel.val[0]>150)
				{
					data[j+2]=255;
					data[j+1]=255;
					data[j]=255;
				}

	return filtered;

}
IplImage* get_black_pixels_without_mask(const IplImage* img,const IplImage* mask)
/*Return a gray image computed deleting the mask component     */
{
	IplImage* filtered=getDarkerPixelsMap(img,120);
	
	int i,j;
	CvScalar pixel;
	unsigned char *data=((uchar*)filtered->imageData);
	int step=filtered->widthStep;
	for(i=0;i<filtered->height;i++)
	{
			for(j=0;j<(filtered->width) * (filtered->nChannels);j+=filtered->nChannels)
			{
				pixel=cvGet2D(mask,i,j/filtered->nChannels);
				

				if(pixel.val[0]>150)
				{
					//data[j+2]=255;
					//data[j+1]=255;
					data[j]=255;
				}
			}
		data+=step;
	}
	return filtered;
	

}
void std_show_image(const IplImage*img,char* name,int width,int height)
{
	IplImage* destination = cvCreateImage( cvSize(width , height),img->depth, img->nChannels );
	cvResize(img,destination,CV_INTER_NN);
	cvNamedWindow(name, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(name,400,10);
	cvShowImage(name,destination);

	cvReleaseImage(&destination);
}
void show_scaled_image_and_stop(const IplImage*img,int width,int height)
/*Simply show an image and stop until key pressing. Useful for debugging.*/
{
	IplImage* destination = cvCreateImage( cvSize(width , height),img->depth, img->nChannels );
	cvResize(img,destination,CV_INTER_NN);
	cvNamedWindow("show_scaled_image_and_stop", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("show_scaled_image_and_stop",400,10);
	cvShowImage("show_scaled_image_and_stop",destination);
	cvWaitKey(0);
	cvDestroyWindow("show_scaled_image_and_stop");
	cvReleaseImage(&destination);
}
void bn_get_containing_box_coordinates(const IplImage* gray_img,CvPoint* topLeft,CvPoint* bottomRight)
/*Given a gray image (img) overwrite the topleft and the bottom right corner of the white pixels. *\
\* I.e. get the describing points of the minimum rectangle containing all the white pixels.       */
{
	int i,j;
	CvScalar pixel;
	(*bottomRight)= cvPoint(0,0);
	(*topLeft)=cvPoint(gray_img->width,gray_img->height);
	for(i=0;i<gray_img->height;i++)
			for(j=0;j<gray_img->width;j++)
			{
				if(((uchar*)gray_img->imageData)[i*(gray_img->widthStep / sizeof(uchar))+j] >=200 )
				{
					//topLeft part
					if(j<topLeft->x)
						topLeft->x=j;
					if(i<topLeft->y)
						topLeft->y=i;
					//bottomRight part
					if(j>bottomRight->x)
						bottomRight->x=j;
					if(i>bottomRight->y)
						bottomRight->y=i;
					
				}
				
			}
	
}
IplImage* getABPixelsMap(const IplImage* img,int a_thres,int b_thres)
/*Given an arbitrary image (img) this function returns a mask of pixel for the a,b coordinates
 * in the Lab color space          
\* Typical values for a and b are (140,100).                                    */
{
	IplImage* outImg = NULL;
	IplImage* elabMask = NULL;
	CvScalar pixel;
	
	outImg =  cvCreateImage( cvGetSize(img),IPL_DEPTH_8U,1);
	elabMask =  cvCreateImage( cvGetSize(img),img->depth,img->nChannels);
	cvCopyImage(img,elabMask);
	
	cvCvtColor( elabMask, elabMask, CV_BGR2Lab );
	//show_scaled_image_and_stop(elabMask,600,400);
	int i,j;
	for(i=0;i<elabMask->height;i++)
			for(j=0;j<elabMask->width;j++)
			{
				pixel = cvGet2D(elabMask,i,j);
				if(pixel.val[1]>a_thres && pixel.val[2] > b_thres)
					((uchar*)outImg->imageData)[i*(outImg->widthStep / sizeof(uchar))+j] = 255;
				else
					((uchar*)outImg->imageData)[i*(outImg->widthStep / sizeof(uchar))+j] = 0;
			}
	cvSmooth(outImg,outImg,CV_MEDIAN,9,0,0,0);
	
	cvReleaseImage(&elabMask);
	return outImg;
}	
IplImage* getHuePixelsMap(const IplImage* img,int hue,CvSize output_size,int eps,int sat)
/*Given an arbitrary image (img) this function returns a mask of pixel of hue=hue *\
 (according to the parameters of eps and sat) of dimension "output_size".             
\* Typical values for eps and sat are (4,100).                                    */
{
	IplImage* elabMask = NULL;
	IplImage* bitMask = NULL;
	IplImage* outImg = NULL;

	outImg =  cvCreateImage( output_size,IPL_DEPTH_8U,1);
	elabMask =  cvCreateImage( output_size,img->depth,img->nChannels);

	bitMask = cvCreateImage( output_size,IPL_DEPTH_8U,1);
	
	cvResize(img,elabMask,CV_INTER_NN);
	//cvFlip(elabMask,NULL,1);
	cvCvtColor( elabMask, elabMask, CV_BGR2HSV );
		
	
	selectHue(elabMask,bitMask,hue,eps,sat);
	cvSmooth(bitMask,outImg,CV_MEDIAN,9,0,0,0);//Faccio la mediana per eliminare rumore //cvCopy(bitMask,outImg,NULL);

	cvReleaseImage(&bitMask);
	cvReleaseImage(&elabMask);

	return outImg;
}
IplImage* selectHue(const IplImage* elabMask,IplImage* bitMask,int hue,int eps,int sat)
{/*vuole in ingresso una immagine  in HSV e seleziona una tinta con una certa precisione eps*\
   sat è la saturazione minima richiesta al pixel. Nei calcoli si assume che l'hardware effet
 \*tui le somme in registri più grandi di quelli delle variabili.							*/
	CvScalar pixel;
	//eps=3;sat=30;
	int i,j;
	for(i=0;i<elabMask->height;i++)
			for(j=0;j<elabMask->width;j++)
			{
				pixel = cvGet2D(elabMask,i,j);
				//if((pixel.val[0]>hue+eps && pixel.val[0]<((hue+256-eps)%256)) || (pixel.val[1]<sat))
				//if((pixel.val[0]>hue+0 && pixel.val[0]<156) || (pixel.val[1]<30  ))
				//if( pixel.val[0]>155 && pixel.val[0]<200 && pixel.val[1] >30 && pixel.val[1]<80) //<-- best on shit test image
				if((pixel.val[0]>((255+hue-eps)%255) || pixel.val[0]<((hue+eps)%255)) && pixel.val[1]>sat)
					((uchar*)bitMask->imageData)[i*(bitMask->widthStep / sizeof(uchar))+j] = 255;
				else
					((uchar*)bitMask->imageData)[i*(bitMask->widthStep / sizeof(uchar))+j] = 0;
			}
}

