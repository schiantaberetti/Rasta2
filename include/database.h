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

void openDB(char* databaseFile,sqlite3 **db){
	/*open a db in the sqlite format*/
	int value;
	value = sqlite3_open(databaseFile, db);
	if( value ){
	      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	      sqlite3_close(db);	      
	}
}


void queryDB(char* query,sqlite3 **db){
	/*execute a query onto the database in the sqlite format*/
	sqlite3_stmt *stmt;
	int value;
	int col=0;
	//executes the query
	value = sqlite3_prepare_v2(*db,query,-1,&stmt,0);
	if(value){
		printf("Selecting data from DB Failed\n");
		exit(0);
	}
	// Read the number of rows fetched
	int cols = sqlite3_column_count(stmt);
	while(1){
		// fetch a row’s status
		value = sqlite3_step(stmt);

		if(value == SQLITE_ROW){
		// SQLITE_ROW means fetched a row

		// sqlite3_column_text returns a const void* , typecast it to const char*
			for(col=0 ; col<cols;col++){
				const char *val = (const char*)sqlite3_column_text(stmt,col);
				printf("%s = %s\t",sqlite3_column_name(stmt,col),val);
			}
			printf("\n");
		}
		else if(value == SQLITE_DONE){
			// All rows finished
			printf("All rows fetched\n");
			break;
		}
		else{
			// Some error encountered
			printf("Some error encountered\n");
			exit(0);
		}
	}
}


void closeDB(sqlite3 **db){
	/*close a db in the sqlite format*/
	sqlite3_close(*db);
}



#endif
