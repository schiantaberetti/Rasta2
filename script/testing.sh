#!/bin/bash
function calculate() {
	if [ $# -gt 0 ];then 
		echo "scale=4; $1" | bc ;
	fi
}

function is_point_out(){
	x=$1
	y=$2
	
	width=$3
	height=$4
	
	let br_x=x+width
	let br_y=y+height
	
	test_point_x=$5
	test_point_y=$6
	
	if [ $test_point_x -lt $x -o $test_point_y -lt $y -o $test_point_y -gt $br_y -o $test_point_x -gt $br_x ];then
		echo "YES"
		
	fi	

}

function test_rectangle(){
	test_image=$5
	up_point=`sqlite3 ../test/database_test.db "select up_x,up_y from test_images where name='$test_image'" | sed -e 's/|/ /'`
	dx_point=`sqlite3 ../test/database_test.db "select dx_x,dx_y from test_images where name='$test_image'" | sed -e 's/|/ /'`
	sx_point=`sqlite3 ../test/database_test.db "select sx_x,sx_y from test_images where name='$test_image'" | sed -e 's/|/ /'`
	inner_point=`sqlite3 ../test/database_test.db "select inner_x,inner_y from test_images where name='$test_image'" | sed -e 's/|/ /'`
	down_point=`sqlite3 ../test/database_test.db "select down_x,down_y from test_images where name='$test_image'" | sed -e 's/|/ /'`

	if [ ! `is_point_out $1 $2 $3 $4 $inner_point` ];then
		
		if [ `is_point_out $1 $2 $3 $4 $sx_point` ];then
			
			if [ `is_point_out $1 $2 $3 $4 $dx_point` ];then
				
				if [ `is_point_out $1 $2 $3 $4 $up_point` ];then
					
					if [ `is_point_out $1 $2 $3 $4 $down_point` ];then
						
						echo "YES"
					else echo "NO"
					fi
				else echo "NO"
				fi
			else echo "NO"
			fi
		else echo "NO"
		fi
	else echo "NO"
	fi	
}
function matchDBResult(){
	res="YES"
	program_out=$1
	test_image=$2
	
	pdf_name=`sqlite3 ../test/database_test.db "select name_of_pdf from test_images where name='$test_image'"`
	number_page=`sqlite3 ../test/database_test.db "select number_of_pages from test_images where name='$test_image'"`
	
	if [ $pdf_name=`echo $program_out | cut -d ' ' -f 1` -a $number_page= `echo $program_out | cut -d ' ' -f 2`];then
		test_rectangle 	`echo $program_out | cut -d ' ' -f 3` `echo $program_out | cut -d ' ' -f 4` `echo $program_out | cut -d ' ' -f 5` `echo $program_out | cut -d ' ' -f 6` $test_image
	else
		echo "NO"
	fi
	
	echo $res
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
	test_passed=0;
	for image_test in `sqlite3 "$SQLITE_DB" "$LIST_TEST_IMAGES"`; do
		echo "Testing query = $image_test number = $number_of_query"
		START=`date +%s%N`	

		output_string=`cd .. && "$OUTPUT_DIR/$EXECUTABLE" "$TEST_IMG_DIR/$image_test" && cd $WORKING_DIR`

		FINISH=`date +%s%N`

		if [ `matchDBResult $output_string $image_test`="YES" ] ; then 
			let test_passed=test_passed+1
		fi
		
        ELAPSED=` calculate "( $FINISH - $START)/1000000" `
		total_testset_time=`calculate "$ELAPSED + $total_testset_time"`
		let number_of_query=number_of_query+1;		
	done	 

	# SAVING TEST RESULT
	total_testset_time=calculate "$total_testset_time/$number_of_test_image" 
	accuracy=calculate "$test_passed/$number_of_test_image"
	echo $DB_pdf_size $number_of_pages $total_testset_time $accuracy >> "$OUTPUT_DIR/$RESULT_FILE"

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
