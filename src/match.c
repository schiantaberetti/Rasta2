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

IplImage *preProcessTestImage(char *image_filename,CvPoint *tl,CvPoint *br)
/*This function returns a cropped cleaned image of image_filename and compute
 * the position (top_left,bottom_right) of the red object in the image.*/
{
	IplImage *input_image,*cleaned_image,*cropped_sample;
	CvPoint offset;
	
	input_image=cvLoadImage( image_filename, 1 );

	getRedAreaCoords(input_image,tl,br);
	
	cleaned_image=cleanUpRedComponent(input_image);
	
	cropped_sample = getCentredROI(cleaned_image,CROP_DIM,CROP_DIM,&offset);
	
	(tl->x)=(tl->x)-offset.x;
	(tl->y)=(tl->y)-offset.y;
	(br->x)=(br->x)-offset.x;
	(br->y)=(br->y)-offset.y;
	
	cvReleaseImage(&input_image);
	cvReleaseImage(&cleaned_image);
	return cropped_sample;
}

void saveSiftToFile(IplImage* image,char *filename)
{
	int nFeat;
	struct feature *feat_template;
	
	nFeat=sift_features( image, &feat_template );
	export_features( filename, feat_template, nFeat );
	free(feat_template);
}

char *getBestMatchInDB(char *test_siftFilename)
/*Compare sifts in test_siftFilename with the sifts in the files of the DB and
 * returns the name of the sift file that best matchs.*/
{
	char *targetSiftFile = NULL;
	int n_best=0;
	int matches;
	
	sqlite3 *db;
	sqlite3_stmt* stmt;
	struct SiftFileData *siftMetaData=NULL;
	
	openDB(DB_PATH,&db);	
	
	queryDB("SELECT * FROM sifts",&stmt,&db);
	
	while(fetchSiftQuery(&stmt,&siftMetaData)){
		//printf("SiftData MetaInfo:\nname: %s\npath: %s\nid_sift: %d\nid_pages: %d\n",siftMetaData->name,siftMetaData->path,siftMetaData->id_sift,siftMetaData->id_pages);
		matches=getSiftMatches(test_siftFilename,siftMetaData->uri);
		printf("File %s, trovati %d matches.\n",siftMetaData->name,matches);
		if(matches>n_best)
		{
			n_best=matches;
			dynStringAssignement(&targetSiftFile,siftMetaData->uri);
		}
		
		destroySiftFileData(siftMetaData);//printf("I'm here!\n");
		siftMetaData=NULL;
	}

	closeDB(&db);
	return targetSiftFile;
}
void getSiftPdfCoords(char* db_siftFilename,char **pdfFilename,int *page_number)
/*Return the pdf and the page number to which the sift is related*/
{
	char *query=NULL;
	sqlite3 *db;
	sqlite3_stmt* stmt;
	
	openDB(DB_PATH,&db);
	db_siftFilename=chainString(db_siftFilename,"'");
	query=chainString("select pages.number_of_page,papers.name,papers.path from papers,pages,sifts where pages.id_paper=papers.id_paper and pages.id_pages=sifts.id_pages and sifts.name='",db_siftFilename);	
	queryDB(query,&stmt,&db);
	fetchPDFInfo(&stmt,pdfFilename,page_number);

	free(query);
	free(db_siftFilename);
	closeDB(&db);
}
void showResult(const char *siftFileName,const CvPoint *tl,const CvPoint *br)
{
	char *image_uri=NULL;
	char *query=NULL;
	IplImage *pdf_page;
	sqlite3 *db;
	sqlite3_stmt* stmt;
	
	openDB(DB_PATH,&db);	
	siftFileName=chainString(siftFileName,"'");
	query=chainString("select pages.name,pages.path from pages,sifts where pages.id_pages=sifts.id_pages and sifts.name='",siftFileName);
	//printf("Query: %s",query);
	queryDB(query,&stmt,&db);
	fetchImageURI(&stmt,&image_uri);

	closeDB(&db);
	
	printf("Top Left corner: x=%d y=%d\n",tl->x,tl->y);
	printf("Bottom Right corner: x=%d y=%d\n",br->x,br->y);
	
	pdf_page=cvLoadImage(image_uri,1);
	cvRectangle(pdf_page,                    // the dest image 
                *tl,        // top left point 
                *br,       // bottom right point 
                cvScalar(0, 255, 0, 0), // the color; blue 
                10, 8, 0);               // thickness, line type, shift
	
	std_show_image(pdf_page,"original",400,600);

	cvWaitKey(0);

	cvReleaseImage(&pdf_page);
	free(query);
	free(siftFileName);
	free(image_uri);
}
char* findPdfFileInDB(char* test_image,int* tlx,int* tly,int* width,int* height,int *page_number)
/*Returns the name of the most probable pdf from wich the photo test_image has been taken and
 * set the page_number pointer to the right page in the pdf and
 * set the coords and dimensions wrt the red circled area in the test_image.*/
{
	CvPoint br,tl;
	IplImage *input_image;
	CvMat *transformation_matrix;
	char *test_siftFilename = "/tmp/testSiftFile";
	char *db_siftFilename=NULL;
	char *pdfFilename=NULL;
	
	input_image=preProcessTestImage(test_image,&tl,&br);
	
	printf("Saving SIFT to file...\n");
	saveSiftToFile(input_image,test_siftFilename);
	
	printf("Beginning research in DB.\n");
	db_siftFilename=getBestMatchInDB(test_siftFilename);
	printf("Winner is: %s.\n",db_siftFilename);

	getSiftPdfCoords(basename(db_siftFilename),&pdfFilename,page_number);
	printf("pdfFilename: %s\nPage number: %d.\n",pdfFilename,*page_number);
	
	int useless;
	transformation_matrix=getProjectionAndMatchText(test_siftFilename,db_siftFilename,&useless);
	perspectiveTrasformation(transformation_matrix,&br);
	perspectiveTrasformation(transformation_matrix,&tl);

	*tlx = tl.x;
	*tly = tl.y;
	*width = br.x - tl.x;
	*height = br.y - tl.y;
	
	showResult(basename(db_siftFilename),&tl,&br);
	
	cvReleaseMat(&transformation_matrix);
	cvReleaseImage(&input_image);
	free(db_siftFilename);
	//free(pdfFilename);
	return pdfFilename;
}

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

char* findPdfFileInDB_old(char* test_image,int* tlx,int* tly,int* width,int* height,int *page_number)
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
	//show_scaled_image_and_stop(cleaned_image,800,600);
	
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
