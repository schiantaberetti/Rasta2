/*
  Detects SIFT features in two images and finds matches between them.

  Copyright (C) 2006-2010  Rob Hess <hess@eecs.oregonstate.edu>

  @version 1.1.2-20100521
*/

#include "pdftotext.h"
#include <iostream>
#include <sstream>
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
	if(argc!=2){
		printf("Error: usage <circled_pdf_image>\n");
		return 0;
	}
	circledImage=argv[1];
	getImageDate(circledImage,time);

	pdfName=findPdfFileInDB(circledImage,&tlx,&tly,&width,&height,&numberOfPage);
	numberOfPage++; //needed to compensate the notation of imagemagick
	/*getTextCircledPosition( originalImage,circledImage, &tlx,&tly,&width,&height);*/
	outFileStringBuilder.append(pdfName);
	outFileStringBuilder.append("_from_page_");
	std::ostringstream ss;
	ss<<numberOfPage;
	outFileStringBuilder.append(ss.str());
	outFileStringBuilder.append("_in_date_");
	outFileStringBuilder.append(time);
	outFileName = new char [outFileStringBuilder.size()+1];
	strcpy(	outFileName, outFileStringBuilder.c_str());
	parseText(outFileName);
	if(strcmp(pdfName,PDF_NOT_FOUND)!=0) extractTextFromPdf(pdfName,outFileName,numberOfPage,numberOfPage,tlx,tly,width,height);
	std::cout<<pdfName<<" "<<numberOfPage<<" "<<tlx<<" "<<tly<<" "<<width<<" "<<height<<std::endl;
	
	if(strcmp(pdfName,PDF_NOT_FOUND)!=0)  free(pdfName);
	return 0;
}

