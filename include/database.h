#ifndef _database_h_
#define _database_h_ 1


#include <stdio.h>
#include "sqlite3.h"



struct {
//Info from db
        char* paperName;
        char* pageName;
	char* paperPath;
	char* pagePath;
        int numberOfPage;
}DBInfo;

extern void openDB(char* databaseFile,sqlite3 **db);


extern void queryDB(char* query,sqlite3_stmt** stmt,sqlite3 **db);


extern void closeDB(sqlite3 **db);


extern int fetchQuery(sqlite3_stmt** stmt);

#endif
