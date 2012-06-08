#!/bin/bash


function fatal_error() {
if [ $# -gt 0 ]; then
echo $1
fi
echo "Aborting..."
exit 1
}
function print_status() {
if [ $# -gt 0 ]; then
echo $1
if [ $# -gt 1 ]; then
if [ $2 = "VISUAL" ]; then
type convert >/dev/null 2>&1 && zenity --info --title="$0" --text="$1"
fi
fi
fi
}

PDF_DIR="`readlink -f "../database/pdf"`"
DB_DIR="`readlink -f "../database"`"
TEST_IMG_DIR="`readlink -f "../test-img-marinai"`"
SQLITE_DB="../test/database_test.db"


# INITIALIZE SQULITE3 DB
print_status "Reinitializing SQLITE TEST-DB." DEBUG
if [ -f "$SQLITE_DB" ];then
rm "$SQLITE_DB"
fi
cat /dev/null > "$SQLITE_DB"
echo "CREATE TABLE test_images (id_test INTEGER PRIMARY KEY,name varchar,path varchar,name_of_pdf varchar,number_of_pages int,sx_x int,sx_y int,dx_x int,dx_y int,up_x int,up_y,down_x int,down_y,inner_x int,inner_y int);">/tmp/sqlitetmpfile
sqlite3 "$SQLITE_DB" < /tmp/sqlitetmpfile

# INSERT DATA OF THE TEST_IMAGES
sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a037_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a037.pdf','1',0,0,0,0,0,0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a099_page4.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a099.pdf','4',0,390,304,390,174,324,174,462,174,393)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a207_page0.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a207.pdf','0',0,0,0,0,0,0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a409_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a409.pdf','1',0,0,0,0,0,0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a678_formula_page3.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a678.pdf','3',0,0,0,0,0,0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a678_immagine_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a678.pdf','1',0,0,0,0,0,0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a678_paragrafo2_page3.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a678.pdf','3',0,0,0,0,0,0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a678_paragrafo3_page3.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a678.pdf','3',0,0,0,0,0,0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a678_paragrafo_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a678.pdf','1',0,0,0,0,0,0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a957_formula_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a957.pdf','1',0,0,0,0,0,0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520a957_frase_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a957.pdf','1',0,0,0,0,0,0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520b235_page3.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520b235.pdf','3',0,0,0,0,0,0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520b235_paragrafo2_page3.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520b235.pdf','3',0,0,0,0,0,0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,sx_x,sx_y,dx_x,dx_y,up_x,up_y,down_x,down_y,inner_x,inner_y) VALUES ('test-4520z029_page0.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520z029.pdf','0',0,0,0,0,0,0,0,0,0,0)";

echo "Insert the coordinates of the example"
./insertCoordsIntoTestDB.sh
