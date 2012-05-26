/*
  Detects SIFT features in two images and finds matches between them.

  Copyright (C) 2006-2010  Rob Hess <hess@eecs.oregonstate.edu>

  @version 1.1.2-20100521
*/

#include "template_extractor.h"
#include "sift_template.h"
#include "match.h"

#define CROP_DIM	800
#define DB_PDF_IMG_PATH "database/pdf_img/"

int getTextCircledPosition( char* pdf_image_name,char* photo_name,int* tlx,int* tly,int* width,int* height)
/*Returns the position of the circled text (inside photo_name file) in the pdf page represented by pdf_image_name*/
{
	IplImage* original_image;
	IplImage* retrieved_image, *projection;
	IplImage *cropped_sample = NULL;
	IplImage *cleaned_image = NULL;
//	IplImage *template;
	CvMat* transformation_matrix;
	CvPoint br,tl; //Position of the template in the image
	
	 original_image = cvLoadImage( pdf_image_name, 1 );
  
  	retrieved_image = cvLoadImage( photo_name, 1 );
  
 
	//template=getCircledTemplate(retrieved_image);
	getRedAreaCoords(retrieved_image,&tl,&br);
	printf("\nTop Left corner: x=%d y=%d\n",tl.x,tl.y);
	printf("Bottom Right corner: x=%d y=%d\n",br.x,br.y);
	cleaned_image=cleanUpRedComponent(retrieved_image);
	show_scaled_image_and_stop(cleaned_image,800,600);
	
	CvPoint offset;
	cropped_sample = getCentredROI(cleaned_image,CROP_DIM,CROP_DIM,&offset);
	tl.x=tl.x-offset.x;
	tl.y=tl.y-offset.y;
	br.x=br.x-offset.x;
	br.y=br.y-offset.y;
	
	bestFitForTemplate(cropped_sample,DB_PDF_IMG_PATH);
	//ATTENTION	
	//If the number of match is not enough the matrix is null
	transformation_matrix=getProjection(cropped_sample,original_image);
	perspectiveTrasformation(transformation_matrix,&br);
	perspectiveTrasformation(transformation_matrix,&tl);
	
	//OLD
	//projection=getTemplateProjection(retrieved_image,original_image);
	//getRedAreaCoords(projection,&tl,&br);
	
	//VERY OLD
	//getTemplatePositionFromImage(retrieved_image,original_image,&tl,&br);
	

	printf("\nTop Left corner: x=%d y=%d\n",tl.x,tl.y);
	printf("Bottom Right corner: x=%d y=%d\n",br.x,br.y);
	cvRectangle(original_image,                    // the dest image 
                tl,        // top left point 
                br,       // bottom right point 
                cvScalar(0, 255, 0, 0), // the color; blue 
                10, 8, 0);               // thickness, line type, shift
	
	std_show_image(original_image,"original",400,600);

	cvWaitKey(0);

	*tlx = tl.x;
	*tly = tl.y;
	*width = br.x - tl.x;
	*height = br.y - tl.y;
	printf("\nOKI");
	cvReleaseImage(&cleaned_image);
	//cvReleaseImage(&projection);
	cvReleaseImage(&cropped_sample);
	cvReleaseMat(&transformation_matrix);
	cvReleaseImage(&original_image);
	cvReleaseImage(&retrieved_image);
	//cvReleaseImage(&template);
	cvDestroyAllWindows();
	
	return 0;
}

void getImageDate(char* name,char* time){
/*Returns the date of the image specified by name formatted in YYYY_MM_DD_HH_MM_SS*/
	ImageInfo_t *imageInfo;
	ProcessFile(name,&imageInfo);
        FileTimeAsString(time,imageInfo);
	parseText(time);
	free(imageInfo);
}

void parseText(char* text){
/*Parse a char array and substitutes some annoying character like /,:,  with _*/
	int i=0;
	int length;
	length=strlen(text);
	for(i=0;i<length;i++){
		if(text[i]==':' || text[i]==' '|| text[i]=='/')
			text[i]='_';
	}

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

