#include "database.h"
#include <stdio.h>
#include "sqlite3.h"

void logOrderOfMatching(int n)
{
	char note[80];
	sprintf(note,"Page found in order: %d",n);
	logToFile(note);
}
void deleteLog()
{
	remove(LOGFILE);
}
void logToFile(char *note)
{
	FILE *file;
	file = fopen(LOGFILE,"a+"); /* apend file (add text to
	a file or create a file if it does not exist.*/
	fprintf(file,"%s\n",note); /*writes*/
	fclose(file); /*done!*/
	return 0; 
}

void openDB(char* databaseFile,sqlite3 **db){
	/*open a db in the sqlite format*/
	int value;
	value = sqlite3_open(databaseFile, db);
	if( value ){
		printf("%s",databaseFile);
	      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	      sqlite3_close(db);	      
	}
}


void queryDB(char* query,sqlite3_stmt** stmt,sqlite3 **db){
	/*execute a query onto the database in the sqlite format*/
	int value;
	int col=0;
	//executes the query
	value = sqlite3_prepare_v2(*db,query,-1,stmt,0);
	if(value){
		printf("Selecting data from DB Failed\n");
		exit(0);
	}
}


void closeDB(sqlite3 **db){
	/*close a db in the sqlite format*/
	sqlite3_close(*db);
}

void dynStringAssignement(char **dst,const char *src)
{
	if((*dst)!=NULL)
		free(*dst);
	if(src!=NULL)
	{
		(*dst)=(char*)malloc(sizeof(char)*(strlen(src)+1));
		strcpy(*dst,src);
	}
}
char * chainString(const char* str1,const char* str2)
{
	char * neo=(char*)malloc(sizeof(char)*(strlen(str1)+strlen(str2)+1));
	strcpy(neo,str1);
	strcat(neo,str2);
	return neo;
}
struct SiftFileData* dynCopy(const struct SiftFileData* src)
{
	struct SiftFileData* neo = (struct SiftFileData *)malloc(sizeof(struct SiftFileData));
	neo->id_pages = src->id_pages;
	neo->id_sift = src -> id_sift;
	neo->uri=NULL;
	neo->name=NULL;
	neo->path=NULL;
	
	dynStringAssignement(&(neo->uri),src->uri);
	dynStringAssignement(&(neo->name),src->name);
	dynStringAssignement(&(neo->path),src->path);
	return neo;
}
void destroySiftFileData(struct SiftFileData *sfd)
{
	free(sfd->name);
	free(sfd->path);
	free(sfd->uri);
	free(sfd);
}
int fetchImageURI(sqlite3_stmt** stmt,char **image_uri)
{
	int cols = sqlite3_column_count(*stmt);
	int length,value;
	char *name=NULL,*path=NULL,*uri=NULL;
	// fetch a row’s status
	value = sqlite3_step(*stmt);

	if(value == SQLITE_ROW){
		name=(char*)sqlite3_column_text(*stmt,0);
		path=(char*)sqlite3_column_text(*stmt,1);
		uri=chainString(path,name);
		dynStringAssignement(image_uri,uri);
		
		free(uri);
		return(1);
	}
	else if(value == SQLITE_DONE){
		// All rows finished
		return(0);
	}
	else{
		// Some error encountered
		printf("Some error encountered\n");
		return(0);
	}
	
	
}
int fetchPDFInfo(sqlite3_stmt** stmt,char **pdfFilename,int *page_number)
{
	int cols = sqlite3_column_count(*stmt);
	int length,value;
	char *name=NULL,*path=NULL,*uri=NULL;
	// fetch a row’s status
	value = sqlite3_step(*stmt);

	if(value == SQLITE_ROW){
		(*page_number)=atoi((const char*)sqlite3_column_text(*stmt,0));
		name=(char*)sqlite3_column_text(*stmt,1);
		path=(char*)sqlite3_column_text(*stmt,2);
		uri=chainString(path,name);
		dynStringAssignement(pdfFilename,uri);
		
		free(uri);
		return(1);
	}
	else if(value == SQLITE_DONE){
		// All rows finished
		return(0);
	}
	else{
		// Some error encountered
		printf("Some error encountered\n");
		return(0);
	}
	
}
int fetchSiftQuery(sqlite3_stmt** stmt,struct SiftFileData **siftMetaData){
	// Read the number of rows fetched
	int cols = sqlite3_column_count(*stmt);
	int length,value;
	char * uri;
	// fetch a row’s status
	value = sqlite3_step(*stmt);

	if(value == SQLITE_ROW){
	// SQLITE_ROW means fetched a row
		if((*siftMetaData)!=NULL)
			free(*siftMetaData);
		(*siftMetaData)=(struct SiftFileData *)malloc(sizeof(struct SiftFileData));
		(*siftMetaData)->name=NULL;
		(*siftMetaData)->path=NULL;
		(*siftMetaData)->uri=NULL;
	

	// sqlite3_column_text returns a const void* , typecast it to const char*
		(*siftMetaData)->id_sift=atoi((const char*)sqlite3_column_text(*stmt,DB_SIFT_ID));
		dynStringAssignement(&((*siftMetaData)->name),(char*)sqlite3_column_text(*stmt,DB_SIFT_FILENAME));
		dynStringAssignement(&((*siftMetaData)->path),(char*)sqlite3_column_text(*stmt,DB_SIFT_PATH));
		(*siftMetaData)->id_pages=atoi((const char*)sqlite3_column_text(*stmt,DB_SIFT_ID_PAGES));
		
		uri=chainString((*siftMetaData)->path,(*siftMetaData)->name);
		dynStringAssignement(&((*siftMetaData)->uri),uri);
		free(uri);
		
		return(1);
	}
	else if(value == SQLITE_DONE){
		// All rows finished
		return(0);
	}
	else{
		// Some error encountered
		printf("Some error encountered\n");
		return(0);
	}


}

int fetchQuery(sqlite3_stmt** stmt,struct DBInfo **dbInfo){
	// Read the number of rows fetched
	int cols = sqlite3_column_count(*stmt);
	int length,value;
	// fetch a row’s status
	value = sqlite3_step(*stmt);

	if(value == SQLITE_ROW){
	// SQLITE_ROW means fetched a row

	// sqlite3_column_text returns a const void* , typecast it to const char*
		(*dbInfo)->paperName=(char*)sqlite3_column_text(*stmt,PAPER_NAME);
		(*dbInfo)->pageName=(char*)sqlite3_column_text(*stmt,PAGE_NAME);
		(*dbInfo)->paperPath=(char*)sqlite3_column_text(*stmt,PAPER_PATH);
		(*dbInfo)->pagePath=(char*)sqlite3_column_text(*stmt,PAGE_PATH);
		(*dbInfo)->siftPath=(char*)sqlite3_column_text(*stmt,SIFT_PATH);
		(*dbInfo)->siftName=(char*)sqlite3_column_text(*stmt,SIFT_NAME);
		(*dbInfo)->numberOfPage=atoi((const char*)sqlite3_column_text(*stmt,NUMBER_OF_PAGE));
		
		
		return(1);
	}
	else if(value == SQLITE_DONE){
		// All rows finished
		return(0);
	}
	else{
		// Some error encountered
		printf("Some error encountered\n");
		return(0);
	}


}

