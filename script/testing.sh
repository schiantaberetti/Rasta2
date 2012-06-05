#!/bin/bash

OUTPUT_DIR="`readlink -f "../"`"
EXECUTABLE="pdfextractor"
PDF_DIR_MARINAI="`readlink -f "../database/pdf-marinai"`"
PDF_DIR="`readlink -f "../database/pdf"`"
DB_DIR="`readlink -f "../database"`"
TEST_IMG_DIR="`readlink -f "../test-img-marinai"`"
SQLITE_DB="database_test.db"
QUERY="select distinct name from test_images"
COUNT_QUERY="select distinct count(name) from test_images"
#NAME OF THE RESULT FILE
RESULT_FILE="result"

#TAKE THE NUMBER OF PDF EXAMINED
number_of_examined_pdf=1;
#TIME IN MILLISECONDS
elapsed_time=0;

#create the db-test
./createTestDb.sh
#NUMBER OF TEST IMAGE
number_of_test_image=`sqlite3 "$DB_DIR/$SQLITE_DB" "$COUNT_QUERY"`;

#IF A PREVIOUS RESULT FILE EXIST THEN DELETE IT
rm -f $OUTPUT_DIR/$RESULT_FILE

#MOVE THE TEST INTO PDF DIR
for pdf_file in `ls -1 $PDF_DIR_MARINAI/*.pdf | sed 's#.*/##' `; do	
	mv $PDF_DIR_MARINAI/$pdf_file $PDF_DIR
	./createDB_unparallelized.sh


#BRUTTURA DA ELIMINARE
	rm -rf database
	mkdir database
	cp $DB_DIR/database.db database
#BRUTTURA DA ELIMINARE
	for image_test in `sqlite3 "$DB_DIR/$SQLITE_DB" "$QUERY"`; do
		START=`date +%s%N`
		#pdfextractor		
		$OUTPUT_DIR/$EXECUTABLE $TEST_IMG_DIR/$image_test
		#sleep 0.1
		FINISH=`date +%s%N`
                ELAPSED=`expr $FINISH - $START`
		ELAPSED=`expr $ELAPSED / 1000000`
		elapsed_time=`expr $ELAPSED + $elapsed_time`		
	done
	elapsed_time=`expr $elapsed_time / $number_of_test_image`
	echo $number_of_examined_pdf $elapsed_time >> $OUTPUT_DIR/$RESULT_FILE
	elapsed_time=0;        
let number_of_examined_pdf=number_of_examined_pdf+1;
done

#BRUTTURA
	rm -r database


#UNDO THE MOVE FOR A NEW UTILIZATION
for pdf_file in `ls -1 $PDF_DIR/*.pdf | sed 's#.*/##' `; do
	mv $PDF_DIR/$pdf_file $PDF_DIR_MARINAI
done

#SUM= '0'
#START=`date +%s%N`
#ELAPSED=`expr $FINISH - $START`
#ELAPSED=`expr $ELAPSED / 1000000`
#echo "Elapsed time: $ELAPSED"
