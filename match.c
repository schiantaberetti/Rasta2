/*
  Detects SIFT features in two images and finds matches between them.

  Copyright (C) 2006-2010  Rob Hess <hess@eecs.oregonstate.edu>

  @version 1.1.2-20100521
*/

#include "template_extractor.h"
#include "sift_template.h"
#include "match.h"


int getTextCircledPosition( char* pdf_image_name,char* photo_name,int* tlx,int* tly,int* width,int* height)
/*Returns the position of the circled text (inside photo_name file) in the pdf page represented by pdf_image_name*/
{
	IplImage* original_image,*large_original_image;
	IplImage* retrieved_image;
	IplImage* template,*large_template;
	CvPoint br,tl; //Position of the template in the image
	
	 original_image = cvLoadImage( pdf_image_name, 1 );
  
  	retrieved_image = cvLoadImage( photo_name, 1 );
  
 
	template=getCircledTemplate(retrieved_image);
	
	
	int scale = 1;
	large_template =  cvCreateImage( cvSize((cvGetSize(template).width)*scale,(cvGetSize(template).height)*scale),template->depth,template->nChannels);
	cvResize(template,large_template,CV_INTER_NN);
	large_original_image =  cvCreateImage( cvSize((cvGetSize(original_image).width)*scale,(cvGetSize(original_image).height)*scale),original_image->depth,original_image->nChannels);
	cvResize(original_image,large_original_image,CV_INTER_NN);
	//show_scaled_image_and_stop(large_original_image,600,400);
	
	getTemplatePositionFromImage(large_template,large_original_image,&tl,&br);
	printf("\nTop Left corner: x=%d y=%d\n",tl.x,tl.y);
	printf("Bottom Right corner: x=%d y=%d\n",br.x,br.y);
	cvRectangle(large_original_image,                    // the dest image 
                tl,        // top left point 
                br,       // bottom right point 
                cvScalar(0, 255, 0, 0), // the color; blue 
                10, 8, 0);               // thickness, line type, shift
	
	std_show_image(large_original_image,"original",400,600);

	cvWaitKey(0);
	*tlx = tl.x;
	*tly = tl.y;
	*width = br.x - tl.x;
	*height = br.y - tl.y;

	cvReleaseImage(&large_original_image);
	cvReleaseImage(&original_image);
	cvReleaseImage(&retrieved_image);
	cvReleaseImage(&template);
	cvReleaseImage(&large_template);
	cvDestroyAllWindows();
	
	return 0;
}

void getImageInfo(char* name,ImageInfo_t* imageInfo){
	ProcessFile(name);
	imageInfo=&ImageInfo;
}
/*
int main( int argc, char** argv )
{
	IplImage* original_image;
	IplImage* retrieved_image;
	IplImage* template;
	CvPoint br,tl; //Position of the template in the image
	
	 original_image = cvLoadImage( argv[1], 1 );
  
  retrieved_image = cvLoadImage( argv[2], 1 );
 
	template=getCircledTemplate(retrieved_image);
	show_scaled_image_and_stop(template,600,400);
	
	getTemplatePositionFromImage(template,original_image,&tl,&br);
	printf("\nTop Left corner: x=%d y=%d\n",tl.x,tl.y);
	printf("Bottom Right corner: x=%d y=%d\n",br.x,br.y);
	cvRectangle(original_image,                    // the dest image 
                tl,        // top left point 
                br,       // bottom right point 
                cvScalar(0, 255, 0, 0), // the color; blue 
                10, 8, 0);               // thickness, line type, shift
	
	std_show_image(original_image,"original",400,600);

	cvWaitKey(0);


	cvReleaseImage(&original_image);
	cvReleaseImage(&retrieved_image);
	cvReleaseImage(&template);
	cvDestroyAllWindows();
	
	return 0;
}*/

