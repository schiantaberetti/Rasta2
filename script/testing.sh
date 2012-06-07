#!/bin/bash
function calculate() {
	if [ $# -gt 0 ];then 
		echo "scale=4; $1" | bc ;
	fi
}
WORKING_DIR=`pwd`
DB_INC_STEP=1
OUTPUT_DIR="`readlink -f "../"`"
EXECUTABLE="pdfextractor"
PDF_DIR_MARINAI="`readlink -f "../database/pdf-marinai"`"
PDF_DIR="`readlink -f "../database/pdf"`"
IMG_DIR="`readlink -f "../database/pdf_img"`"
DB_DIR="`readlink -f "../database"`"
TEST_IMG_DIR="`readlink -f "../test-img-marinai"`"
SQLITE_DB="../test/database_test.db"
LIST_TEST_IMAGES="select distinct name from test_images"
COUNT_QUERY="select distinct count(name) from test_images"
#NAME OF THE RESULT FILE
RESULT_FILE="test_result.txt"

#TAKE THE NUMBER OF PDF CONSIDERED IN THE DB
DB_pdf_size=0;
#TIME IN MILLISECONDS
total_testset_time=0;

number_of_query=1;

#create the db-test
./createTestDB.sh
#NUMBER OF TEST IMAGE
number_of_test_image=`sqlite3 "$SQLITE_DB" "$COUNT_QUERY"`;

#IF A PREVIOUS RESULT FILE EXIST THEN DELETE IT
if [ -f "$OUTPUT_DIR/$RESULT_FILE" ];then 
	rm -f $OUTPUT_DIR/$RESULT_FILE
fi

#################################################
while [ `ls -A $PDF_DIR_MARINAI` ]; do
	# Moving pdf chunk to pdf directory
	echo "Moving pdf chunk to pdf directory."
	for pdf_file in `ls -1 "$PDF_DIR_MARINAI/" |tail -n $DB_INC_STEP| sed 's#.*/##' `;do
		mv "$PDF_DIR_MARINAI/$pdf_file" "$PDF_DIR"
	done
	./createDB.sh
	
	let DB_pdf_size=DB_pdf_size+DB_INC_STEP
	
	echo "Execution over the test images."
	number_of_pages=`ls $IMG_DIR/*.jpg | wc -l`;
	echo "-----Number of pdf in the db = $DB_pdf_size number of pages = $number_of_pages-----"
	
	total_testset_time=0;        
	number_of_query=0;
	for image_test in `sqlite3 "$SQLITE_DB" "$LIST_TEST_IMAGES"`; do
		echo "Testing query = $image_test number = $number_of_query"
		START=`date +%s%N`	

		output_string=`cd .. && "$OUTPUT_DIR/$EXECUTABLE" "$TEST_IMG_DIR/$image_test" && cd $WORKING_DIR`

		FINISH=`date +%s%N`
        ELAPSED=` calculate "( $FINISH - $START)/1000000" `
		total_testset_time=`calculate "$ELAPSED + $total_testset_time"`
		let number_of_query=number_of_query+1;		
	done	 

	# SAVING TEST RESULT
	total_testset_time=calculate "$total_testset_time/$number_of_test_image" 
	echo $DB_pdf_size $number_of_pages $total_testset_time >> "$OUTPUT_DIR/$RESULT_FILE"

done

echo "Moving back the pdfs."
#UNDO THE MOVE FOR A NEW UTILIZATION
for pdf_file in `ls -1 $PDF_DIR/*.pdf | sed 's#.*/##' `; do
	mv $PDF_DIR/$pdf_file $PDF_DIR_MARINAI
done

#SUM= '0'
#START=`date +%s%N`
#ELAPSED=`expr $FINISH - $START`
#ELAPSED=`expr $ELAPSED / 1000000`
#echo "Elapsed time: $ELAPSED"
