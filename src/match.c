/*
  Detects SIFT features in two images and finds matches between them.

  Copyright (C) 2006-2010  Rob Hess <hess@eecs.oregonstate.edu>

  @version 1.1.2-20100521
*/

#include "template_extractor.h"
#include "sift_template.h"
#include "match.h"
#include "sift.h"
#include "database.h"
#include "sqlite3.h"
#include <stdio.h>

int getTextCircledPosition( char* pdf_image_name,char* photo_name,int* tlx,int* tly,int* width,int* height)
/*Returns the position of the circled text (inside photo_name file) in the pdf page represented by pdf_image_name*/
{

	IplImage* original_image;
	IplImage* retrieved_image, *projection;
	IplImage *cropped_sample = NULL;
	IplImage *cleaned_image = NULL;
	CvMat* transformation_matrix;
	CvPoint br,tl; //Position of the template in the image
	
	 original_image = cvLoadImage( pdf_image_name, 1 );
  
  	retrieved_image = cvLoadImage( photo_name, 1 );
  
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
	
	//ATTENTION	
	//If the number of match is not enough the matrix is null
	transformation_matrix=getProjection(cropped_sample,original_image);
	perspectiveTrasformation(transformation_matrix,&br);
	perspectiveTrasformation(transformation_matrix,&tl);

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

	cvReleaseImage(&cleaned_image);
	cvReleaseImage(&cropped_sample);
	cvReleaseMat(&transformation_matrix);
	cvReleaseImage(&original_image);
	cvReleaseImage(&retrieved_image);
	cvDestroyAllWindows();
	
	return 0;
}

char* findPdfFileInDB(char* test_image,int* tlx,int* tly,int* width,int* height,int *page_number)
/*Returns the name of the most probable pdf from wich the photo test_image has been taken and
 * set the page_number pointer to the right page in the pdf and
 * set the coords and dimensions wrt the red circled area in the test_image.*/
{

	//BRUTTURA
	char* tmpSiftTemplateFile="temp.sift";
	struct feature* feat_template=NULL,*tmp_feat=NULL;

	CvMat* transformation_matrix,*tmpMatrix=NULL;
	IplImage* retrieved_image, *projection=NULL;
	IplImage *cropped_sample = NULL;
	IplImage *cleaned_image = NULL;
	int bestMatch=0,tmpMatch,nFeat;
	CvPoint br,tl; //Position of the template in the image
 	char* nameOfImage,*nameOfSift,*outputName=NULL;
	//DEBUG
	char* nameSrcImage=NULL;
	IplImage *srcImage = NULL;
	//DEBUG
	sqlite3 *db;
	sqlite3_stmt* stmt;
	struct DBInfo *dbInfo;
	dbInfo=(struct DBinfo *)malloc(sizeof(struct DBInfo));
	char* query="SELECT distinct p1.name,p2.name,p1.path,p2.path,p2.number_of_page,p3.name,p3.path from papers p1,pages p2 ,sifts p3 where p1.id_paper=p2.id_paper and p3.id_pages=p2.id_pages";
	openDB(DB_PATH,&db);	
	queryDB(query,&stmt,&db);
  
  	retrieved_image = cvLoadImage( test_image, 1 );
  
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
	
	//BRUTTURA
	printf("\nCalcolo i sift del template e li salvo su file");
	nFeat=sift_features( cropped_sample, &feat_template );
	export_features( tmpSiftTemplateFile, feat_template, nFeat );
	free(feat_template);

	while(fetchQuery(&stmt,&dbInfo)){
		nameOfImage=(char*)malloc(sizeof(char)*(strlen(dbInfo->pageName)+strlen(dbInfo->pagePath)+1));
		strcpy(nameOfImage,dbInfo->pagePath);
		strcat(nameOfImage,dbInfo->pageName);


		nameOfSift=(char*)malloc(sizeof(char)*(strlen(dbInfo->siftName)+strlen(dbInfo->siftPath)+1));
		strcpy(nameOfSift,dbInfo->siftPath);
		strcat(nameOfSift,dbInfo->siftName);

		//ATTENTION	
		//If the number of match is not enough the matrix is null
		//VERSION THAT CALCULATE SIFT FOR IMAGE & TEMPLATE
		//tmpMatrix=getProjectionAndMatch(cropped_sample,nameOfImage,&tmpMatch);
		//tmp_feat=clone_feature(feat_template);

		//VERSION THAT CALCULATE SIFT FOR ONLY IMAGE (IT WORKS CORRECT )
		//nFeat=sift_features( cropped_sample, &feat_template );		
		//tmpMatrix=getProjectionAndMatchFeatures(feat_template,nFeat,nameOfSift,&tmpMatch);

		//BRUTTURA
		tmpMatrix=getProjectionAndMatchText(tmpSiftTemplateFile,nameOfSift,&tmpMatch);

		printf("\nMatch trovati con %s: %d\n",dbInfo->pageName,tmpMatch);
		if(tmpMatch>bestMatch){
			bestMatch=tmpMatch;			
			if(tmpMatrix!=NULL) transformation_matrix=cvCloneMat(tmpMatrix);
			*page_number=dbInfo->numberOfPage;
			if(outputName!=NULL) free(outputName);					
			outputName=(char*)malloc(sizeof(char)*(strlen(dbInfo->paperPath)+strlen(dbInfo->paperName)+1));
			strcpy(outputName,dbInfo->paperPath);
			strcat(outputName,dbInfo->paperName);
			//DEBUG
			if(outputName!=NULL) free(nameSrcImage);
			nameSrcImage=(char*)malloc(sizeof(char)*(strlen(nameOfImage)+1));
			strcpy(nameSrcImage,nameOfImage);
			//DEBUG
		}
	}
	printf("Le info sono: %s %d\n",outputName,*page_number);


	perspectiveTrasformation(transformation_matrix,&br);
	perspectiveTrasformation(transformation_matrix,&tl);

	//DEBUG
	srcImage = cvLoadImage( nameSrcImage, 1 );

	printf("\nTop Left corner: x=%d y=%d\n",tl.x,tl.y);
	printf("Bottom Right corner: x=%d y=%d\n",br.x,br.y);
	cvRectangle(srcImage,                    // the dest image 
                tl,        // top left point 
                br,       // bottom right point 
                cvScalar(0, 255, 0, 0), // the color; blue 
                10, 8, 0);               // thickness, line type, shift
	
	std_show_image(srcImage,"original",400,600);

	cvWaitKey(0);

	//DEGUB

	*tlx = tl.x;
	*tly = tl.y;
	*width = br.x - tl.x;
	*height = br.y - tl.y;

	cvReleaseImage(&cleaned_image);
	cvReleaseImage(&cropped_sample);
	cvReleaseMat(&transformation_matrix);
	//DEBUG
	if(srcImage!=NULL) cvReleaseImage(&srcImage);
	//DEBUG
	if(tmpMatrix!=NULL) cvReleaseMat(&transformation_matrix);
	//if(feat_template!=NULL) free(feat_template);
	if(tmp_feat!=NULL) free(feat_template);
	cvReleaseImage(&retrieved_image);
	free(dbInfo);
	closeDB(&db);
	cvDestroyAllWindows();

	if(remove(tmpSiftTemplateFile)== -1 )
		printf( "\nErrore nella cancellazione del file" );
	
	return outputName;
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
