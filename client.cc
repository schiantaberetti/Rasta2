/*
  Detects SIFT features in two images and finds matches between them.

  Copyright (C) 2006-2010  Rob Hess <hess@eecs.oregonstate.edu>

  @version 1.1.2-20100521
*/

#include "pdftotext.h"
//#include <stdlib.h>
//#include <stdio.h>
#include <iostream>
extern "C" { //cplusplus method to import c compiled code.
#include "match.h" 
}

int main(int argc, char** argv){	
	//variables tha contains the top left corner and the dimension of the red cirle
	int tlx,tly,width,height;
	//the number of the page in the pdf document
	int numberOfPage;
	//the name of the pdf
	char* pdfName;
	//the name of the red circled image
	char* circledImage;
	//the name of the original pdf page image 
	char* originalImage;
	//time of the photo
	char time[20];
	//the name of the output file
	char* outFileName;
	//the string to build the output file name
	std::string outFileStringBuilder("note_from_");
	//TODO 
	if(argc!=5){
		printf("Error: usage <pdf> <number_of_page> <pdf_page_img> <circled_pdf_image>\n");
		return 0;
	}
	pdfName=argv[1];
	numberOfPage=atoi(argv[2]);
	originalImage=argv[3];
	circledImage=argv[4];
	getImageDate(circledImage,time);
	getTextCircledPosition( originalImage,circledImage, &tlx,&tly,&width,&height);

	outFileStringBuilder.append(pdfName);
	outFileStringBuilder.append("_from_page_");
	outFileStringBuilder.append(argv[2]);
	outFileStringBuilder.append("_in_date_");
	outFileStringBuilder.append(time);
	outFileName = new char [outFileStringBuilder.size()+1];
	strcpy(	outFileName, outFileStringBuilder.c_str());
	parseText(outFileName);
	extractTextFromPdf(pdfName,outFileName,numberOfPage,numberOfPage,tlx,tly,width,height);
	

	return 0;
}
/*
int test_template( int argc, char** argv )
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
}
/*
int main( int argc, char** argv )
{
  IplImage* img1, * img2;
  CvPoint br,tl;
  if( argc != 3 )
    fatal_error( "usage: %s <img1> <img2>", argv[0] );
  
  img1 = cvLoadImage( argv[1], 1 );
  if( ! img1 )
    fatal_error( "unable to load image from %s", argv[1] );
  img2 = cvLoadImage( argv[2], 1 );
  if( ! img2 )
    fatal_error( "unable to load image from %s", argv[2] );
	
  getTemplatePositionFromImage(img1,img2,&tl,&br);
  printf("\nTopLeft %d %d",tl.x,tl.y);
  printf("\nBottomRight %d %d \n",br.x,br.y);
  return 0;
}
*/

