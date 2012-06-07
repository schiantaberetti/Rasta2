#!/bin/bash
function calculate() {
	if [ $# -gt 0 ];then 
		echo "scale=4; $1" | bc ;
	fi
}

function display_image() {
	show_image="pippo.jpg"
	border_size=6
	borderx=` calculate "($3 + $border_size)" `
	bordery=` calculate "($4 + $border_size)" `

	`mogrify -fill red -stroke black -draw "rectangle $3,$4 $borderx,$bordery" -write $show_image $1/$2`
	borderx=` calculate "($5 + $border_size)" `
	bordery=` calculate "($6 + $border_size)" `
	`mogrify -fill red -stroke black -draw "rectangle $5,$6 $borderx,$bordery" $show_image`
	borderx=` calculate "($7 + $border_size)" `
	bordery=` calculate "($8 + $border_size)" `
	`mogrify -fill red -stroke black -draw "rectangle $7,$8 $borderx,$bordery" $show_image`
	borderx=` calculate "($9 + $border_size)" `
	bordery=` calculate "(${10} + $border_size)" `
	`mogrify -fill red -stroke black -draw "rectangle $9,${10} $borderx,$bordery" $show_image`
	borderx=` calculate "(${11} + $border_size)" `
	bordery=` calculate "(${12} + $border_size)" `
	`mogrify -fill red -stroke black -draw "rectangle ${11},${12} $borderx,$bordery" $show_image`
	display $show_image
	rm $show_image 
}


SQLITE_DB="../test/database_test.db"
TEST_IMAGE="4520a037-1.jpg"
IMG_SHOW_DIR="img_to_show"

sx=5
sy=280
dx=300
dy=$sy
bx=170
by=305
ux=$bx
uy=270
inx=70
iny=80

#path=`sqlite3 "$SQLITE_DB" "select path from test_images where name= '$TEST_IMAGE'"`;
display_image $IMG_SHOW_DIR $TEST_IMAGE $sx $sy $dx $dy $ux $uy $bx $by $inx $iny


# INSERT DATA OF THE TEST_IMAGES
#sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a037_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a037.pdf','1',0,0,0,0,0,0,0,0,0,0)";

#sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a099_page4.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a099.pdf','4',0,390,304,390,174,324,174,462,174,393)";

#sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a207_page0.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a207.pdf','0',0,0,0,0,0,0,0,0,0,0)";

#sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a409_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a409.pdf','1',0,0,0,0,0,0,0,0,0,0)";

#sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a678_formula_page3.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a678.pdf','3',0,0,0,0,0,0,0,0,0,0)";

#sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a678_immagine_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a678.pdf','1',0,0,0,0,0,0,0,0,0,0)";

#sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a678_paragrafo2_page3.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a678.pdf','3',0,0,0,0,0,0,0,0,0,0)";

#sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a678_paragrafo3_page3.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a678.pdf','3',0,0,0,0,0,0,0,0,0,0)";

#sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a678_paragrafo_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a678.pdf','1',0,0,0,0,0,0,0,0,0,0)";

#sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a957_formula_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a957.pdf','1',0,0,0,0,0,0,0,0,0,0)";

#sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a957_frase_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a957.pdf','1',0,0,0,0,0,0,0,0,0,0)";

#sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520b235_page3.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520b235.pdf','3',0,0,0,0,0,0,0,0,0,0)";

#sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520b235_paragrafo2_page3.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520b235.pdf','3',0,0,0,0,0,0,0,0,0,0)";

#sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520z029_page0.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520z029.pdf','0',0,0,0,0,0,0,0,0,0,0)";

