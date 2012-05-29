#include "database.h"
#include <stdio.h>
#include "sqlite3.h"



void openDB(char* databaseFile,sqlite3 **db){
	/*open a db in the sqlite format*/
	int value;
	value = sqlite3_open(databaseFile, db);
	if( value ){
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

