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

