/*
  Detects SIFT features in two images and finds matches between them.

  Copyright (C) 2006-2010  Rob Hess <hess@eecs.oregonstate.edu>

  @version 1.1.2-20100521
*/

#include "pdftotext.h"
#include "match.h"

int main(int argc, char** argv){	
	if(argc!=9){
		printf("Error: usage <pdf_name> <text_file_name> <starting_page> <ending_page> <pdf_x> <pdf_y> <pdf_width> <pdf_height>\n");
		return 0;
	}
	int tlx,tly,width,height;
	//extractTextFromPdf(argv[1],argv[2],atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),atoi(argv[6]),atoi(argv[7]),atoi(argv[8]));
	extractTextFromPdf("paper.pdf","out.text",1,1,0,0,300,400);
	getTextCircledPosition( "cena_smarta.jpg","lm_cena_smarta_vert.jpg", &tlx,&tly,&width,&height);

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

