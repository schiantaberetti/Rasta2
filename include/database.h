#ifndef _database_h_
#define _database_h_ 1


#include <stdio.h>
#include "sqlite3.h"

#define DB_PATH "database/database.db"
#define PAPER_NAME 0
#define PAGE_NAME 1
#define PAPER_PATH 2
#define PAGE_PATH 3
#define NUMBER_OF_PAGE 4
#define SIFT_NAME 5
#define SIFT_PATH 6

#define DB_SIFT_ID 0
#define DB_SIFT_FILENAME 1
#define DB_SIFT_PATH 2
#define DB_SIFT_ID_PAGES 3

struct DBInfo{
//Info from db
        char* paperName;
        char* pageName;
	char* paperPath;
	char* pagePath;
	char* siftPath;
	char* siftName;
        int numberOfPage;
};

struct SiftFileData{
	int id_sift;
	char *name;
	char *path;
	int id_pages;
	char *uri;
};

extern void openDB(char* databaseFile,sqlite3 **db);


extern void queryDB(char* query,sqlite3_stmt** stmt,sqlite3 **db);


extern void closeDB(sqlite3 **db);


extern int fetchQuery(sqlite3_stmt** stmt,struct DBInfo **dbInfo);


#endif
