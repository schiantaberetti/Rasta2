/*
  Detects SIFT features in two images and finds matches between them.

  Copyright (C) 2006-2010  Rob Hess <hess@eecs.oregonstate.edu>

  @version 1.1.2-20100521
*/

#include "sift_template.h"


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
	
  getCornerFromTemplate(img1,img2,&tl,&br);
  printf("\nTopLeft %d %d",tl.x,tl.y);
  printf("\nBottomRight %d %d \n",br.x,br.y);
  return 0;
}


