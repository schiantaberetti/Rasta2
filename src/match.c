
#include "template_extractor.h"
#include "sift_template.h"
#include "match.h"
#include "sift.h"
#include "database.h"
#include "sqlite3.h"
#include <stdio.h>
#include "list.h"
#include <pthread.h>

struct SiftThread {
	struct list_head list;
	pthread_t thread;

	char *test_sift_file;
	int matches;
	struct SiftFileData *siftMetaData;
};

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

int startSiftMatchJob(struct SiftThread* job)
{
	int matches;
	struct SiftFileData *siftMetaData = job->siftMetaData;
#ifdef DEBUG 
	printf("Starting Sift Job of %s.\n",siftMetaData->name);
#endif	
	matches=getSiftMatches(job->test_sift_file,siftMetaData->uri);
#ifdef DEBUG
	printf("File %s, trovati %d matches.\n",siftMetaData->name,matches);
#endif
	job->matches=matches;
	return matches; 
}
void destroySiftThreadJob(struct SiftThread *job)
{
#ifdef DEBUG 
	printf("freeing test string.\n");
#endif
	free(job->test_sift_file);
#ifdef DEBUG
	printf("freeing sift meta data.\n");
#endif
	destroySiftFileData(job->siftMetaData);
#ifdef DEBUG
	printf("freeing job structure.\n");
#endif
	free(job);
#ifdef DEBUG 	
	printf("done freeing.\n");
#endif
}

struct SiftFileData *siftJobsResultProcessor(struct list_head* thread_list)
{
	struct SiftThread *job;
	struct SiftFileData *db_target_sift=NULL;
	int n_best=0;
	int *matches;
	
	struct list_head* iter,*q;
	list_for_each_safe(iter,q, thread_list){
		job = container_of(iter,struct SiftThread,list);
#ifdef DEBUG 
			printf("Waiting: %s\n",job->siftMetaData->name); 
#endif
		pthread_join( job->thread, (&matches));

		if(matches>n_best)
		{
			n_best=matches;
			if(db_target_sift!=NULL)
				destroySiftFileData(db_target_sift);
			db_target_sift=dynCopy(job->siftMetaData);
		}
		list_del(iter);
		destroySiftThreadJob(job);
	}
	
	return db_target_sift;
}
struct SiftFileData *getSiftData(struct list_head *thread_list,int position)
{
	struct SiftThread *job;
	struct SiftFileData *db_target_sift=NULL;
	int counter=0;
	struct list_head* iter,*q;
	list_for_each_safe(iter,q, thread_list){
		
		if(counter==position)
		{
			job = container_of(iter,struct SiftThread,list);
			db_target_sift=dynCopy(job->siftMetaData);
		}
		counter++;
	}
	return db_target_sift;
}
int hasMoreMatch(struct list_head* l_a,struct list_head * l_b)
{
		struct SiftThread *a,*b;
		a=container_of(l_a,struct SiftThread,list);
		b=container_of(l_b,struct SiftThread,list);
		if((a->matches)<(b->matches))
			return 1;
		if((a->matches)>(b->matches))
			return -1;
		return 0;
	
}
void destroyJobs(struct list_head* thread_list)
{
	struct SiftThread *job;
	
	struct list_head* iter,*q;
	list_for_each_safe(iter,q, thread_list){
		job = container_of(iter,struct SiftThread,list);

		list_del(iter);
		destroySiftThreadJob(job);
	}
}

void jobsProcessor(struct list_head* thread_list)
{	
	struct SiftThread *job;
	int matches;
	
	struct list_head* iter,*q;
	list_for_each_safe(iter,q, thread_list){
		job = container_of(iter,struct SiftThread,list);
#ifdef DEBUG 
			printf("Waiting: %s\n",job->siftMetaData->name); 
#endif
		pthread_join( job->thread, (&matches));
	}

	list_sort(thread_list,hasMoreMatch);

}


struct SiftThread* createSiftJob(char *test_siftFilename,struct SiftFileData *siftMetaData)
{
	struct SiftThread* job;
	job=(struct SiftThread*)malloc(sizeof(struct SiftThread));
	job->test_sift_file=NULL;
	dynStringAssignement(&(job->test_sift_file),test_siftFilename);
	job->siftMetaData=dynCopy(siftMetaData);
	return job;
}

char *getBestMatchInDB(char *test_siftFilename)
/*Compare sifts in test_siftFilename with the sifts in the files of the DB and
 * returns the name of the sift file that best matchs.*/
{
	struct list_head* thread_list=(struct list_head*)malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(thread_list);
	struct SiftThread *job=NULL;

	struct SiftFileData *siftMetaData=NULL; //Iterator
	
	sqlite3 *db;
	sqlite3_stmt* stmt;
	openDB(DB_PATH,&db);	
	queryDB("SELECT * FROM sifts",&stmt,&db);
	
	while(fetchSiftQuery(&stmt,&siftMetaData)){
#ifdef DEBUG
		printf("SiftData MetaInfo:\nname: %s\npath: %s\nid_sift: %d\nid_pages: %d\n",siftMetaData->name,siftMetaData->path,siftMetaData->id_sift,siftMetaData->id_pages);
#endif
		job=createSiftJob(test_siftFilename, siftMetaData); //INIT the job structure
		pthread_create( &(job->thread), NULL, startSiftMatchJob, job);			// Release the job
		list_add_tail(&(job->list),thread_list)		;					// Add the job to the pending jobs list
	}
	closeDB(&db);
	
	siftMetaData=siftJobsResultProcessor(thread_list);
	free(thread_list);
	return siftMetaData;
}
struct list_head* getJobsResults(char *test_siftFilename)
/*Compare sifts in test_siftFilename with the sifts in the files of the DB and
 * returns list of threads with their results.*/
{
	struct list_head* thread_list=(struct list_head*)malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(thread_list);
	struct SiftThread *job=NULL;

	struct SiftFileData *siftMetaData=NULL; //Iterator
	
	sqlite3 *db;
	sqlite3_stmt* stmt;
	openDB(DB_PATH,&db);	
	queryDB("SELECT * FROM sifts",&stmt,&db);
	
	while(fetchSiftQuery(&stmt,&siftMetaData)){
#ifdef DEBUG
		printf("SiftData MetaInfo:\nname: %s\npath: %s\nid_sift: %d\nid_pages: %d\n",siftMetaData->name,siftMetaData->path,siftMetaData->id_sift,siftMetaData->id_pages);
#endif
		job=createSiftJob(test_siftFilename, siftMetaData); //INIT the job structure
		pthread_create( &(job->thread), NULL, startSiftMatchJob, job);			// Release the job
		list_add_tail(&(job->list),thread_list)		;					// Add the job to the pending jobs list
	}
	closeDB(&db);
	jobsProcessor(thread_list);
	//free(thread_list);
	return thread_list;
}


char *getBestMatchInDB_unparallel(char *test_siftFilename)
/*Compare sifts in test_siftFilename with the sifts in the files of the DB and
 * returns the name of the sift file that best matchs.*/
{
	//char *targetSiftFile = NULL;
	int n_best=0;
	int matches;
	
	sqlite3 *db;
	sqlite3_stmt* stmt;
	struct SiftFileData *siftMetaData=NULL;
	struct SiftFileData *db_target_sift=NULL;
	
	openDB(DB_PATH,&db);	
	
	queryDB("SELECT * FROM sifts",&stmt,&db);
	
	while(fetchSiftQuery(&stmt,&siftMetaData)){
		//printf("SiftData MetaInfo:\nname: %s\npath: %s\nid_sift: %d\nid_pages: %d\n",siftMetaData->name,siftMetaData->path,siftMetaData->id_sift,siftMetaData->id_pages);
		matches=getSiftMatches(test_siftFilename,siftMetaData->uri);
		//printf("File %s, trovati %d matches.\n",siftMetaData->name,matches);
		if(matches>n_best)
		{
			n_best=matches;
			if(db_target_sift!=NULL)
				destroySiftFileData(db_target_sift);
			db_target_sift=dynCopy(siftMetaData);
		}
		
		destroySiftFileData(siftMetaData);
		siftMetaData=NULL;
	}

	closeDB(&db);
	return db_target_sift;
}

void logFirstSiftNames(struct list_head *thread_list)
{
	struct SiftThread *job;
	int count=0;
	
	struct list_head* iter,*q;
	list_for_each_safe(iter,q, thread_list){
		job = container_of(iter,struct SiftThread,list);
		logToFile(job->siftMetaData->name);
		if (count==RESULTS_NUMBER-1) break;
		count++;
	}
	
}
void getSiftPdfCoords(struct SiftFileData* db_sift,char **pdfFilename,int *page_number)
/*Return the pdf and the page number to which the sift is related*/
{
	char *query=NULL;
	char *db_siftID=NULL;
	char sift_id[5];
	sqlite3 *db;
	sqlite3_stmt* stmt;
	
	openDB(DB_PATH,&db);
	
	sprintf(sift_id,"%d",db_sift->id_sift);
	//printf("SIFT_ID: %s\n",sift_id);
	db_siftID=chainString(sift_id,"'");
	query=chainString("select pages.number_of_page,papers.name,papers.path from papers,pages,sifts where pages.id_paper=papers.id_paper and pages.id_pages=sifts.id_pages and sifts.id_sift='",db_siftID);	
	queryDB(query,&stmt,&db);
	fetchPDFInfo(&stmt,pdfFilename,page_number);

	free(query);
	free(db_siftID);
	closeDB(&db);
}
void showResult(const struct SiftFileData *target_sift,const CvPoint *tl,const CvPoint *br)
{
	char *image_uri=NULL;
	char *query=NULL;
	char *siftID=NULL;
	char sift_id[5];
	IplImage *pdf_page;
	sqlite3 *db;
	sqlite3_stmt* stmt;
	
	openDB(DB_PATH,&db);	
	sprintf(sift_id,"%d",target_sift->id_sift);

	printf("SIFT_ID: %s\n",sift_id);
	siftID=chainString(sift_id,"'");
	query=chainString("select pages.name,pages.path from pages,sifts where pages.id_pages=sifts.id_pages and sifts.id_sift='",siftID);
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
	free(siftID);
	free(image_uri);
}
char* findPdfFileInDB(char* test_image,int* tlx,int* tly,int* width,int* height,int *page_number)
/*Returns the name of the most probable pdf from wich the photo test_image has been taken and
 * set the page_number pointer to the right page in the pdf and
 * set the coords and dimensions wrt the red circled area in the test_image.*/
{
	deleteLog();
	CvPoint br,tl;
	IplImage *input_image;
	CvMat *transformation_matrix=NULL;
	char *test_siftFilename = "/tmp/testSiftFile";
	struct SiftFileData *db_target_sift=NULL;
	char *pdfFilename=NULL;
	
	input_image=preProcessTestImage(test_image,&tl,&br);

#ifdef DEBUG	
	printf("Saving SIFT to file...\n");
#endif
	saveSiftToFile(input_image,test_siftFilename);
	
	cvReleaseImage(&input_image);
	
#ifdef DEBUG
	printf("Beginning research in DB.\n");
#endif
	struct list_head *thread_list;
	thread_list=getJobsResults(test_siftFilename);//db_target_sift=getBestMatchInDB(test_siftFilename);
#ifdef DEBUG
	//printf("Winner is: %s.\n",db_target_sift->uri);
#endif
	logFirstSiftNames(thread_list);
	int i=0;
	do
	{
		db_target_sift=getSiftData(thread_list,i);
		if(db_target_sift!=NULL)
		{
			getSiftPdfCoords(db_target_sift,&pdfFilename,page_number);
			
	#ifdef DEBUG
			printf("pdfFilename: %s\nPage number: %d.\n",pdfFilename,*page_number);
	#endif
			
			int useless;
			transformation_matrix=getProjectionAndMatchText(test_siftFilename,db_target_sift->uri,&useless);
			
			if(transformation_matrix!=NULL)
			{
				perspectiveTrasformation(transformation_matrix,&br);
				perspectiveTrasformation(transformation_matrix,&tl);

				*tlx = tl.x;
				*tly = tl.y;
				*width = br.x - tl.x;
				*height = br.y - tl.y;
	#ifdef DEBUG
				showResult(db_target_sift,&tl,&br);
	#endif
				
				destroySiftFileData(db_target_sift);
			}
			else
			{
				destroySiftFileData(db_target_sift);
				dynStringAssignement(&pdfFilename,PDF_NOT_FOUND);
			}
		}
		else
			dynStringAssignement(&pdfFilename,PDF_NOT_FOUND);

	i++;
	}while(i<NUMBER_OF_TRIES && (transformation_matrix==NULL));
	
	if(transformation_matrix!=NULL)
	{
		cvReleaseMat(&transformation_matrix);
		logToFile(test_image);
		logOrderOfMatching(i);
	}	
	destroyJobs(thread_list);
	free(thread_list);
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
