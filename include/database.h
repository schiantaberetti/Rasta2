#ifndef _database_h_
#define _database_h_ 1


#include <stdio.h>
#include "sqlite3.h"

#define DB_PATH "sqlite-database/database_sqlite"
#define PAPER_NAME 0
#define PAGE_NAME 1
#define PAPER_PATH 2
#define PAGE_PATH 3
#define NUMBER_OF_PAGE 4

struct DBInfo{
//Info from db
        char* paperName;
        char* pageName;
	char* paperPath;
	char* pagePath;
        int numberOfPage;
};

extern void openDB(char* databaseFile,sqlite3 **db);


extern void queryDB(char* query,sqlite3_stmt** stmt,sqlite3 **db);


extern void closeDB(sqlite3 **db);


extern int fetchQuery(sqlite3_stmt** stmt,struct DBInfo **dbInfo);


#endif
